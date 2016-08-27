/*
*******************************************************************************
*
*   Copyright (C) 1999-2004, International Business Machines
*   Corporation and others.  All Rights Reserved.
*
*******************************************************************************
*   file name:  utf.h
*   encoding:   US-ASCII
*   tab size:   8 (not used)
*   indentation:4
*
*   created on: 1999sep09
*   created by: Markus W. Scherer
*/

#ifndef __third_party_icu_icu_utf_h__
#define __third_party_icu_icu_utf_h__

namespace base_icu
{

    typedef uint32 UChar32;
    typedef int8 UBool;

    // General ---------------------------------------------------------------------
    // from utf.h

    /**
    * This value is intended for sentinel values for APIs that
    * (take or) return single code points (UChar32).
    * It is outside of the Unicode code point range 0..0x10ffff.
    *
    * For example, a "done" or "error" value in a new API
    * could be indicated with CBU_SENTINEL.
    *
    * ICU APIs designed before ICU 2.4 usually define service-specific "done"
    * values, mostly 0xffff.
    * Those may need to be distinguished from
    * actual U+ffff text contents by calling functions like
    * CharacterIterator::hasNext() or UnicodeString::length().
    *
    * @return -1
    * @see UChar32
    * @stable ICU 2.4
    */
#define CBU_SENTINEL (-1)

    /**
    * Is this code point a Unicode noncharacter?
    * @param c 32-bit code point
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU_IS_UNICODE_NONCHAR(c) \
    ((c)>=0xfdd0 && \
    ((uint32)(c)<=0xfdef || ((c)&0xfffe)==0xfffe) && \
    (uint32)(c)<=0x10ffff)

    /**
    * Is c a Unicode code point value (0..U+10ffff)
    * that can be assigned a character?
    *
    * Code points that are not characters include:
    * - single surrogate code points (U+d800..U+dfff, 2048 code points)
    * - the last two code points on each plane (U+__fffe and U+__ffff, 34 code points)
    * - U+fdd0..U+fdef (new with Unicode 3.1, 32 code points)
    * - the highest Unicode code point value is U+10ffff
    *
    * This means that all code points below U+d800 are character code points,
    * and that boundary is tested first for performance.
    *
    * @param c 32-bit code point
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU_IS_UNICODE_CHAR(c) \
    ((uint32)(c)<0xd800 || \
    ((uint32)(c)>0xdfff && \
    (uint32)(c)<=0x10ffff && \
    !U_IS_UNICODE_NONCHAR(c)))

    /**
    * Is this code point a surrogate (U+d800..U+dfff)?
    * @param c 32-bit code point
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU_IS_SURROGATE(c) (((c)&0xfffff800)==0xd800)

    /**
    * Assuming c is a surrogate code point (U_IS_SURROGATE(c)),
    * is it a lead surrogate?
    * @param c 32-bit code point
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU_IS_SURROGATE_LEAD(c) (((c)&0x400)==0)


    // UTF-8 macros ----------------------------------------------------------------
    // from utf8.h

    //extern const uint8 utf8_countTrailBytes[256];
    //const uint8 utf8_countTrailBytes[256];

     /*
    * This table could be replaced on many machines by
    * a few lines of assembler code using an
    * "index of first 0-bit from msb" instruction and
    * one or two more integer instructions.
    *
    * For example, on an i386, do something like
    * - MOV AL, leadByte
    * - NOT AL         (8-bit, leave b15..b8==0..0, reverse only b7..b0)
    * - MOV AH, 0
    * - BSR BX, AX     (16-bit)
    * - MOV AX, 6      (result)
    * - JZ finish      (ZF==1 if leadByte==0xff)
    * - SUB AX, BX (result)
    * -finish:
    * (BSR: Bit Scan Reverse, scans for a 1-bit, starting from the MSB)
    *
    * In Unicode, all UTF-8 byte sequences with more than 4 bytes are illegal;
    * lead bytes above 0xf4 are illegal.
    * We keep them in this table for skipping long ISO 10646-UTF-8 sequences.
    */
    const uint8 utf8_countTrailBytes[256] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3,
        3, 3, 3,    /* illegal in Unicode */
        4, 4, 4, 4, /* illegal in Unicode */
        5, 5,       /* illegal in Unicode */
        0, 0        /* illegal bytes 0xfe and 0xff */
    };

    /**
    * Count the trail bytes for a UTF-8 lead byte.
    * @internal
    */
#define CBU8_COUNT_TRAIL_BYTES(leadByte) (base_icu::utf8_countTrailBytes[(uint8)leadByte])

    /**
    * Mask a UTF-8 lead byte, leave only the lower bits that form part of the code point value.
    * @internal
    */
#define CBU8_MASK_LEAD_BYTE(leadByte, countTrailBytes) ((leadByte)&=(1<<(6-(countTrailBytes)))-1)

    /**
    * Does this code unit (byte) encode a code point by itself (US-ASCII 0..0x7f)?
    * @param c 8-bit code unit (byte)
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU8_IS_SINGLE(c) (((c)&0x80)==0)

    /**
    * Is this code unit (byte) a UTF-8 lead byte?
    * @param c 8-bit code unit (byte)
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU8_IS_LEAD(c) ((uint8)((c)-0xc0)<0x3e)

    /**
    * Is this code unit (byte) a UTF-8 trail byte?
    * @param c 8-bit code unit (byte)
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU8_IS_TRAIL(c) (((c)&0xc0)==0x80)

    /**
    * How many code units (bytes) are used for the UTF-8 encoding
    * of this Unicode code point?
    * @param c 32-bit code point
    * @return 1..4, or 0 if c is a surrogate or not a Unicode code point
    * @stable ICU 2.4
    */
#define CBU8_LENGTH(c) \
    ((uint32)(c)<=0x7f ? 1 : \
    ((uint32)(c)<=0x7ff ? 2 : \
    ((uint32)(c)<=0xd7ff ? 3 : \
    ((uint32)(c)<=0xdfff || (uint32)(c)>0x10ffff ? 0 : \
    ((uint32)(c)<=0xffff ? 3 : 4)\
    ) \
    ) \
    ) \
    )

    /**
    * The maximum number of UTF-8 code units (bytes) per Unicode code point (U+0000..U+10ffff).
    * @return 4
    * @stable ICU 2.4
    */
#define CBU8_MAX_LENGTH 4

    /**
    * Function for handling "next code point" with error-checking.
    * @internal
    */
    UChar32 utf8_nextCharSafeBody(const uint8 *s, int32 *pi, int32 length, UChar32 c, UBool strict);

    /**
    * Get a code point from a string at a code point boundary offset,
    * and advance the offset to the next code point boundary.
    * (Post-incrementing forward iteration.)
    * "Safe" macro, checks for illegal sequences and for string boundaries.
    *
    * The offset may point to the lead byte of a multi-byte sequence,
    * in which case the macro will read the whole sequence.
    * If the offset points to a trail byte or an illegal UTF-8 sequence, then
    * c is set to a negative value.
    *
    * @param s const uint8 * string
    * @param i string offset, i<length
    * @param length string length
    * @param c output UChar32 variable, set to <0 in case of an error
    * @see CBU8_NEXT_UNSAFE
    * @stable ICU 2.4
    */
#define CBU8_NEXT(s, i, length, c) { \
    (c)=(s)[(i)++]; \
    if(((uint8)(c))>=0x80) { \
    if(CBU8_IS_LEAD(c)) { \
    (c)=base_icu::utf8_nextCharSafeBody((const uint8 *)s, &(i), (int32)(length), c, -1); \
    } else { \
    (c)=CBU_SENTINEL; \
    } \
    } \
    }

    /**
    * Append a code point to a string, overwriting 1 to 4 bytes.
    * The offset points to the current end of the string contents
    * and is advanced (post-increment).
    * "Unsafe" macro, assumes a valid code point and sufficient space in the string.
    * Otherwise, the result is undefined.
    *
    * @param s const uint8 * string buffer
    * @param i string offset
    * @param c code point to append
    * @see CBU8_APPEND
    * @stable ICU 2.4
    */
#define CBU8_APPEND_UNSAFE(s, i, c) { \
    if((uint32)(c)<=0x7f) { \
    (s)[(i)++]=(uint8)(c); \
    } else { \
    if((uint32)(c)<=0x7ff) { \
    (s)[(i)++]=(uint8)(((c)>>6)|0xc0); \
    } else { \
    if((uint32)(c)<=0xffff) { \
    (s)[(i)++]=(uint8)(((c)>>12)|0xe0); \
    } else { \
    (s)[(i)++]=(uint8)(((c)>>18)|0xf0); \
    (s)[(i)++]=(uint8)((((c)>>12)&0x3f)|0x80); \
    } \
    (s)[(i)++]=(uint8)((((c)>>6)&0x3f)|0x80); \
    } \
    (s)[(i)++]=(uint8)(((c)&0x3f)|0x80); \
    } \
    }

    // UTF-16 macros ---------------------------------------------------------------
    // from utf16.h

    /**
    * Does this code unit alone encode a code point (BMP, not a surrogate)?
    * @param c 16-bit code unit
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU16_IS_SINGLE(c) !CBU_IS_SURROGATE(c)

    /**
    * Is this code unit a lead surrogate (U+d800..U+dbff)?
    * @param c 16-bit code unit
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU16_IS_LEAD(c) (((c)&0xfffffc00)==0xd800)

    /**
    * Is this code unit a trail surrogate (U+dc00..U+dfff)?
    * @param c 16-bit code unit
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU16_IS_TRAIL(c) (((c)&0xfffffc00)==0xdc00)

    /**
    * Is this code unit a surrogate (U+d800..U+dfff)?
    * @param c 16-bit code unit
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU16_IS_SURROGATE(c) CBU_IS_SURROGATE(c)

    /**
    * Assuming c is a surrogate code point (U16_IS_SURROGATE(c)),
    * is it a lead surrogate?
    * @param c 16-bit code unit
    * @return TRUE or FALSE
    * @stable ICU 2.4
    */
#define CBU16_IS_SURROGATE_LEAD(c) (((c)&0x400)==0)

    /**
    * Helper constant for CBU16_GET_SUPPLEMENTARY.
    * @internal
    */
#define CBU16_SURROGATE_OFFSET ((0xd800<<10UL)+0xdc00-0x10000)

    /**
    * Get a supplementary code point value (U+10000..U+10ffff)
    * from its lead and trail surrogates.
    * The result is undefined if the input values are not
    * lead and trail surrogates.
    *
    * @param lead lead surrogate (U+d800..U+dbff)
    * @param trail trail surrogate (U+dc00..U+dfff)
    * @return supplementary code point (U+10000..U+10ffff)
    * @stable ICU 2.4
    */
#define CBU16_GET_SUPPLEMENTARY(lead, trail) \
    (((base_icu::UChar32)(lead)<<10UL)+(base_icu::UChar32)(trail)-CBU16_SURROGATE_OFFSET)


    /**
    * Get the lead surrogate (0xd800..0xdbff) for a
    * supplementary code point (0x10000..0x10ffff).
    * @param supplementary 32-bit code point (U+10000..U+10ffff)
    * @return lead surrogate (U+d800..U+dbff) for supplementary
    * @stable ICU 2.4
    */
#define CBU16_LEAD(supplementary) (UChar)(((supplementary)>>10)+0xd7c0)

    /**
    * Get the trail surrogate (0xdc00..0xdfff) for a
    * supplementary code point (0x10000..0x10ffff).
    * @param supplementary 32-bit code point (U+10000..U+10ffff)
    * @return trail surrogate (U+dc00..U+dfff) for supplementary
    * @stable ICU 2.4
    */
#define CBU16_TRAIL(supplementary) (UChar)(((supplementary)&0x3ff)|0xdc00)

    /**
    * How many 16-bit code units are used to encode this Unicode code point? (1 or 2)
    * The result is not defined if c is not a Unicode code point (U+0000..U+10ffff).
    * @param c 32-bit code point
    * @return 1 or 2
    * @stable ICU 2.4
    */
#define CBU16_LENGTH(c) ((uint32)(c)<=0xffff ? 1 : 2)

    /**
    * The maximum number of 16-bit code units per Unicode code point (U+0000..U+10ffff).
    * @return 2
    * @stable ICU 2.4
    */
#define CBU16_MAX_LENGTH 2

    /**
    * Get a code point from a string at a code point boundary offset,
    * and advance the offset to the next code point boundary.
    * (Post-incrementing forward iteration.)
    * "Safe" macro, handles unpaired surrogates and checks for string boundaries.
    *
    * The offset may point to the lead surrogate unit
    * for a supplementary code point, in which case the macro will read
    * the following trail surrogate as well.
    * If the offset points to a trail surrogate or
    * to a single, unpaired lead surrogate, then that itself
    * will be returned as the code point.
    *
    * @param s const UChar * string
    * @param i string offset, i<length
    * @param length string length
    * @param c output UChar32 variable
    * @stable ICU 2.4
    */
#define CBU16_NEXT(s, i, length, c) { \
    (c)=(s)[(i)++]; \
    if(CBU16_IS_LEAD(c)) { \
    uint16 __c2; \
    if((i)<(length) && CBU16_IS_TRAIL(__c2=(s)[(i)])) { \
    ++(i); \
    (c)=CBU16_GET_SUPPLEMENTARY((c), __c2); \
    } \
    } \
    }

    /**
    * Append a code point to a string, overwriting 1 or 2 code units.
    * The offset points to the current end of the string contents
    * and is advanced (post-increment).
    * "Unsafe" macro, assumes a valid code point and sufficient space in the string.
    * Otherwise, the result is undefined.
    *
    * @param s const UChar * string buffer
    * @param i string offset
    * @param c code point to append
    * @see CBU16_APPEND
    * @stable ICU 2.4
    */
#define CBU16_APPEND_UNSAFE(s, i, c) { \
    if((uint32)(c)<=0xffff) { \
    (s)[(i)++]=(uint16)(c); \
    } else { \
    (s)[(i)++]=(uint16)(((c)>>10)+0xd7c0); \
    (s)[(i)++]=(uint16)(((c)&0x3ff)|0xdc00); \
    } \
    }

} //namesapce base_icu



namespace base_icu
{

    /**
    * UTF8_ERROR_VALUE_1 and UTF8_ERROR_VALUE_2 are special error values for UTF-8,
    * which need 1 or 2 bytes in UTF-8:
    * \code
    * U+0015 = NAK = Negative Acknowledge, C0 control character
    * U+009f = highest C1 control character
    * \endcode
    *
    * These are used by UTF8_..._SAFE macros so that they can return an error value
    * that needs the same number of code units (bytes) as were seen by
    * a macro. They should be tested with UTF_IS_ERROR() or UTF_IS_VALID().
    *
    * @deprecated ICU 2.4. Obsolete, see utf_old.h.
    */
#define CBUTF8_ERROR_VALUE_1 0x15

    /**
    * See documentation on UTF8_ERROR_VALUE_1 for details.
    *
    * @deprecated ICU 2.4. Obsolete, see utf_old.h.
    */
#define CBUTF8_ERROR_VALUE_2 0x9f


    /**
    * Error value for all UTFs. This code point value will be set by macros with e>
    * checking if an error is detected.
    *
    * @deprecated ICU 2.4. Obsolete, see utf_old.h.
    */
#define CBUTF_ERROR_VALUE 0xffff

   

    static const UChar32 utf8_minLegal[4] ={ 0, 0x80, 0x800, 0x10000 };

    static const UChar32 utf8_errorValue[6] =
    {
        CBUTF8_ERROR_VALUE_1, CBUTF8_ERROR_VALUE_2, CBUTF_ERROR_VALUE, 0x10ffff,
        0x3ffffff, 0x7fffffff
    };

    /*
    * Handle the non-inline part of the U8_NEXT() macro and its obsolete sibling
    * UTF8_NEXT_CHAR_SAFE().
    *
    * The "strict" parameter controls the error behavior:
    * <0  "Safe" behavior of U8_NEXT(): All illegal byte sequences yield a negative
    *     code point result.
    *  0  Obsolete "safe" behavior of UTF8_NEXT_CHAR_SAFE(..., FALSE):
    *     All illegal byte sequences yield a positive code point such that this
    *     result code point would be encoded with the same number of bytes as
    *     the illegal sequence.
    * >0  Obsolete "strict" behavior of UTF8_NEXT_CHAR_SAFE(..., TRUE):
    *     Same as the obsolete "safe" behavior, but non-characters are also treated
    *     like illegal sequences.
    *
    * The special negative (<0) value -2 is used for lenient treatment of surrogate
    * code points as legal. Some implementations use this for roundtripping of
    * Unicode 16-bit strings that are not well-formed UTF-16, that is, they
    * contain unpaired surrogates.
    *
    * Note that a UBool is the same as an int8_t.
    */
    inline UChar32 utf8_nextCharSafeBody(const uint8 *s, int32 *pi, int32 length,
        UChar32 c, UBool strict)
    {
        int32 i=*pi;
        uint8 count=CBU8_COUNT_TRAIL_BYTES(c);
        if((i)+count<=(length)) {
            uint8 trail, illegal=0;

            CBU8_MASK_LEAD_BYTE((c), count);
            /* count==0 for illegally leading trail bytes and the illegal bytes 0xfe and 0xff */
            switch(count)
            {
                /* each branch falls through to the next one */
            case 5:
            case 4:
                /* count>=4 is always illegal: no more than 3 trail bytes in Unicode's UTF-8 */
                illegal=1;
                break;
            case 3:
                trail=s[(i)++];
                (c)=((c)<<6)|(trail&0x3f);
                if(c<0x110) {
                    illegal|=(trail&0xc0)^0x80;
                } else {
                    /* code point>0x10ffff, outside Unicode */
                    illegal=1;
                    break;
                }
            case 2:
                trail=s[(i)++];
                (c)=((c)<<6)|(trail&0x3f);
                illegal|=(trail&0xc0)^0x80;
            case 1:
                trail=s[(i)++];
                (c)=((c)<<6)|(trail&0x3f);
                illegal|=(trail&0xc0)^0x80;
                break;
            case 0:
                if(strict>=0) {
                    return CBUTF8_ERROR_VALUE_1;
                } else {
                    return CBU_SENTINEL;
                }
                /* no default branch to optimize switch()  - all values are covered */
            }

            /*
            * All the error handling should return a value
            * that needs count bytes so that UTF8_GET_CHAR_SAFE() works right.
            *
            * Starting with Unicode 3.0.1, non-shortest forms are illegal.
            * Starting with Unicode 3.2, surrogate code points must not be
            * encoded in UTF-8, and there are no irregular sequences any more.
            *
            * U8_ macros (new in ICU 2.4) return negative values for error conditions.
            */

            /* correct sequence - all trail bytes have (b7..b6)==(10)? */
            /* illegal is also set if count>=4 */
            if(illegal || (c)<utf8_minLegal[count] || (CBU_IS_SURROGATE(c) && strict!=-2)) {
                /* error handling */
                uint8 errorCount=count;
                /* don't go beyond this sequence */
                i=*pi;
                while(count>0 && CBU8_IS_TRAIL(s[i])) {
                    ++(i);
                    --count;
                }
                if(strict>=0) {
                    c=utf8_errorValue[errorCount-count];
                } else {
                    c=CBU_SENTINEL;
                }
            } else if((strict)>0 && CBU_IS_UNICODE_NONCHAR(c)) {
                /* strict: forbid non-characters like U+fffe */
                c=utf8_errorValue[count];
            }
        } else /* too few bytes left */ {
            /* error handling */
            int32 i0=i;
            /* don't just set (i)=(length) in case there is an illegal sequence */
            while((i)<(length) && CBU8_IS_TRAIL(s[i])) {
                ++(i);
            }
            if(strict>=0) {
                c=utf8_errorValue[i-i0];
            } else {
                c=CBU_SENTINEL;
            }
        }
        *pi=i;
        return c;
    }

} //namespace base_icu



#endif //__third_party_icu_icu_utf_h__