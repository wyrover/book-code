/*!
* \file cactus_string.cpp
* \brief 常用字符串函数实现
*
* 常用字符串函数实现
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#include <cactus/cactus_string.h>
#include <cactus/cactus_exception.h>
#include <atlutil.h>
#include <assert.h>

#pragma comment(lib, "Rpcrt4.lib")

namespace cactus
{

void ExpandString(stringlist& string_list, const string8& string, const char delimiter)
{
    char quote = 0;
    bool last_char_delimiter = true;
    const char* ptr = string.c_str();
    const char* start_ptr = NULL;
    const char* end_ptr = ptr;

    while (*ptr) {
        // Switch into quote mode if the last char was a delimeter ( excluding whitespace )
        // and we're not already in quote mode
        if (last_char_delimiter && !quote && (*ptr == '"' || *ptr == '\'')) {
            quote = *ptr;
        }
        // Switch out of quote mode if we encounter a quote that hasn't been escaped
        else if (*ptr == quote && *(ptr - 1) != '\\') {
            quote = 0;
        }
        // If we encouter a delimiter while not in quote mode, add the item to the list
        else if (*ptr == delimiter && !quote) {
            if (start_ptr)
                string_list.push_back(string8(start_ptr, end_ptr + 1));
            else
                string_list.push_back("");

            last_char_delimiter = true;
            start_ptr = NULL;
        }
        // Otherwise if its not white space or we're in quote mode, advance the pointers
        else if (!isspace(*ptr) || quote) {
            if (!start_ptr)
                start_ptr = ptr;

            end_ptr = ptr;
            last_char_delimiter = false;
        }

        ptr++;
    }

    // If there's data pending, add it.
    if (start_ptr)
        string_list.push_back(string8(start_ptr, end_ptr + 1));
}

void JoinString(string8& string, const stringlist& string_list, const char delimiter)
{
    for (size_t i = 0; i < string_list.size(); i++) {
        string += string_list[i];

        if (delimiter != '\0' && i < string_list.size() - 1)
            string = string + delimiter;
    }
}

stringlist split(const std::string& text, char separator)
{
    stringlist splittedText;
    std::string::const_iterator itStart = text.begin();

    while (!text.empty()) {
        std::string::const_iterator itSeparator = std::find(itStart,
                text.end(),
                separator);
        splittedText.push_back(text.substr(itStart - text.begin(),
                                           itSeparator - itStart));

        if (itSeparator == text.end())
            break;

        itStart = itSeparator + 1;
    }

    return splittedText;
}

string16list split(const string16& text, char16 separator)
{
    string16list splittedText;
    string16::const_iterator itStart = text.begin();

    while (!text.empty()) {
        string16::const_iterator itSeparator = std::find(itStart,
                                               text.end(),
                                               separator);
        splittedText.push_back(text.substr(itStart - text.begin(),
                                           itSeparator - itStart));

        if (itSeparator == text.end())
            break;

        itStart = itSeparator + 1;
    }

    return splittedText;
}

inline std::string wrap(const std::string &text, int wrapColumn)
{
    const char lineBreak = '\n';
    stringlist lines = split(text, lineBreak);
    std::string wrapped;

    for (stringlist::const_iterator it = lines.begin(); it != lines.end(); ++it) {
        if (it != lines.begin())
            wrapped += lineBreak;

        const std::string &line = *it;
        unsigned int index = 0;

        while (index < line.length()) {
            std::string lineSlice(line.substr(index, wrapColumn));
            wrapped += lineSlice;
            index += wrapColumn;

            if (index < line.length())
                wrapped += lineBreak;
        }
    }

    return wrapped;
}

inline string16 wrap(const string16& text, int wrapColumn)
{
    const char16 lineBreak = L'\n';
    string16list lines = split(text, lineBreak);
    string16 wrapped;

    for (string16list::const_iterator it = lines.begin(); it != lines.end(); ++it) {
        if (it != lines.begin())
            wrapped += lineBreak;

        const string16 &line = *it;
        unsigned int index = 0;

        while (index < line.length()) {
            string16 lineSlice(line.substr(index, wrapColumn));
            wrapped += lineSlice;
            index += wrapColumn;

            if (index < line.length())
                wrapped += lineBreak;
        }
    }

    return wrapped;
}

LPTSTR lstrcasestr(LPCTSTR phaystack, LPCTSTR pneedle)
// This is the locale-obeying variant of strcasestr.  It uses CharUpper/Lower in place of toupper/lower,
// which sees chars like ?as the same as ?(depending on code page/locale).  This function is about
// 1 to 8 times slower than strcasestr() depending on factors such as how many partial matches for needle
// are in haystack.
// License: GNU GPL
// Copyright (C) 1994,1996,1997,1998,1999,2000 Free Software Foundation, Inc.
// See strcasestr() for more comments.
{
    register const TBYTE *haystack, *needle;
    register unsigned bl, bu, cl, cu;
    haystack = (const TBYTE *) phaystack;
    needle = (const TBYTE *) pneedle;
    bl = (UINT)ltolower(*needle);

    if (bl != 0) {
        // Scan haystack until the first character of needle is found:
        bu = (UINT)(size_t)ltoupper(bl);
        haystack--;             /* possible ANSI violation */

        do {
            cl = *++haystack;

            if (cl == '\0')
                goto ret0;
        } while ((cl != bl) && (cl != bu));

        // See if the rest of needle is a one-for-one match with this part of haystack:
        cl = (UINT)ltolower(*++needle);

        if (cl == '\0')  // Since needle consists of only one character, it is already a match as found above.
            goto foundneedle;

        cu = (UINT)ltoupper(cl);
        ++needle;
        goto jin;

        for (;;) {
            register unsigned a;
            register const TBYTE *rhaystack, *rneedle;

            do {
                a = *++haystack;

                if (a == '\0')
                    goto ret0;

                if ((a == bl) || (a == bu))
                    break;

                a = *++haystack;

                if (a == '\0')
                    goto ret0;

shloop:
                ;
            } while ((a != bl) && (a != bu));

jin:
            a = *++haystack;

            if (a == '\0')  // Remaining part of haystack is shorter than needle.  No match.
                goto ret0;

            if ((a != cl) && (a != cu)) // This promising candidate is not a complete match.
                goto shloop;            // Start looking for another match on the first char of needle.

            rhaystack = haystack-- + 1;
            rneedle = needle;
            a = (UINT)ltolower(*rneedle);

            if ((UINT)ltolower(*rhaystack) == a)
                do {
                    if (a == '\0')
                        goto foundneedle;

                    ++rhaystack;
                    a = (UINT)ltolower(*++needle);

                    if ((UINT)ltolower(*rhaystack) != a)
                        break;

                    if (a == '\0')
                        goto foundneedle;

                    ++rhaystack;
                    a = (UINT)ltolower(*++needle);
                } while ((UINT)ltolower(*rhaystack) == a);

            needle = rneedle;       /* took the register-poor approach */

            if (a == '\0')
                break;
        } // for(;;)
    } // if (bl != '\0')

foundneedle:
    return (LPTSTR) haystack;
ret0:
    return 0;
}

LPTSTR tcscasestr(LPCTSTR phaystack, LPCTSTR pneedle)
// To make this work with MS Visual C++, this version uses tolower/toupper() in place of
// _tolower/_toupper(), since apparently in GNU C, the underscore macros are identical
// to the non-underscore versions; but in MS the underscore ones do an unconditional
// conversion (mangling non-alphabetic characters such as the zero terminator).  MSDN:
// tolower: Converts c to lowercase if appropriate
// _tolower: Converts c to lowercase

// Return the offset of one string within another.
// Copyright (C) 1994,1996,1997,1998,1999,2000 Free Software Foundation, Inc.
// This file is part of the GNU C Library.

// The GNU C Library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// The GNU C Library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with the GNU C Library; if not, write to the Free
// Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
// 02111-1307 USA.

// My personal strstr() implementation that beats most other algorithms.
// Until someone tells me otherwise, I assume that this is the
// fastest implementation of strstr() in C.
// I deliberately chose not to comment it.  You should have at least
// as much fun trying to understand it, as I had to write it :-).
// Stephen R. van den Berg, berg@pool.informatik.rwth-aachen.de

// Faster looping by precalculating bl, bu, cl, cu before looping.
// 2004 Apr 08  Jose Da Silva, digital@joescat@com
{
    register const TBYTE *haystack, *needle;
    register unsigned bl, bu, cl, cu;
    haystack = (const TBYTE *) phaystack;
    needle = (const TBYTE *) pneedle;
    // Since ctolower returns TCHAR (which is signed in ANSI builds), typecast to
    // TBYTE first to promote characters \x80-\xFF to unsigned 32-bit correctly:
    bl = (TBYTE)ctolower(*needle);

    if (bl != '\0') {
        // Scan haystack until the first character of needle is found:
        bu = (TBYTE)ctoupper(bl);
        haystack--;             /* possible ANSI violation */

        do {
            cl = *++haystack;

            if (cl == '\0')
                goto ret0;
        } while ((cl != bl) && (cl != bu));

        // See if the rest of needle is a one-for-one match with this part of haystack:
        cl = (TBYTE)ctolower(*++needle);

        if (cl == '\0')  // Since needle consists of only one character, it is already a match as found above.
            goto foundneedle;

        cu = (TBYTE)ctoupper(cl);
        ++needle;
        goto jin;

        for (;;) {
            register unsigned a;
            register const TBYTE *rhaystack, *rneedle;

            do {
                a = *++haystack;

                if (a == '\0')
                    goto ret0;

                if ((a == bl) || (a == bu))
                    break;

                a = *++haystack;

                if (a == '\0')
                    goto ret0;

shloop:
                ;
            } while ((a != bl) && (a != bu));

jin:
            a = *++haystack;

            if (a == '\0')  // Remaining part of haystack is shorter than needle.  No match.
                goto ret0;

            if ((a != cl) && (a != cu)) // This promising candidate is not a complete match.
                goto shloop;            // Start looking for another match on the first char of needle.

            rhaystack = haystack-- + 1;
            rneedle = needle;
            a = (TBYTE)ctolower(*rneedle);

            if ((TBYTE)ctolower(*rhaystack) == (int) a)
                do {
                    if (a == '\0')
                        goto foundneedle;

                    ++rhaystack;
                    a = (TBYTE)ctolower(*++needle);

                    if ((TBYTE)ctolower(*rhaystack) != (int) a)
                        break;

                    if (a == '\0')
                        goto foundneedle;

                    ++rhaystack;
                    a = (TBYTE)ctolower(*++needle);
                } while ((TBYTE)ctolower(*rhaystack) == (int) a);

            needle = rneedle;       /* took the register-poor approach */

            if (a == '\0')
                break;
        } // for(;;)
    } // if (bl != '\0')

foundneedle:
    return (LPTSTR) haystack;
ret0:
    return 0;
}




UINT StrReplace(LPTSTR aHaystack, LPTSTR aOld, LPTSTR aNew, StringCaseSenseType aStringCaseSense
                , UINT aLimit, size_t aSizeLimit, LPTSTR *aDest, size_t *aHaystackLength)
// Replaces all (or aLimit) occurrences of aOld with aNew in aHaystack.
// On success, it returns the number of replacements done (0 if none).  On failure (out of memory), it returns 0
// (and if aDest isn't NULL, it also sets *aDest to NULL on failure).
//
// PARAMETERS:
// - aLimit: Specify UINT_MAX to have no restriction on the number of replacements.  Otherwise, specify a number >=0.
// - aSizeLimit: Specify -1 to assume that aHaystack has enough capacity for any mode #1 replacement. Otherwise,
//   specify the size limit (in either mode 1 or 2), but it must be >= length of aHaystack (simplifies the code).
// - aDest: If NULL, the function will operate in mode #1.  Otherwise, it uses mode #2 (see further below).
// - aHaystackLength: If it isn't NULL, *aHaystackLength must be the length of aHaystack.  HOWEVER, *aHaystackLength
//   is then changed here to be the length of the result string so that caller can use it to improve performance.
//
// MODE 1 (when aDest==NULL): aHaystack is used as both the source and the destination (sometimes temporary memory
// is used for performance, but it's freed afterward and so transparent to the caller).
// When it passes in -1 for aSizeLimit (the default), caller must ensure that aHaystack has enough capacity to hold
// the new/replaced result.  When non-NULL, aSizeLimit will be enforced by limiting the number of replacements to
// the available memory (i.e. any remaining replacements are simply not done and that part of haystack is unaltered).
//
// MODE 2 (when aDest!=NULL): If zero replacements are needed, we set *aDest to be aHaystack to indicate that no
// new memory was allocated.  Otherwise, we store in *aDest the address of the new memory that holds the result.
// - The caller is responsible for any new memory in *aDest (freeing it, etc.)
// - The initial value of *aDest doesn't matter.
// - The contents of aHaystack isn't altered, not even if aOld_length==aNew_length (some callers rely on this).
//
// v1.0.45: This function was heavily revised to improve performance and flexibility.  It has also made
// two other/related StrReplace() functions obsolete.  Also, the code has been simplified to avoid doing
// a first pass through haystack to find out exactly how many replacements there are because that step
// nearly doubles the time required for the entire operation (in most cases).  Its benefit was mainly in
// memory savings and avoidance of any reallocs since the initial alloc was always exactly right; however,
// testing shows that one or two reallocs are generally much quicker than doing the size-calculation phase
// because extra alloc'ing & memcpy'ing is much faster than an extra search through haystack for all the matches.
// Furthermore, the new approach minimizes reallocs by using smart prediction.  Furthermore, the caller shrinks
// the result memory via _expand() to avoid having too much extra/overhang.  These optimizations seem to make
// the new approach better than the old one in every way, but especially performance.
{
#define REPLACEMENT_MODE2 aDest  // For readability.
    // THINGS TO SET NOW IN CASE OF EARLY RETURN OR GOTO:
    // Set up the input/output lengths:
    size_t haystack_length = aHaystackLength ? *aHaystackLength : _tcslen(aHaystack); // For performance, use caller's length if it was provided.
    size_t length_temp; // Just a placeholder/memory location used by the alias below.
    size_t &result_length = aHaystackLength ? *aHaystackLength : length_temp; // Make an alias for convenience and maintainability (if this is an output parameter for our caller, this step takes care that in advance).
    // Set up the output buffer:
    LPTSTR result_temp; // In mode #1, holds temporary memory that is freed before we return.
    LPTSTR &result = aDest ? *aDest : result_temp; // Make an alias for convenience and maintainability (if aDest is non-NULL, it's an output parameter for our caller, and this step takes care that in advance).
    result = NULL;     // It's allocated only upon first use to avoid a potentially massive allocation that might
    result_length = 0; // be wasted and cause swapping (not to mention that we'll have better ability to estimate the correct total size after the first replacement is discovered).
    size_t result_size = 0;
    // Variables used by both replacement methods.
    LPTSTR src, match_pos;
    // END OF INITIAL SETUP.

    // From now on, result_length and result should be kept up-to-date because they may have been set up
    // as output parameters above.

    if (!(*aHaystack && *aOld)) {
        // Nothing to do if aHaystack is blank. If aOld is blank, that is not supported because it would be an
        // infinite loop. This policy is now largely due to backward compatibility because some other policy
        // may have been better.
        result = aHaystack; // Return unaltered string to caller in its output parameter (result is an alias for *aDest).
        result_length = haystack_length; // This is an alias for an output parameter, so update it for caller.
        return 0; // Report "no replacements".
    }

    size_t aOld_length = _tcslen(aOld);
    size_t aNew_length = _tcslen(aNew);
    int length_delta = (int)(aNew_length - aOld_length); // Cast to int to avoid loss of unsigned. A negative delta means the replacement substring is smaller than what it's replacing.

    if (aSizeLimit != -1) { // Caller provided a size *restriction*, so if necessary reduce aLimit to stay within bounds.  Compare directly to -1 due to unsigned.
        int extra_room = (int)(aSizeLimit - 1 - haystack_length); // Cast to int to preserve negatives.

        if (extra_room < 0) // Caller isn't supposed to call it this way.  To avoid having to complicate the
            aLimit = 0;     // calculations in the else-if below, allow no replacements in this case.
        else if (length_delta > 0) { // New-str is bigger than old-str.  This is checked to avoid going negative or dividing by 0 below. A positive delta means length of new/replacement string is greater than that of what it's replacing.
            UINT upper_limit = (UINT)(extra_room / length_delta);

            if (aLimit > upper_limit)
                aLimit = upper_limit;
        }

        //else length_delta <= 0, so there no overflow should be possible.  Leave aLimit as-is.
    }

    if (!REPLACEMENT_MODE2) { // Mode #1
        if (!length_delta // old_len==new_len, so use in-place method because it's just as fast in this case but it avoids the extra memory allocation.
            || haystack_length < 5000) // ...or the in-place method will likely be faster, and an earlier stage has ensured there's no risk of overflow.
            goto in_place_method; // "Goto" to avoid annoying indentation and long IF-blocks.

        //else continue on because the extra-memory method will usually perform better than the in-place method.
        // The extra-memory method is much faster than the in-place method when many replacements are needed because
        // it avoids a memmove() to shift the remainder of the buffer up against the area of memory that
        // will be replaced (which for the in-place method is done for every replacement).  The savings
        // can be enormous if aSource is very large, assuming the system can allocate the memory without swapping.
    }

    // Otherwise:
    // Since above didn't jump to the in place method, either the extra-memory method is preferred or this is mode #2.
    // Never use the in-place method for mode #2 because caller always wants a separate memory area used (for its
    // purposes, the extra-memory method is probably just as fast or faster than in-place method).
    // Below uses a temp var. because realloc() returns NULL on failure but leaves original block allocated.
    // Note that if it's given a NULL pointer, realloc() does a malloc() instead.
    LPTSTR realloc_temp;
#define STRREPLACE_REALLOC(size) \
    {\
        result_size = size;\
        if (   !(realloc_temp = trealloc(result, result_size))   )\
            goto out_of_mem;\
        result = realloc_temp;\
    }
    // Other variables used by the replacement loop:
    size_t haystack_portion_length, new_result_length;
    UINT replacement_count;

    // Perform the replacement:
    for (replacement_count = 0, src = aHaystack
                                      ; aLimit && (match_pos = tcsstr2(src, aOld, aStringCaseSense));) { // Relies on short-circuit boolean order.
        ++replacement_count;
        --aLimit;
        haystack_portion_length = match_pos - src; // The length of the haystack section between the end of the previous match and the start of the current one.
        // Using the required length calculated below, expand/realloc "result" if necessary.
        new_result_length = result_length + haystack_portion_length + aNew_length;

        if (new_result_length >= result_size) // Uses >= to allow room for terminator.
            STRREPLACE_REALLOC(PredictReplacementSize(length_delta, replacement_count, aLimit, (int)haystack_length
                               , (int)new_result_length, (int)(match_pos - aHaystack))); // This will return if an alloc error occurs.

        // Now that we know "result" has enough capacity, put the new text into it.  The first step
        // is to copy over the part of haystack that appears before the match.
        if (haystack_portion_length) {
            tmemcpy(result + result_length, src, haystack_portion_length);
            result_length += haystack_portion_length;
        }

        // Now append the replacement string in place of the old string.
        if (aNew_length) {
            tmemcpy(result + result_length, aNew, aNew_length);
            result_length += aNew_length;
        }

        //else omit it altogether; i.e. replace every aOld with the empty string.
        // Set up src to be the position where the next iteration will start searching.  For consistency with
        // the in-place method, overlapping matches are not detected.  For example, the replacement
        // of all occurrences of ".." with ". ." in "..." would produce ". ..", not ". . .":
        src = match_pos + aOld_length; // This has two purposes: 1) Since match_pos is about to be altered by strstr, src serves as a placeholder for use by the next iteration; 2) it's also used further below.
    }

    if (!replacement_count) { // No replacements were done, so optimize by keeping the original (avoids a malloc+memcpy).
        // The following steps are appropriate for both mode #1 and #2 (for simplicity and maintainability,
        // they're all done unconditionally even though mode #1 might not require them all).
        result = aHaystack; // Return unaltered string to caller in its output parameter (result is an alias for *aDest).
        result_length = haystack_length; // This is an alias for an output parameter, so update it for caller.
        return replacement_count;
        // Since no memory was allocated, there's never anything to free.
    }

    // (Below relies only above having returned when no replacements because it assumes result!=NULL from now on.)

    // Otherwise, copy the remaining characters after the last replacement (if any) (fixed for v1.0.25.11).
    if (haystack_portion_length = haystack_length - (src - aHaystack)) { // This is the remaining part of haystack that need to be copied over as-is.
        new_result_length = result_length + haystack_portion_length;

        if (new_result_length >= result_size) // Uses >= to allow room for terminator.
            STRREPLACE_REALLOC(new_result_length + 1); // This will return if an alloc error occurs.

        tmemcpy(result + result_length, src, haystack_portion_length); // memcpy() usually benches a little faster than strcpy().
        result_length = new_result_length; // Remember that result_length is actually an output for our caller, so even if for no other reason, it must be kept accurate for that.
    }

    result[result_length] = '\0'; // Must terminate it unconditionally because other sections usually don't do it.

    if (!REPLACEMENT_MODE2) { // Mode #1.
        // Since caller didn't provide destination memory, copy the result from our temporary memory (that was used
        // for performance) back into the caller's original buf (which has already been confirmed to be large enough).
        tmemcpy(aHaystack, result, result_length + 1); // Include the zero terminator.
        free(result); // Free the temp. mem that was used for performance.
    }

    return replacement_count;  // The output parameters have already been populated properly above.
out_of_mem: // This can only happen with the extra-memory method above (which due to its nature can't fall back to the in-place method).

    if (result) {
        free(result); // Must be freed in mode #1.  In mode #2, it's probably a non-terminated string (not to mention being an incomplete result), so if it ever isn't freed, it should be terminated.
        result = NULL; // Indicate failure by setting output param for our caller (this also indicates that the memory was freed).
    }

    result_length = 0; // Output parameter for caller, though upon failure it shouldn't matter (just for robustness).
    return 0;
in_place_method:
    // This method is available only to mode #1.  It should help performance for short strings such as those from
    // ExpandExpression().
    // This in-place method is used when the extra-memory method wouldn't be faster enough to be worth its cost
    // for the particular strings involved here.
    //
    // Older comment:
    // The below doesn't quite work when doing a simple replacement such as ".." with ". .".
    // In the above example, "..." would be changed to ". .." rather than ". . ." as it should be.
    // Therefore, use a less efficient, but more accurate method instead.  UPDATE: But this method
    // can cause an infinite loop if the new string is a superset of the old string, so don't use
    // it after all.
    //for ( ; ptr = StrReplace(aHaystack, aOld, aNew, aStringCaseSense); ); // Note that this very different from the below.

    for (replacement_count = 0, src = aHaystack
                                      ; aLimit && (match_pos = tcsstr2(src, aOld, aStringCaseSense)) // Relies on short-circuit boolean order.
         ; --aLimit, ++replacement_count) {
        src = match_pos + aNew_length;  // The next search should start at this position when all is adjusted below.

        if (length_delta) { // This check can greatly improve performance if old and new strings happen to be same length.
            // Since new string can't fit exactly in place of old string, adjust the target area to
            // accept exactly the right length so that the rest of the string stays unaltered:
            tmemmove(src, match_pos + aOld_length
                     , (haystack_length - (match_pos - aHaystack)) - aOld_length + 1); // +1 to include zero terminator.
            // Above: Calculating length vs. using strlen() makes overall speed of the operation about
            // twice as fast for some typical test cases in a 2 MB buffer such as replacing \r\n with \n.
        }

        tmemcpy(match_pos, aNew, aNew_length); // Perform the replacement.
        // Must keep haystack_length updated as we go, for use with memmove() above:
        haystack_length += length_delta; // Note that length_delta will be negative if aNew is shorter than aOld.
    }

    result_length = haystack_length; // Set for caller (it's an alias for an output parameter).
    result = aHaystack; // Not actually needed in this method, so this is just for maintainability.
    return replacement_count;
}


size_t PredictReplacementSize(ptrdiff_t aLengthDelta, int aReplacementCount, int aLimit, size_t aHaystackLength
                              , size_t aCurrentLength, size_t aEndOffsetOfCurrMatch)
// Predict how much size the remainder of a replacement operation will consume, including its actual replacements
// and the parts of haystack that won't need replacement.
// PARAMETERS:
// - aLengthDelta: The estimated or actual difference between the length of the replacement and what it's replacing.
//   A negative number means the replacement is smaller, which will cause a shrinking of the result.
// - aReplacementCount: The number of replacements so far, including the one the caller is about to do.
// - aLimit: The *remaining* number of replacements *allowed* (not including the one the caller is about to do).
// - aHaystackLength: The total length of the original haystack/subject string.
// - aCurrentLength: The total length of the new/result string including the one the caller is about to do.
// - aEndOffsetOfCurrMatch: The offset of the char after the last char of the current match.  For example, if
//   the empty string is the current match and it's found at the beginning of haystack, this value would be 0.
{
    // Since realloc() is an expensive operation, especially for huge strings, make an extra
    // effort to get a good estimate based on how things have been going so far.
    // While this should definitely improve average-case memory-utilization and usually performance
    // (by avoiding costly realloc's), this estimate is crude because:
    // 1) The length of what is being replaced can vary due to wildcards in pattern, etc.
    // 2) The length of what is replacing it can vary due to backreferences.  Thus, the delta
    //    of each replacement is only a guess based on that of the current replacement.
    // 3) For code simplicity, the number of upcoming replacements isn't yet known; thus a guess
    //    is made based on how many there have been so far compared to percentage complete.
    INT_PTR total_delta; // The total increase/decrease in length from the number of predicted additional replacements.
    int repl_multiplier = aLengthDelta < 0 ? -1 : 1; // Negative is used to keep additional_replacements_expected conservative even when delta is negative.

    if (aLengthDelta == 0) // Avoid all the calculations because it will wind up being zero anyway.
        total_delta = 0;
    else {
        if (!aHaystackLength // aHaystackLength can be 0 if an empty haystack being replaced by something else. If so, avoid divide-by-zero in the prediction by doing something simpler.
            || !aEndOffsetOfCurrMatch)  // i.e. don't the prediction if the current match is the empty string and it was found at the very beginning of Haystack because it would be difficult to be accurate (very rare anyway).
            total_delta = repl_multiplier * aLengthDelta; // Due to rarity, just allow room for one extra after the one we're about to do.
        else { // So the above has ensured that the following won't divide by zero anywhere.
            // The following doesn't take into account the original value of aStartingOffset passed in
            // from the caller because:
            // 1) It's pretty rare for it to be greater than 0.
            // 2) Even if it is, the prediction will just be too conservative at first, but that's
            //    pretty harmless; and anyway each successive realloc followed by a match makes the
            //    prediction more and more accurate in spite of aStartingOffset>0.
            // percent_complete must be a double because we need at least 9 digits of precision for cases where
            // 1 is divided by a big number like 1 GB.
            double percent_complete = aEndOffsetOfCurrMatch  // Don't subtract 1 (verified correct).
                                      / (double)aHaystackLength; // percent_complete isn't actually a percentage, but a fraction of 1.  e.g. 0.5 rather than 50.
            int additional_replacements_expected = percent_complete >= 1.0 ? 0  // It's often 100% complete, in which case there's hardly ever another replacement after this one (the only possibility is to replace the final empty-string in haystack with something).
                                                   : (int)(
                                                           (aReplacementCount / percent_complete) // This is basically "replacements per percentage point, so far".
                                                           * (1 - percent_complete) // This is the percentage of haystack remaining to be scanned (e.g. 0.5 for 50%).
                                                           + 1 * repl_multiplier // Add 1 or -1 to make it more conservative (i.e. go the opposite direction of ceil when repl_multiplier is negative).
                                                   );
            // additional_replacements_expected is defined as the replacements expected *after* the one the caller
            // is about to do.

            if (aLimit >= 0 && aLimit < additional_replacements_expected) {
                // A limit is currently in effect and it's less than expected replacements, so cap the expected.
                // This helps reduce memory utilization.
                additional_replacements_expected = aLimit;
            } else { // No limit or additional_replacements_expected is within the limit.
                // So now things are set up so that there's about a 50/50 chance than no more reallocs
                // will be needed.  Since realloc is costly (due to internal memcpy), try to reduce
                // the odds of it happening without going overboard on memory utilization.
                // Something a lot more complicated could be used in place of the below to improve things
                // a little, but it just doesn't seem worth it given the usage patterns expected and
                // the actual benefits.  Besides, there is some limiting logic further below that will
                // cap this if it's unreasonably large:
                additional_replacements_expected += (int)(0.20 * additional_replacements_expected + 1) // +1 so that there's always at least one extra.
                                                    * repl_multiplier; // This keeps the estimate conservative if delta < 0.
            }

            // The following is the "quality" of the estimate.  For example, if this is the very first replacement
            // and 1000 more replacements are predicted, there will often be far fewer than 1000 replacements;
            // in fact, there could well be zero.  So in the below, the quality will range from 1 to 3, where
            // 1 is the worst quality and 3 is the best.
            double quality = 1 + 2 * (1 - (
                                          (double)additional_replacements_expected / (aReplacementCount + additional_replacements_expected)
                                      ));
            // It seems best to use whichever of the following is greater in the calculation further below:
            INT_PTR haystack_or_new_length = (aCurrentLength > aHaystackLength) ? aCurrentLength : aHaystackLength;
            // The following is a crude sanity limit to avoid going overboard with memory
            // utilization in extreme cases such as when a big string has many replacements
            // in its first half, but hardly any in its second.  It does the following:
            // 1) When Haystack-or-current length is huge, this tries to keep the portion of the memory increase
            //    that's speculative proportionate to that length, which should reduce the chance of swapping
            //    (at the expense of some performance in cases where it causes another realloc to be required).
            // 2) When Haystack-or-current length is relatively small, allow the speculative memory allocation
            //    to be many times larger than that length because the risk of swapping is low.  HOWEVER, TO
            //    AVOID WASTING MEMORY, the caller should probably call _expand() to shrink the result
            //    when it detects that far fewer replacements were needed than predicted (this is currently
            //    done by Var::AcceptNewMem()).
            INT_PTR total_delta_limit = (INT_PTR)(haystack_or_new_length < 10 * 1024 * 1024 ? quality * 10 * 1024 * 1024
                                                  : quality * haystack_or_new_length); // See comment above.
            total_delta = additional_replacements_expected
                          * (aLengthDelta < 0 ? -aLengthDelta : aLengthDelta); // So actually, total_delta will be the absolute value.

            if (total_delta > total_delta_limit)
                total_delta = total_delta_limit;

            total_delta *= repl_multiplier;  // Convert back from absolute value.
        } // The current match isn't an empty string at the very beginning of haystack.
    } // aLengthDelta!=0

    // Above is responsible for having set total_delta properly.
    INT_PTR subsequent_length = aHaystackLength - aEndOffsetOfCurrMatch // This is the length of the remaining portion of haystack that might wind up going into the result exactly as-is (adjusted by the below).
                                + total_delta; // This is additional_replacements_expected times the expected delta (the length of each replacement minus what it replaces) [can be negative].

    if (subsequent_length < 0) // Must not go below zero because that would cause the next line to
        subsequent_length = 0; // create an increase that's too small to handle the current replacement.

    // Return the sum of the following:
    // 1) subsequent_length: The predicted length needed for the remainder of the operation.
    // 2) aCurrentLength: The amount we need now, which includes room for the replacement the caller is about to do.
    //    Note that aCurrentLength can be 0 (such as for an empty string replacement).
    return subsequent_length + aCurrentLength + 1; // Caller relies on +1 for the terminator.
}

void BSTR2CString(CString* pStr, BSTR bstr)
{
    ATLASSERT(pStr != NULL);

    if (pStr == NULL) {
        return;
    }

    int nLen = SysStringLen(bstr);
#if defined(_UNICODE)
    LPTSTR lpsz = pStr->GetBufferSetLength(nLen);
    ATLASSERT(lpsz != NULL);
    Checked::memcpy_s(lpsz, nLen * sizeof(TCHAR), bstr, nLen * sizeof(TCHAR));
    pStr->ReleaseBuffer(nLen);
#else
    int nBytes = WideCharToMultiByte(CP_ACP, 0, bstr, nLen, NULL, NULL, NULL,
                                     NULL);
    LPSTR lpsz = pStr->GetBufferSetLength(nBytes);
    ASSERT(lpsz != NULL);
    WideCharToMultiByte(CP_ACP, 0, bstr, nLen, lpsz, nBytes, NULL, NULL);
    pStr->ReleaseBuffer(nBytes);
#endif
}


CString ToString(LPCTSTR fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    CString s;
    s.FormatV(fmt, args);
    va_end(args);
    return s;
}

CString ToString(UINT fmtid, ...)
{
    va_list args;
    va_start(args, fmtid);
    CString fmt;
    fmt.LoadString(fmtid);
    CString s;
    s.FormatV(fmt, args);
    va_end(args);
    return s;
}

string16 format_string(const char16* fmt, ...)
{
    std::auto_ptr<char16> buf;
    int size = 512;

    while (true) {
        buf = std::auto_ptr<char16>(new char16[size <<= 1]);
        va_list ap;
        va_start(ap, fmt);
        int written = _vsnwprintf(buf.get(), size, fmt, ap);
        va_end(ap);

        if (written == size) {
            if (buf.get()[size] == 0)
                break;
        } else if (written >= 0 && written < size) {
            assert(buf.get()[written] == 0);
            break;
        }

        // else continue growing the buffer...
    }

    return string16(buf.get());
}

string8 format_string(const char* fmt, ...)
{
    std::auto_ptr<char> buf;
    int size = 512;

    while (true) {
        buf = std::auto_ptr<char>(new char[size <<= 1]);
        va_list ap;
        va_start(ap, fmt);
        int written = _vsnprintf(buf.get(), size, fmt, ap);
        va_end(ap);

        if (written == size) {
            if (buf.get()[size] == 0)
                break;
        } else if (written >= 0 && written < size) {
            assert(buf.get()[written] == 0);
            break;
        }

        // else continue growing the buffer...
    }

    return string8(buf.get());
}

int vsnprintf(char* buffer, size_t size, const char* format, va_list arguments)
{
    int length = vsnprintf_s(buffer, size, size - 1, format, arguments);

    if (length < 0)
        return _vscprintf(format, arguments);

    return length;
}

int vswprintf(char16* buffer, size_t size, const char16* format, va_list arguments)
{
    int length = _vsnwprintf_s(buffer, size, size - 1, format, arguments);

    if (length < 0)
        return _vscwprintf(format, arguments);

    return length;
}

CString Format(double dVal, int nDecPlaces)
{
    char* szLocale = _strdup(setlocale(LC_NUMERIC, NULL)); // current locale
    setlocale(LC_NUMERIC, ""); // local default
    CString sValue;

    if (nDecPlaces < 0)
        sValue.Format(_T("%f"), dVal);
    else
        sValue.Format(_T("%.*f"), nDecPlaces, dVal);

    // restore locale
    setlocale(LC_NUMERIC, szLocale);
    free(szLocale);
    return sValue;
}

CString Format(int nVal)
{
    CString sValue;
    sValue.Format(_T("%ld"), nVal);
    return sValue;
}

CString FormatCost(double dCost)
{
    CString sValue;
    sValue.Format(_T("%.6f"), dCost);
    char* szLocale = _strdup(setlocale(LC_NUMERIC, NULL)); // current locale
    setlocale(LC_NUMERIC, ""); // local default
    const UINT BUFSIZE = 100;
    TCHAR szCost[BUFSIZE + 1];
    GetCurrencyFormat(NULL, 0, sValue, NULL, szCost, BUFSIZE);
    sValue = szCost;
    // restore locale
    setlocale(LC_NUMERIC, szLocale);
    free(szLocale);
    return sValue;
}

string16 FormatMessage(unsigned messageid)
{
    char16* string_buffer = NULL;
    unsigned string_length = ::FormatMessage(
                                 FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                 FORMAT_MESSAGE_IGNORE_INSERTS, NULL, messageid, 0,
                                 reinterpret_cast<char16 *>(&string_buffer), 0, NULL);
    string16 formatted_string;

    if (string_buffer) {
        formatted_string = string_buffer;
        LocalFree(reinterpret_cast<HLOCAL>(string_buffer));
    } else {
        // The formating failed. simply convert the message value into a string.
        char16 _buffer[MAX_PATH];
        swprintf_s(_buffer, MAX_PATH, L"message number %d", messageid);
        formatted_string = string16(_buffer);
    }

    return formatted_string;
}

string16 FormatLastWin32Error()
{
    return FormatMessage(GetLastError());
}

string8 HexEncode(const void* bytes, size_t size)
{
    static const char kHexChars[] = "0123456789ABCDEF";
    // Each input byte creates two output hex characters.
    string8 ret(size * 2, '\0');

    for (size_t i = 0; i < size; ++i) {
        char b = reinterpret_cast<const char*>(bytes)[i];
        ret[(i * 2)] = kHexChars[(b >> 4) & 0xf];
        ret[(i * 2) + 1] = kHexChars[b & 0xf];
    }

    return ret;
}

string8 toOctalString(uint n)
{
    string8 s;
    s.resize(32);
    string8::size_type charPos = 32;
    const int radix = 1 << 3;
    int mask = radix - 1;

    do {
        s[--charPos] = '0' + (n & mask);
        n >>= 3;
    } while (n != 0);

    return string8(s, charPos, (32 - charPos));
}

string16 formatBytesW(int64_t aBytes)
{
    char16 buf[64];

    if (aBytes < 1024) {
        _snwprintf(buf, _countof(buf), L"%d %s", (int)(aBytes & 0xffffffff), L"B");
    } else if (aBytes < 1048576) {
        _snwprintf(buf, _countof(buf), L"%.02f %s", (double)aBytes / (1024.0), L"KB");
    } else if (aBytes < 1073741824) {
        _snwprintf(buf, _countof(buf), L"%.02f %s", (double)aBytes / (1048576.0), L"MB");
    } else if (aBytes < (int64_t)1099511627776) {
        _snwprintf(buf, _countof(buf), L"%.02f %s", (double)aBytes / (1073741824.0), L"GB");
    } else if (aBytes < (int64_t)1125899906842624) {
        _snwprintf(buf, _countof(buf), L"%.03f %s", (double)aBytes / (1099511627776.0), L"TB");
    } else if (aBytes < (int64_t)1152921504606846976) {
        _snwprintf(buf, _countof(buf), L"%.03f %s", (double)aBytes / (1125899906842624.0), L"PB");
    } else {
        _snwprintf(buf, _countof(buf), L"%.03f %s", (double)aBytes / (1152921504606846976.0), L"EB"); //TODO Crash beta-16
    }

    return buf;
}

CString GetSizeStr(__int64 nSize)
{
    CString retval;
    float fSize;

    if (nSize > 1024 * 1024 * 1024) {
        fSize = (float)nSize / (1024 * 1024 * 1024);
        retval.Format(L"%1.1f GB", fSize);
    } else if (nSize > 1024 * 1024) {
        fSize = (float)nSize / (1024 * 1024);
        retval.Format(L"%1.1f MB", fSize);
    } else if (nSize > 1024) {
        fSize = (float)nSize / 1024;
        retval.Format(L"%1.1f KB", fSize);
    } else {
        retval.Format(L"%d B", nSize);
    }

    return retval;
}

CString CalcStrFormatByteSize(LONGLONG qdw, BOOL bLimit /*= FALSE*/)
{
    CString retStr;

    if (qdw > 1024 * 1024 * 1024 && bLimit) {
        retStr = L"大于1G";
    } else {
        StrFormatByteSize64(qdw, retStr.GetBuffer(64), 64);
        retStr.ReleaseBuffer();
    }

    return retStr;
}

string16 URLEncode2(const string16& url)
{
    string16 retval;
    std::vector<char16> buffer(1024, 0);
    DWORD dwLength = 0;
    AtlEscapeUrl(url.c_str(), &buffer[0], &dwLength, 1024);

    if (&buffer[0] != NULL)
        retval = string16(&buffer[0]);
    else
        retval = StringUtil::sWEmpty;

    return retval;
}

char OctetToHexChar(char c)
{
    unsigned char nibble = 0x0F & c;
    return (nibble < 0x0A) ? '0' + nibble : 'A' - 0x0A + nibble ;
}

string8 EscapeChar(char c)
{
    string8 escaped = "%  ";
    escaped[2] = OctetToHexChar(c);
    escaped[1] = OctetToHexChar(c >> 4);
    return escaped;
}

string8 EscapeUrl(const string8& s)
{
    string8 escaped;

    for (size_t i = 0; i < s.size(); ++i) {
        char ch = s[i];

        if (!strchr("%\"<>\\^[]`+$,@;!#'", ch))
            escaped.append(1, ch);
        else
            escaped += EscapeChar(ch);
    }

    return escaped;
}

string8 char2hex(char dec)
{
    char dig1 = (dec & 0xF0) >> 4;
    char dig2 = (dec & 0x0F);

    if (0 <= dig1 && dig1 <= 9) dig1 += 48; //0,48 in ascii

    if (10 <= dig1 && dig1 <= 15) dig1 += 65 - 10; //A,65 in ascii

    if (0 <= dig2 && dig2 <= 9) dig2 += 48;

    if (10 <= dig2 && dig2 <= 15) dig2 += 65 - 10;

    string8 r;
    r.append(&dig1, 1);
    r.append(&dig2, 1);
    return r;
}

string8 urlencode(const string8 &c)
{
    string8 escaped;
    int max = c.length();

    for (int i = 0; i < max; i++) {
        if ((48 <= c[i] && c[i] <= 57) || //0-9
            (65 <= c[i] && c[i] <= 90) ||//ABC...XYZ
            (97 <= c[i] && c[i] <= 122) || //abc...xyz
            (c[i] == '~' || c[i] == '-' || c[i] == '_' || c[i] == '.')
           ) {
            escaped.append(&c[i], 1);
        } else {
            escaped.append("%");
            escaped.append(char2hex(c[i]));  //converts char 255 to string "FF"
        }
    }

    return escaped;
}


BOOL IsGuid(LPCTSTR szGuid)
{
    GUID guid;
    return GuidFromString(szGuid, guid);
}

BOOL SameGuids(const GUID &guid1, const GUID &guid2)
{
    return (memcmp(& guid1, & guid2, sizeof(GUID)) == 0);
}

BOOL IsBeginGuiD(LPCWSTR lpStr)
{
    const size_t nLen = wcslen(lpStr);
    return (nLen >= 38) && (lpStr[0] == L'{') && (lpStr[37] == L'}');
}

BOOL GuidIsNull(const GUID &guid)
{
    static GUID NULLGUID = {0};
    return SameGuids(guid, NULLGUID);
}

BOOL GuidFromString(const string16& data, GUID &guid)
{
    RPC_STATUS rpcs = UuidFromString((RPC_WSTR)data.c_str(), &guid);

    if (rpcs != RPC_S_OK) {
        ZeroMemory(&guid, sizeof(guid));
        return FALSE;
    }

    return TRUE;
}

BOOL GuidFromString(LPCTSTR szGuid, GUID &guid)
{
#pragma comment(lib, "Rpcrt4.lib")
    RPC_STATUS rpcs = UuidFromString((RPC_WSTR)szGuid, & guid);

    if (rpcs != RPC_S_OK) {
        ZeroMemory(&guid, sizeof(guid));
        return FALSE;
    }

    return TRUE;
}

BOOL GuidToString(const GUID &guid, CString &sGuid)
{
#pragma comment(lib, "Rpcrt4.lib")
    RPC_WSTR pszGuid;

    if (RPC_S_OK == UuidToString((UUID *)&guid, & pszGuid))
        sGuid = (LPCWSTR)pszGuid;
    else
        sGuid.Empty();

    RpcStringFree(&pszGuid);
    return !sGuid.IsEmpty();
}

string16 GuidToString(const GUID &guid)
{
    string16 data;
    RPC_WSTR pszGuid;

    if (RPC_S_OK == UuidToString((UUID *)&guid, & pszGuid))
        data = (LPCWSTR)pszGuid;
    else
        data = StringUtil::sWEmpty;

    RpcStringFree(&pszGuid);
    return data;
}

int StringFromGUID2(REFGUID rguid, LPOLESTR lptsz, int cchMax)
{
    if (cchMax < 39)
        return 0;

    return swprintf(lptsz, W("{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}"),
        rguid.Data1, rguid.Data2, rguid.Data3,
        rguid.Data4[0], rguid.Data4[1],
        rguid.Data4[2], rguid.Data4[3],
        rguid.Data4[4], rguid.Data4[5],
        rguid.Data4[6], rguid.Data4[7]) + 1;
}

//+-------------------------------------------------------------------------
//
//  Function:   IIDFromString
//
//  Synopsis:   converts string {...} form int guid
//
//  Arguments:  [lpsz] - ptr to buffer for results
//              [lpclsid] - the guid to convert
//
//  Returns:    NOERROR
//              CO_E_CLASSSTRING
//
//--------------------------------------------------------------------------
STDAPI IIDFromString(LPWSTR lpsz, CLSID * lpclsid)
{
    if (lpsz == NULL)
    {
        *lpclsid = CLSID_NULL;
        return NOERROR;
    }

    if (*lpsz == 0)
    {
        return(CO_E_CLASSSTRING);
    }

    return wGUIDFromString(lpsz,lpclsid)
        ? NOERROR : CO_E_CLASSSTRING;
}

//+-------------------------------------------------------------------------
//
//  Function:   wGUIDFromString    (internal)
//
//  Synopsis:   Parse GUID such as {00000000-0000-0000-0000-000000000000}
//
//  Arguments:  [lpsz]  - the guid string to convert
//              [pguid] - guid to return
//
//  Returns:    TRUE if successful
//
//--------------------------------------------------------------------------
static BOOL wGUIDFromString(LPCWSTR lpsz, GUID * pguid)
{
    if (*lpsz++ != '{' )
        return FALSE;

    if (wUUIDFromString(lpsz, pguid) != TRUE)
        return FALSE;

    lpsz +=36;

    if (*lpsz++ != '}' )
        return FALSE;

    if (*lpsz != '\0')
        return FALSE;

    return TRUE;
}

//+-------------------------------------------------------------------------
//
//  Function:   wUUIDFromString    (internal)
//
//  Synopsis:   Parse UUID such as 00000000-0000-0000-0000-000000000000
//
//  Arguments:  [lpsz]  - Supplies the UUID string to convert
//              [pguid] - Returns the GUID.
//
//  Returns:    TRUE if successful
//
//--------------------------------------------------------------------------
static BOOL wUUIDFromString(LPCWSTR lpsz, GUID * pguid)
{
    DWORD dw;

    if (!HexStringToDword(lpsz, pguid->Data1, sizeof(DWORD)*2, '-'))
        return FALSE;

    if (!HexStringToDword(lpsz, dw, sizeof(WORD)*2, '-'))
        return FALSE;
    pguid->Data2 = (WORD)dw;

    if (!HexStringToDword(lpsz, dw, sizeof(WORD)*2, '-'))
        return FALSE;
    pguid->Data3 = (WORD)dw;

    if (!HexStringToDword(lpsz, dw, sizeof(BYTE)*2, 0))
        return FALSE;
    pguid->Data4[0] = (BYTE)dw;

    if (!HexStringToDword(lpsz, dw, sizeof(BYTE)*2, '-'))
        return FALSE;
    pguid->Data4[1] = (BYTE)dw;

    if (!HexStringToDword(lpsz, dw, sizeof(BYTE)*2, 0))
        return FALSE;
    pguid->Data4[2] = (BYTE)dw;

    if (!HexStringToDword(lpsz, dw, sizeof(BYTE)*2, 0))
        return FALSE;
    pguid->Data4[3] = (BYTE)dw;

    if (!HexStringToDword(lpsz, dw, sizeof(BYTE)*2, 0))
        return FALSE;
    pguid->Data4[4] = (BYTE)dw;

    if (!HexStringToDword(lpsz, dw, sizeof(BYTE)*2, 0))
        return FALSE;
    pguid->Data4[5] = (BYTE)dw;

    if (!HexStringToDword(lpsz, dw, sizeof(BYTE)*2, 0))
        return FALSE;
    pguid->Data4[6] = (BYTE)dw;

    if (!HexStringToDword(lpsz, dw, sizeof(BYTE)*2, 0))
        return FALSE;
    pguid->Data4[7] = (BYTE)dw;

    return TRUE;
}

//+-------------------------------------------------------------------------
//
//  Function:   HexStringToDword   (private)
//
//  Synopsis:   scan lpsz for a number of hex digits (at most 8); update lpsz
//              return value in Value; check for chDelim;
//
//  Arguments:  [lpsz]    - the hex string to convert
//              [Value]   - the returned value
//              [cDigits] - count of digits
//
//  Returns:    TRUE for success
//
//--------------------------------------------------------------------------
static BOOL HexStringToDword(LPCWSTR FAR& lpsz, DWORD FAR& Value,
                             int cDigits, WCHAR chDelim)
{
    int Count;

    Value = 0;
    for (Count = 0; Count < cDigits; Count++, lpsz++)
    {
        if (*lpsz >= '0' && *lpsz <= '9')
            Value = (Value << 4) + *lpsz - '0';
        else if (*lpsz >= 'A' && *lpsz <= 'F')
            Value = (Value << 4) + *lpsz - 'A' + 10;
        else if (*lpsz >= 'a' && *lpsz <= 'f')
            Value = (Value << 4) + *lpsz - 'a' + 10;
        else
            return(FALSE);
    }

    if (chDelim != 0)
        return *lpsz++ == chDelim;
    else
        return TRUE;
}



CStringA BIN2HEX(CStringA sBIN_INPUT)
{
    INT iI;
    INT intLen;
    INT NumBlocks;
    CStringA chkbin;
    CStringA FullBin;
    CStringA TempBin;
    CStringA TempHex;
    CStringA sHEX_OUTPUT;
    bool bBIN;
    sHEX_OUTPUT.Empty(); //将输出字符串清空
    intLen = sBIN_INPUT.GetLength(); //获取字符串长度

    for (iI = 0; iI < intLen; iI++) { //查表法判断是否是二进制
        if (sBIN_INPUT[iI] != '1' && sBIN_INPUT[iI] != '0') {
            bBIN = FALSE;
            throw Exception("输入非二进制！");
            break;
        } else {
            bBIN = TRUE;
        }
    }

    if (bBIN) {    //如果是二进制，进行后续处理
        //如果长度小于4
        if (intLen < 4) {
            FullBin = sBIN_INPUT;

            if (intLen == 3) {
                FullBin = "0" + FullBin;
            }

            if (intLen == 2) {
                FullBin = "00" + FullBin;
            }

            if (intLen == 1) {
                FullBin = "000" + FullBin;
            }

            if (intLen == 0) {
                //AfxMessageBox(_T("没有输入"));  05.30.13 Changed : 取消BIN无输入的提示
            }

            NumBlocks = 1;
        }

        //如果长度等于4
        if (intLen == 4) {
            FullBin = sBIN_INPUT;
            NumBlocks = 1;
        }

        //如果长度大于4
        if (intLen > 4) {
            FullBin = sBIN_INPUT;
            float TempDiv;
            float templen;
            CStringA stemp;
            templen = (float) sBIN_INPUT.GetLength();
            TempDiv = (templen / 4);
            stemp.Format("%f", TempDiv);  //将除的结果浮点数转换为字符串

            if (stemp.Mid(2, 2) == "00") {
                NumBlocks = (int)TempDiv;
            }         //强制 浮点转换到整型，有点牵强

            if (stemp.Mid(2, 2) == "25") {
                FullBin = "000" + FullBin;
                NumBlocks = (int)(FullBin.GetLength() / 4); //强制 浮点转换到整型，有点牵强
            }

            if (stemp.Mid(2, 2) == "50") {
                FullBin = "00" + FullBin;
                NumBlocks = (int)(FullBin.GetLength() / 4); //强制 浮点转换到整型，有点牵强
            }

            if (stemp.Mid(2, 2) == "75") {
                FullBin = "0" + FullBin;
                NumBlocks = (int)(FullBin.GetLength() / 4); //强制 浮点转换到整型，有点牵强
            }
        }

        for (iI = 0; iI < NumBlocks; iI++) { //查表法转换16进制
            TempBin = FullBin.Mid(iI * 4, 4);

            if (TempBin == "0000") {
                sHEX_OUTPUT = sHEX_OUTPUT + "0";
            }

            if (TempBin == "0001") {
                sHEX_OUTPUT = sHEX_OUTPUT + "1";
            }

            if (TempBin == "0010") {
                sHEX_OUTPUT = sHEX_OUTPUT + "2";
            }

            if (TempBin == "0011") {
                sHEX_OUTPUT = sHEX_OUTPUT + "3";
            }

            if (TempBin == "0100") {
                sHEX_OUTPUT = sHEX_OUTPUT + "4";
            }

            if (TempBin == "0101") {
                sHEX_OUTPUT = sHEX_OUTPUT + "5";
            }

            if (TempBin == "0110") {
                sHEX_OUTPUT = sHEX_OUTPUT + "6";
            }

            if (TempBin == "0111") {
                sHEX_OUTPUT = sHEX_OUTPUT + "7";
            }

            if (TempBin == "1000") {
                sHEX_OUTPUT = sHEX_OUTPUT + "8";
            }

            if (TempBin == "1001") {
                sHEX_OUTPUT = sHEX_OUTPUT + "9";
            }

            if (TempBin == "1010") {
                sHEX_OUTPUT = sHEX_OUTPUT + "A";
            }

            if (TempBin == "1011") {
                sHEX_OUTPUT = sHEX_OUTPUT + "B";
            }

            if (TempBin == "1100") {
                sHEX_OUTPUT = sHEX_OUTPUT + "C";
            }

            if (TempBin == "1101") {
                sHEX_OUTPUT = sHEX_OUTPUT + "D";
            }

            if (TempBin == "1110") {
                sHEX_OUTPUT = sHEX_OUTPUT + "E";
            }

            if (TempBin == "1111") {
                sHEX_OUTPUT = sHEX_OUTPUT + "F";
            }
        }
    }

    return sHEX_OUTPUT;
}

CStringA HEX2BIN(CStringA sHEX_INPUT)
{
    INT iI;
    INT intLen;
    CStringA sBIN_OUTPUT;
    sHEX_INPUT.MakeUpper();  //将输入转换为大写
    intLen = sHEX_INPUT.GetLength(); //获取字符串长度
    sBIN_OUTPUT.Empty(); //将输出字符串清空

    for (iI = 0; iI < intLen; iI++) { //查表法组合为二进制
        if (sHEX_INPUT[iI] == '0')
            sBIN_OUTPUT = sBIN_OUTPUT + "0000";
        else if (sHEX_INPUT[iI] == '1')
            sBIN_OUTPUT = sBIN_OUTPUT + "0001";
        else if (sHEX_INPUT[iI] == '2')
            sBIN_OUTPUT = sBIN_OUTPUT + "0010";
        else if (sHEX_INPUT[iI] == '3')
            sBIN_OUTPUT = sBIN_OUTPUT + "0011";
        else if (sHEX_INPUT[iI] == '4')
            sBIN_OUTPUT = sBIN_OUTPUT + "0100";
        else if (sHEX_INPUT[iI] == '5')
            sBIN_OUTPUT = sBIN_OUTPUT + "0101";
        else if (sHEX_INPUT[iI] == '6')
            sBIN_OUTPUT = sBIN_OUTPUT + "0110";
        else if (sHEX_INPUT[iI] == '7')
            sBIN_OUTPUT = sBIN_OUTPUT + "0111";
        else if (sHEX_INPUT[iI] == '8')
            sBIN_OUTPUT = sBIN_OUTPUT + "1000";
        else if (sHEX_INPUT[iI] == '9')
            sBIN_OUTPUT = sBIN_OUTPUT + "1001";
        else if (sHEX_INPUT[iI] == 'A')
            sBIN_OUTPUT = sBIN_OUTPUT + "1010";
        else if (sHEX_INPUT[iI] == 'B')
            sBIN_OUTPUT = sBIN_OUTPUT + "1011";
        else if (sHEX_INPUT[iI] == 'C')
            sBIN_OUTPUT = sBIN_OUTPUT + "1100";
        else if (sHEX_INPUT[iI] == 'D')
            sBIN_OUTPUT = sBIN_OUTPUT + "1101";
        else if (sHEX_INPUT[iI] == 'E')
            sBIN_OUTPUT = sBIN_OUTPUT + "1110";
        else if (sHEX_INPUT[iI] == 'F')
            sBIN_OUTPUT = sBIN_OUTPUT + "1111";
        else {
            throw Exception("输入非十六进制！");
        }
    }

    sBIN_OUTPUT.TrimLeft("0");     //消除左边的“0”
    return sBIN_OUTPUT;
}

CString ConvertStringToHex(CString sText, int iBlockSize)
{
    CString sResult = _T(""), sTemp;
    TCHAR c;

    for (int i = 0; i < sText.GetLength(); ++i) {
        c = sText.GetAt(i);
        sTemp.Format(_T("%04X"), c);
        sResult += sTemp;
    }

    while (sResult.GetLength() % iBlockSize != 0) {
        sTemp.Format(_T("%04X"), _T(" "));
        sResult += sTemp;
    }

    return sResult;
}

CString ConvertHexToString(CString sText)
{
    wchar_t c;
    CString sResult, sTemp;
    int iTemp, iSize, iMultiplier;
    unsigned int ctemp;
    // Get text length
    iSize = sText.GetLength();

    // Loop the hex string
    for (int i = 0; i < iSize; i += 4) {
        // Initialize temp value
        iTemp = 0;

        // Loop the four hex numbers
        for (int j = 0; j < 4; j++) {
            // Get char
            c = sText.GetAt(i + j);
            // Calculate multiplier
            iMultiplier = 3 - j;

            // Convert back to a valid integer
            if ((c == 'A') || (c == 'B') || (c == 'C') || (c == 'D') || (c == 'E') || (c == 'F')) {
                ctemp = c;

                if (iMultiplier <= 0) {
                    iTemp += (ctemp - 55);
                } else {
                    iTemp += (ctemp - 55) * (iMultiplier * 16);
                }
            } else {
                if (iMultiplier <= 0) {
                    iTemp += _ttoi(&c);
                } else {
                    iTemp += _ttoi(&c) * (iMultiplier * 16);
                }
            }
        }

        // Add character to result
        sTemp.Format(_T("%c"), iTemp);
        sResult += sTemp;
    }

    return sResult;
}

string16 GetHiString()
{
    string16 sayhi;
    SYSTEMTIME st;
    ::GetLocalTime(&st);

    if (st.wHour >= 5 && st.wHour <= 7)
        sayhi = L"早上好";
    else if (st.wHour >= 8 && st.wHour <= 11)
        sayhi = L"上午好";
    else if (st.wHour >= 12 && st.wHour <= 17)
        sayhi = L"下午好";
    else
        sayhi = L"晚上好";

    return sayhi;
}

string16 GetSysDateString()
{
    SYSTEMTIME sysTime = {0};
    char16 szDate[MAX_PATH] = {0};
    ::GetSystemTime(&sysTime);
    ::swprintf_s(szDate, L"%u-%02u-%u", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
    return string16(szDate);
}

ULARGE_INTEGER ParseVersion( LPCTSTR szVer )
{
    DWORD d1 = 0, d2 = 0, d3 = 0, d4 = 0;
    _stscanf( szVer, _T("%u.%u.%u.%u"), &d1, &d2, &d3, &d4 );

    ULARGE_INTEGER ll;	
    ll.HighPart = MAKELONG( d2, d1 );
    ll.LowPart = MAKELONG( d4, d3 );
    return ll;
}

BOOL EvalCompareResult( INT nRet, LPCTSTR sOpt, BOOL &bResult )
{
    if( _tcsicmp(sOpt, _T("<"))==0 )
        bResult = nRet < 0;
    else if( _tcsicmp(sOpt, _T("<="))==0 )
        bResult = nRet <= 0;
    else if( _tcsicmp(sOpt, _T(">"))==0)
        bResult = nRet > 0;
    else if( _tcsicmp(sOpt, _T(">="))==0 )
        bResult = nRet >= 0;
    else if( _tcsicmp(sOpt, _T("="))==0 || _tcsicmp(sOpt, _T("=="))==0 )
        bResult = nRet == 0;
    else if( _tcsicmp(sOpt, _T("!="))==0 )
        bResult = nRet != 0;
    else
        return FALSE;

    return TRUE;
}

BOOL VersionInRange(ULARGE_INTEGER lVersion, LPCTSTR lpszLowerVersion, LPCTSTR lpszUpperVersion )
{
    BOOL bResult = FALSE;
    if( _tcslen(lpszLowerVersion)>0 
        && !(CompareVersion(lVersion, ParseVersion(lpszLowerVersion), _T(">="), bResult ) && bResult) )
        return FALSE;

    if( _tcslen(lpszUpperVersion)>0 
        && !(CompareVersion( lVersion, ParseVersion(lpszUpperVersion), _T("<"), bResult ) && bResult) )
        return FALSE;
    return TRUE;
}

BOOL CompareVersion( const ULARGE_INTEGER &v1, const ULARGE_INTEGER &v2, LPCTSTR sOpt, BOOL &bResult )
{
    INT nRet = CompareLargeInts(v1, v2);
    return EvalCompareResult(nRet, sOpt, bResult);
}

int CompareVersion(const CString& strVer1, const CString& strVer2)
{
    std::vector<int> vecVer1Nums;
    std::vector<int> vecVer2Nums;
    CString strToken;
    int nCurPos = 0;
    strToken = strVer1.Tokenize(L".", nCurPos);

    while (nCurPos != -1) {
        vecVer1Nums.push_back(_ttoi(strToken));
        strToken = strVer1.Tokenize(L".", nCurPos);
    }

    nCurPos = 0;
    strToken = strVer2.Tokenize(L".", nCurPos);

    while (nCurPos != -1) {
        vecVer2Nums.push_back(_ttoi(strToken));
        strToken = strVer2.Tokenize(L".", nCurPos);
    }

    int nSize1 = (int)vecVer1Nums.size();
    int nSize2 = (int)vecVer2Nums.size();

    for (int i = 0; i < nSize1 && i < nSize2; i++) {
        if (vecVer1Nums[i] == vecVer2Nums[i])
            continue;

        if (vecVer1Nums[i] > vecVer2Nums[i])
            return 1;
        else
            return -1;
    }

    if (nSize1 == nSize2)
        return 0;
    else if (nSize1 > nSize2)
        return 1;
    else
        return -1;
}

CString FindVersionSeperator(CString sVersion)
{
    // Declare variables
    CString sSeperator = _T(""), sSeperatorToFind = _T("");
    int iSeperatorCount = 0, iPosition;

    // Find out what kind of separator the user is using
    while ((iSeperatorCount < 7) && (sSeperator.IsEmpty())) {
        // Search for our seperator
        switch (iSeperatorCount) {
        case 0:
            // Search for .
            sSeperatorToFind = _T(".");
            break;

        case 1:
            // Search for ,
            sSeperatorToFind = _T(",");
            break;

        case 2:
            // Search for _
            sSeperatorToFind = _T("_");
            break;

        case 3:
            // Search for |
            sSeperatorToFind = _T("|");
            break;

        case 4:
            // Search for -
            sSeperatorToFind = _T("-");
            break;

        case 5:
            // Search for space
            sSeperatorToFind = _T(" ");
            break;

        default:
            // We cannot found our seperator, treat versions as 1 block
            sSeperator = _T("");
            break;
        }

        // Do the search
        iPosition = sVersion.Find(sSeperatorToFind, 0);

        if (iPosition != -1) {
            // We found our seperator
            sSeperator = sSeperatorToFind;
        }

        // Search for next if we didn't found anything
        iSeperatorCount++;
    }

    // Return the seperator
    return sSeperator;
}

int CompareVersions(CString sVersion1, CString sVersion2)
{
    const int COMPAREVERSION_EQUAL          = 0;
    const int COMPAREVERSION_FIRSTLARGER    = 1;
    const int COMPAREVERSION_FIRSTSMALLER   = 2;
    // Declare variables
    CString arrVersion1[50], arrVersion2[50], sSeperator1, sSeperator2;
    int iPosition, iVersion1Count = 0, iVersion2Count = 0, i;
    // Find out what kind of separator the user is using
    sSeperator1 = FindVersionSeperator(sVersion1);
    sSeperator2 = FindVersionSeperator(sVersion2);
    // Parse versions
    iPosition = sVersion1.Find(sSeperator1, 0);

    while (iPosition != -1) {
        // Copy number of text into field
        if (sSeperator1.IsEmpty()) {
            arrVersion1[iVersion1Count++] = sVersion1;
        } else {
            arrVersion1[iVersion1Count++] = sVersion1.Left(iPosition);
        }

        sVersion1.Delete(0, iPosition + 1);
        iPosition = sVersion1.Find(sSeperator1, 0);
    }

    arrVersion1[iVersion1Count++] = sVersion1;
    iPosition = sVersion2.Find(sSeperator2, 0);

    while (iPosition != -1) {
        // Copy number of text into field
        if (sSeperator2.IsEmpty()) {
            arrVersion2[iVersion2Count++] = sVersion2;
        } else {
            arrVersion2[iVersion2Count++] = sVersion2.Left(iPosition);
        }

        sVersion2.Delete(0, iPosition + 1);
        iPosition = sVersion2.Find(sSeperator2, 0);
    }

    arrVersion2[iVersion2Count++] = sVersion2;

    // Strip all unneeded spaces
    for (i = 0; i < iVersion1Count; i++) {
        arrVersion1[i].Remove(_T(' '));
    }

    for (i = 0; i < iVersion2Count; i++) {
        arrVersion2[i].Remove(_T(' '));
    }

    // Compare the versions
    for (i = 0; i < 50; i++) {
        if (atol(convert_to<string8>((LPCWSTR)arrVersion1[i]).c_str()) < atol(convert_to<string8>((LPCWSTR)arrVersion2[i]).c_str()))
            return COMPAREVERSION_FIRSTSMALLER;

        if (atol(convert_to<string8>((LPCWSTR)arrVersion1[i]).c_str()) > atol(convert_to<string8>((LPCWSTR)arrVersion2[i]).c_str()))
            return COMPAREVERSION_FIRSTLARGER;
    }

    return COMPAREVERSION_EQUAL;
}

int CompareLargeInts(ULARGE_INTEGER ullParam1, ULARGE_INTEGER ullParam2)
{
    if( ullParam1.HighPart > ullParam2.HighPart )
        return 1;
    if( ullParam1.HighPart < ullParam2.HighPart )
        return -1;

    if( ullParam1.LowPart > ullParam2.LowPart )
        return 1;
    if( ullParam1.LowPart < ullParam2.LowPart )
        return -1;

    return 0;
}

ULARGE_INTEGER MakeInt64( WORD a, WORD b, WORD c, WORD d )
{
    ULARGE_INTEGER ull;
    ull.HighPart = MAKELONG(b,a);
    ull.LowPart = MAKELONG(d,c);
    return ull;
}

BOOL IsNumber( LPTSTR lpszName )
{
    if ( NULL == lpszName || *lpszName==0 )
        return FALSE;

    BOOL bRet = TRUE;
    BOOL gotDot = FALSE;
    while( *lpszName != 0 )
    {
        if( !_istdigit( *lpszName ) )
        {
            if( *lpszName==_T('.') && !gotDot )
                gotDot = TRUE;
            else
            {
                bRet = FALSE;
                break;
            }
        }
        ++lpszName;
    }
    return bRet;
}

#if defined(_MSC_VER) && defined(_DEBUG)
void OutputDebugStringFormat(LPCTSTR fmt, ...)
{
    CString sMsg;
    va_list ap;
    va_start(ap, fmt);
    sMsg.FormatV(fmt, ap);
    OutputDebugString(sMsg);
}
#endif

static const int kMaxLogLen = 16 * 1024;
#define MAX_LEN (kMaxLogLen + 1)

void CCLog(const char* pszFormat, ...)
{
    char szBuf[MAX_LEN];
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
    va_end(ap);
    WCHAR wszBuf[MAX_LEN] = {0};
    MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
    OutputDebugStringW(wszBuf);
    OutputDebugStringA("\n");
    WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
    printf("%s\n", szBuf);
}



void Hash::MD5(const unsigned char* message, size_t messageSize, unsigned char digest[16])
{
    HCRYPTPROV cP       = NULL;
    HCRYPTHASH hash     = NULL;
    DWORD      hashSize = 16;
    CryptAcquireContext(&cP, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
    CryptCreateHash(cP, CALG_MD5, 0, 0, &hash);
    CryptHashData(hash, message, (DWORD)messageSize, 0);
    CryptGetHashParam(hash, HP_HASHVAL, digest, &hashSize, 0);

    if (hash) {
        CryptDestroyHash(hash);
    }

    if (cP) {
        CryptReleaseContext(cP, 0);
    }
}

void Hash::SHA1(const unsigned char* message, size_t messageSize, unsigned char digest[20])
{
    HCRYPTPROV cP       = NULL;
    HCRYPTHASH hash     = NULL;
    DWORD      hashSize = 20;
    CryptAcquireContext(&cP, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
    CryptCreateHash(cP, CALG_SHA1, 0, 0, &hash);
    CryptHashData(hash, message, (DWORD)messageSize, 0);
    CryptGetHashParam(hash, HP_HASHVAL, digest, &hashSize, 0);

    if (hash) {
        CryptDestroyHash(hash);
    }

    if (cP) {
        CryptReleaseContext(cP, 0);
    }
}

#define MD5LEN  16
BOOL Hash::GetFileMD5(const wchar_t* filename, BOOL bUpperCase, TCHAR *pszResult)
{
    BOOL bRet = FALSE;
    HCRYPTPROV cP = NULL;
    HCRYPTHASH hash = NULL;
    HANDLE hFile = NULL;
    HANDLE hMapFile = NULL;  
    BYTE rgbHash[MD5LEN];
    DWORD cbHash = 0;
    TCHAR szResult[MD5LEN * 2 + 1] = {0};
    CHAR rgbDigitsL[] = "0123456789abcdef";
    CHAR rgbDigitsU[] = "0123456789ABCDEF";
    CHAR *rgbDigits = bUpperCase ? rgbDigitsU : rgbDigitsL;        
    DWORD dwFileSize = 0, dwHighFileSize = 0;
    BYTE* pBuf;

    do {
        hFile = CreateFile(filename,
                           GENERIC_READ,
                           FILE_SHARE_READ,
                           NULL,
                           OPEN_EXISTING,
                           FILE_FLAG_SEQUENTIAL_SCAN,
                           NULL);

        if (INVALID_HANDLE_VALUE == hFile)
            break;

        dwFileSize = GetFileSize(hFile, &dwHighFileSize);
        hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);

        if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE)
            break;

        bRet = CryptAcquireContext(&cP,
                                      NULL,
                                      NULL,
                                      PROV_RSA_FULL,
                                      CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET);

        if (!bRet)
            break;

        bRet = CryptCreateHash(cP, CALG_MD5, 0, 0, &hash);

        if (!bRet)
            break;

        UINT64 filesize = (UINT64)dwHighFileSize << 32 | dwFileSize;

        for (UINT64 i = 0; i <= filesize; i += 1048576) { // 1024*1024
            size_t bufsize = size_t(min(filesize - i, 1048576));
            pBuf = (BYTE*) MapViewOfFile(hMapFile, FILE_MAP_READ, (DWORD)(i >> 32), (DWORD)i, bufsize);

            if (pBuf == NULL)
                break;

            if (!CryptHashData(hash, pBuf, (DWORD)bufsize, 0))
                break;

            UnmapViewOfFile(pBuf);
        }

        if (bRet = CryptGetHashParam(hash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
            TCHAR szTmpBuff[3] = {0};

            for (DWORD i = 0; i < cbHash; i++) {
                swprintf(szTmpBuff, TEXT("%c%c"), rgbDigits[rgbHash[i] >> 4],
                         rgbDigits[rgbHash[i] & 0xf]);
                lstrcat(szResult, szTmpBuff);
            }

            bRet = TRUE;
        }
    } while (0);    

    if (hash)
        CryptDestroyHash(hash);

    if (cP)
        CryptReleaseContext(cP, 0);

    if (pBuf)
        UnmapViewOfFile(pBuf);

    if (hMapFile)
        CloseHandle(hMapFile);

    if (hFile)
        CloseHandle(hFile);

    lstrcpy(pszResult, szResult);
    return bRet;
}

namespace digit_conversion
{
char digits[] = {
    '0', '1', '2', '3', '4', '5',
    '6', '7', '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
};

std::string ToUnsignedString(int i, int shift)
{
    char  buf[32];
    char * pBuf = buf;
    int charPos = 32;
    int radix = 1 << shift;
    int mask = radix - 1;

    do {
        pBuf[--charPos] = digits[i & mask];
        i = i >> shift;
    } while (i != 0);

    std::string str;
    int strLen = 32 - charPos;
    pBuf = pBuf + charPos;

    while (strLen) {
        str.push_back(*pBuf);
        pBuf++;
        strLen--;
    }

    return str;
}

std::string ToHexString(int i)
{
    return ToUnsignedString(i, 4);
}

std::string ToOctalString(int i)
{
    return  ToUnsignedString(i, 3);
}

std::string ToBinaryString(int i)
{
    return  ToUnsignedString(i, 1);
}

} // namespace digit_conversion
} // namespace cactus