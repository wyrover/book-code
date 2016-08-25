/******************************************************************************
 *  Copyright (C) 2000 by Robert Hubley.                                      *
 *  All rights reserved.                                                      *
 *                                                                            *
 *  This software is provided ``AS IS'' and any express or implied            *
 *  warranties, including, but not limited to, the implied warranties of      *
 *  merchantability and fitness for a particular purpose, are disclaimed.     *
 *  In no event shall the authors be liable for any direct, indirect,         *
 *  incidental, special, exemplary, or consequential damages (including, but  *
 *  not limited to, procurement of substitute goods or services; loss of use, *
 *  data, or profits; or business interruption) however caused and on any     *
 *  theory of liability, whether in contract, strict liability, or tort       *
 *  (including negligence or otherwise) arising in any way out of the use of  *
 *  this software, even if advised of the possibility of such damage.         *
 *                                                                            *
 ******************************************************************************
 *
 * CLASS: MD5
 *
 * DESCRIPTION:
 *    This is a class which encapsulates a set of MD5 Message Digest functions.
 *    MD5 algorithm produces a 128 bit digital fingerprint (signature) from an
 *    dataset of arbitrary length.  For details see RFC 1321 (summarized below).
 *    This implementation is derived from the RSA Data Security, Inc. MD5 Message-Digest
 *    algorithm reference implementation (originally written in C)
 *
 * AUTHOR:
 *    Robert M. Hubley 1/2000
 *
 *
 * NOTES:
 *     Network Working Group                                    R. Rivest
 *     Request for Comments: 1321     MIT Laboratory for Computer Science
 *                                            and RSA Data Security, Inc.
 *                                                             April 1992
 *
 *
 *                          The MD5 Message-Digest Algorithm
 *
 *     Summary
 *
 *        This document describes the MD5 message-digest algorithm. The
 *        algorithm takes as input a message of arbitrary length and produces
 *        as output a 128-bit "fingerprint" or "message digest" of the input.
 *        It is conjectured that it is computationally infeasible to produce
 *        two messages having the same message digest, or to produce any
 *        message having a given prespecified target message digest. The MD5
 *        algorithm is intended for digital signature applications, where a
 *        large file must be "compressed" in a secure manner before being
 *        encrypted with a private (secret) key under a public-key cryptosystem
 *        such as RSA.
 *
 *        The MD5 algorithm is designed to be quite fast on 32-bit machines. In
 *        addition, the MD5 algorithm does not require any large substitution
 *        tables; the algorithm can be coded quite compactly.
 *
 *        The MD5 algorithm is an extension of the MD4 message-digest algorithm
 *        1,2]. MD5 is slightly slower than MD4, but is more "conservative" in
 *        design. MD5 was designed because it was felt that MD4 was perhaps
 *        being adopted for use more quickly than justified by the existing
 *        critical review; because MD4 was designed to be exceptionally fast,
 *        it is "at the edge" in terms of risking successful cryptanalytic
 *        attack. MD5 backs off a bit, giving up a little in speed for a much
 *        greater likelihood of ultimate security. It incorporates some
 *        suggestions made by various reviewers, and contains additional
 *        optimizations. The MD5 algorithm is being placed in the public domain
 *        for review and possible adoption as a standard.
 *
 *        RFC Author:
 *        Ronald L.Rivest
 *        Massachusetts Institute of Technology
 *        Laboratory for Computer Science
 *        NE43 -324545    Technology Square
 *        Cambridge, MA  02139-1986
 *        Phone: (617) 253-5880
 *        EMail:    Rivest@ theory.lcs.mit.edu
 *
 *
 *
 * CHANGE HISTORY:
 *
 *    0.1.0  RMH    1999/12/29      Original version
 *
 */


//
// Imports
//
import java.io.*;


//
// MD5 Class
//
class MD5
{
  private static long S11 = 7L;
  private static long S12 = 12L;
  private static long S13 = 17L;
  private static long S14 = 22L;
  private static long S21 = 5L;
  private static long S22 = 9L;
  private static long S23 = 14L;
  private static long S24 = 20L;
  private static long S31 = 4L;
  private static long S32 = 11L;
  private static long S33 = 16L;
  private static long S34 = 23L;
  private static long S41 = 6L;
  private static long S42 = 10L;
  private static long S43 = 15L;
  private static long S44 = 21L;
 
  private static char pad[] = {128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			      0, 0, 0, 0, 0, 0, 0, 0, 0};

  private char bytBuffer[] = new char[64];
  private long lngState[] = new long[4];
  private long lngByteCount = 0;

  //
  // Constructor
  //
  MD5() 
  {
	this.init();
  }

  //
  // Static Methods
  //
  private static long[] decode(char bytBlock[])
  {
    long lngBlock[] = new long[16];
    int j = 0;
    for(int i = 0; i < bytBlock.length; i += 4) 
    {
	lngBlock[j++] =  bytBlock[i] +
                       bytBlock[i+1] * 256L + 
                       bytBlock[i+2] * 65536L + 
                       bytBlock[i+3] * 16777216L;
    }
    return(lngBlock);
  }

 
  private static void transform(long lngState[], char bytBlock[])
  {
    long lngA = lngState[0];
    long lngB = lngState[1];
    long lngC = lngState[2];
    long lngD = lngState[3];
    long x[] = new long[16];

    x = decode (bytBlock);

    /* Round 1 */
    lngA = ff (lngA, lngB, lngC, lngD, x[ 0], S11, 0xd76aa478L); /* 1 */
    lngD = ff (lngD, lngA, lngB, lngC, x[ 1], S12, 0xe8c7b756L); /* 2 */
    lngC = ff (lngC, lngD, lngA, lngB, x[ 2], S13, 0x242070dbL); /* 3 */
    lngB = ff (lngB, lngC, lngD, lngA, x[ 3], S14, 0xc1bdceeeL); /* 4 */
    lngA = ff (lngA, lngB, lngC, lngD, x[ 4], S11, 0xf57c0fafL); /* 5 */
    lngD = ff (lngD, lngA, lngB, lngC, x[ 5], S12, 0x4787c62aL); /* 6 */
    lngC = ff (lngC, lngD, lngA, lngB, x[ 6], S13, 0xa8304613L); /* 7 */
    lngB = ff (lngB, lngC, lngD, lngA, x[ 7], S14, 0xfd469501L); /* 8 */
    lngA = ff (lngA, lngB, lngC, lngD, x[ 8], S11, 0x698098d8L); /* 9 */
    lngD = ff (lngD, lngA, lngB, lngC, x[ 9], S12, 0x8b44f7afL); /* 10 */
    lngC = ff (lngC, lngD, lngA, lngB, x[10], S13, 0xffff5bb1L); /* 11 */
    lngB = ff (lngB, lngC, lngD, lngA, x[11], S14, 0x895cd7beL); /* 12 */
    lngA = ff (lngA, lngB, lngC, lngD, x[12], S11, 0x6b901122L); /* 13 */
    lngD = ff (lngD, lngA, lngB, lngC, x[13], S12, 0xfd987193L); /* 14 */
    lngC = ff (lngC, lngD, lngA, lngB, x[14], S13, 0xa679438eL); /* 15 */
    lngB = ff (lngB, lngC, lngD, lngA, x[15], S14, 0x49b40821L); /* 16 */
    
    /* Round 2 */
    lngA = gg (lngA, lngB, lngC, lngD, x[ 1], S21, 0xf61e2562L); /* 17 */
    lngD = gg (lngD, lngA, lngB, lngC, x[ 6], S22, 0xc040b340L); /* 18 */
    lngC = gg (lngC, lngD, lngA, lngB, x[11], S23, 0x265e5a51L); /* 19 */
    lngB = gg (lngB, lngC, lngD, lngA, x[ 0], S24, 0xe9b6c7aaL); /* 20 */
    lngA = gg (lngA, lngB, lngC, lngD, x[ 5], S21, 0xd62f105dL); /* 21 */
    lngD = gg (lngD, lngA, lngB, lngC, x[10], S22,  0x2441453L); /* 22 */
    lngC = gg (lngC, lngD, lngA, lngB, x[15], S23, 0xd8a1e681L); /* 23 */
    lngB = gg (lngB, lngC, lngD, lngA, x[ 4], S24, 0xe7d3fbc8L); /* 24 */
    lngA = gg (lngA, lngB, lngC, lngD, x[ 9], S21, 0x21e1cde6L); /* 25 */
    lngD = gg (lngD, lngA, lngB, lngC, x[14], S22, 0xc33707d6L); /* 26 */
    lngC = gg (lngC, lngD, lngA, lngB, x[ 3], S23, 0xf4d50d87L); /* 27 */
    lngB = gg (lngB, lngC, lngD, lngA, x[ 8], S24, 0x455a14edL); /* 28 */
    lngA = gg (lngA, lngB, lngC, lngD, x[13], S21, 0xa9e3e905L); /* 29 */
    lngD = gg (lngD, lngA, lngB, lngC, x[ 2], S22, 0xfcefa3f8L); /* 30 */
    lngC = gg (lngC, lngD, lngA, lngB, x[ 7], S23, 0x676f02d9L); /* 31 */
    lngB = gg (lngB, lngC, lngD, lngA, x[12], S24, 0x8d2a4c8aL); /* 32 */

    /* Round 3 */
    lngA = hh (lngA, lngB, lngC, lngD, x[ 5], S31, 0xfffa3942L); /* 33 */
    lngD = hh (lngD, lngA, lngB, lngC, x[ 8], S32, 0x8771f681L); /* 34 */
    lngC = hh (lngC, lngD, lngA, lngB, x[11], S33, 0x6d9d6122L); /* 35 */
    lngB = hh (lngB, lngC, lngD, lngA, x[14], S34, 0xfde5380cL); /* 36 */
    lngA = hh (lngA, lngB, lngC, lngD, x[ 1], S31, 0xa4beea44L); /* 37 */
    lngD = hh (lngD, lngA, lngB, lngC, x[ 4], S32, 0x4bdecfa9L); /* 38 */
    lngC = hh (lngC, lngD, lngA, lngB, x[ 7], S33, 0xf6bb4b60L); /* 39 */
    lngB = hh (lngB, lngC, lngD, lngA, x[10], S34, 0xbebfbc70L); /* 40 */
    lngA = hh (lngA, lngB, lngC, lngD, x[13], S31, 0x289b7ec6L); /* 41 */
    lngD = hh (lngD, lngA, lngB, lngC, x[ 0], S32, 0xeaa127faL); /* 42 */
    lngC = hh (lngC, lngD, lngA, lngB, x[ 3], S33, 0xd4ef3085L); /* 43 */
    lngB = hh (lngB, lngC, lngD, lngA, x[ 6], S34,  0x4881d05L); /* 44 */
    lngA = hh (lngA, lngB, lngC, lngD, x[ 9], S31, 0xd9d4d039L); /* 45 */
    lngD = hh (lngD, lngA, lngB, lngC, x[12], S32, 0xe6db99e5L); /* 46 */
    lngC = hh (lngC, lngD, lngA, lngB, x[15], S33, 0x1fa27cf8L); /* 47 */
    lngB = hh (lngB, lngC, lngD, lngA, x[ 2], S34, 0xc4ac5665L); /* 48 */
   
    /* Round 4 */
    lngA = ii (lngA, lngB, lngC, lngD, x[ 0], S41, 0xf4292244L); /* 49 */
    lngD = ii (lngD, lngA, lngB, lngC, x[ 7], S42, 0x432aff97L); /* 50 */
    lngC = ii (lngC, lngD, lngA, lngB, x[14], S43, 0xab9423a7L); /* 51 */
    lngB = ii (lngB, lngC, lngD, lngA, x[ 5], S44, 0xfc93a039L); /* 52 */
    lngA = ii (lngA, lngB, lngC, lngD, x[12], S41, 0x655b59c3L); /* 53 */
    lngD = ii (lngD, lngA, lngB, lngC, x[ 3], S42, 0x8f0ccc92L); /* 54 */
    lngC = ii (lngC, lngD, lngA, lngB, x[10], S43, 0xffeff47dL); /* 55 */
    lngB = ii (lngB, lngC, lngD, lngA, x[ 1], S44, 0x85845dd1L); /* 56 */
    lngA = ii (lngA, lngB, lngC, lngD, x[ 8], S41, 0x6fa87e4fL); /* 57 */
    lngD = ii (lngD, lngA, lngB, lngC, x[15], S42, 0xfe2ce6e0L); /* 58 */
    lngC = ii (lngC, lngD, lngA, lngB, x[ 6], S43, 0xa3014314L); /* 59 */
    lngB = ii (lngB, lngC, lngD, lngA, x[13], S44, 0x4e0811a1L); /* 60 */
    lngA = ii (lngA, lngB, lngC, lngD, x[ 4], S41, 0xf7537e82L); /* 61 */
    lngD = ii (lngD, lngA, lngB, lngC, x[11], S42, 0xbd3af235L); /* 62 */
    lngC = ii (lngC, lngD, lngA, lngB, x[ 2], S43, 0x2ad7d2bbL); /* 63 */
    lngB = ii (lngB, lngC, lngD, lngA, x[ 9], S44, 0xeb86d391L); /* 64 */

    lngState[0] = (lngState[0] + lngA) & 0xFFFFFFFFL;
    lngState[1] = (lngState[1] + lngB) & 0xFFFFFFFFL;
    lngState[2] = (lngState[2] + lngC) & 0xFFFFFFFFL;
    lngState[3] = (lngState[3] + lngD) & 0xFFFFFFFFL;

    /* clear senstive information */
    x = decode (pad);
  }

  private static long ff(long lngA,
	 		long lngB, 
			long lngC,
			long lngD,
			long lngX,
			long lngS,
			long lngAC)
  {
	lngA = (lngA + (lngB & lngC | (~lngB) & lngD) + lngX + lngAC) & 0xFFFFFFFFL;
	lngA = ((lngA << lngS) | (lngA >>> (32L-lngS))) & 0xFFFFFFFFL;
	lngA = (lngA + lngB) & 0xFFFFFFFFL;
	return(lngA);
  }




  private static long gg(long lngA,
	 		long lngB, 
			long lngC,
			long lngD,
			long lngX,
			long lngS,
			long lngAC)
  {
	lngA = (lngA +  (lngB & lngD | lngC & ~lngD) + lngX + lngAC) & 0xFFFFFFFFL;
	lngA = ((lngA << lngS) | (lngA >>> (32L-lngS))) & 0xFFFFFFFFL;
	lngA = (lngA + lngB) & 0xFFFFFFFFL;
	return(lngA);
  }

 private static long hh(long lngA,
	 		long lngB, 
			long lngC,
			long lngD,
			long lngX,
			long lngS,
			long lngAC)
  {
	lngA = (lngA + (lngB ^ lngC ^ lngD) + lngX + lngAC) & 0xFFFFFFFFL;
	lngA = ((lngA << lngS) | (lngA >>> (32L-lngS))) & 0xFFFFFFFFL;
	lngA = (lngA + lngB) & 0xFFFFFFFFL;
	return(lngA);
  }

 private static long ii(long lngA,
	 		long lngB, 
			long lngC,
			long lngD,
			long lngX,
			long lngS,
			long lngAC)
  {
	lngA = (lngA + (lngC ^ (lngB | ~lngD)) + lngX + lngAC) & 0xFFFFFFFFL;
	lngA = ((lngA << lngS) | (lngA >>> (32L-lngS))) & 0xFFFFFFFFL;
	lngA = (lngA + lngB) & 0xFFFFFFFFL;
	return(lngA);
  }


  private void update(char bytInput[], long lngLen)
  {
    int index = (int)( this.lngByteCount % 64);
    int i = 0;
    this.lngByteCount += lngLen;
    int partLen = 64 - index;
    
    if (lngLen >= partLen) 
    {
      for (int j = 0; j < partLen; ++j) 
      {
        this.bytBuffer[j + index] = bytInput[j]; 
      }
      transform (this.lngState, this.bytBuffer);

      for (i = partLen; i + 63 < lngLen; i += 64)
      {
      	for (int j = 0; j<64; ++j) 
      	{
          this.bytBuffer[j] = bytInput[j+i]; 
        }	
        transform (this.lngState, this.bytBuffer);
      }
      index = 0;
    }
    else
    {
      i = 0;
    }

    for (int j = 0; j < lngLen - i; ++j) 
    {  
      this.bytBuffer[index + j] = bytInput[i + j];
    }
    
  }

  public void md5final()
  {
    char bytBits[] = new char[8];
    int index, padLen;
    long bits = this.lngByteCount * 8;
    
    bytBits[0] = (char) (bits & 0xffL);
    bytBits[1] = (char) ((bits >>> 8) & 0xffL);
    bytBits[2] = (char) ((bits >>> 16) & 0xffL);
    bytBits[3] = (char)((bits >>> 24) & 0xffL);
    bytBits[4] = (char)((bits >>> 32) & 0xffL);
    bytBits[5] = (char)((bits >>> 40) & 0xffL);
    bytBits[6] = (char)((bits >>> 48) & 0xffL);
    bytBits[7] = (char)((bits >>> 56) & 0xffL);
 
    index = (int) this.lngByteCount%64;
    if (index < 56 )
    {
      padLen = 56 - index;
    }
    else
    {
      padLen = 120 - index;
    }
    update(pad, padLen);
    update(bytBits, 8);	    

  }

  private StringBuffer toHexString()
  {
    long myByte = 0;
    StringBuffer mystring = new StringBuffer();
    for (int j = 0; j < 4; ++j)
    {
      for (int i = 0; i < 32; i += 8)
      {
         myByte = (this.lngState[j] >>> i) & 0xFFL;
         if (myByte < 16)
         {
           mystring.append("0" + Long.toHexString(myByte));
         }
         else
         {
           mystring.append(Long.toHexString(myByte)); 
         }
      }
    }
    return(mystring);
  }

  public void init()
  {
	this.lngByteCount = 0;
  	this.lngState[0] = 0x67452301L;
  	this.lngState[1] = 0xefcdab89L;
  	this.lngState[2] = 0x98badcfeL;
  	this.lngState[3] = 0x10325476L;
  }



  //
  // MAIN routine with test data set
  //
  public static void main (String args [])
       throws IOException
  {

    String strTestData;
    char chrTestData[] = new char[64];
    char chrTestBuffer[] = new char[1000];
    MD5 md5Test = new MD5();

    strTestData = new String("");
    chrTestData = strTestData.toCharArray();    
    md5Test.update(chrTestData,chrTestData.length);    
    md5Test.md5final();
    System.out.println("MD5 (" + strTestData +") = " + md5Test.toHexString() );

    md5Test.init();
    strTestData = new String("a");
    chrTestData = strTestData.toCharArray();    
    md5Test.update(chrTestData,chrTestData.length);    
    md5Test.md5final();
    System.out.println("MD5 (" + strTestData +") = " + md5Test.toHexString() );
  
    md5Test.init();
    strTestData = new String("abc");
    chrTestData = strTestData.toCharArray();    
    md5Test.update(chrTestData,chrTestData.length);    
    md5Test.md5final();
    System.out.println("MD5 (" + strTestData +") = " + md5Test.toHexString() );

    md5Test.init();
    strTestData = new String("message digest");
    chrTestData = strTestData.toCharArray();    
    md5Test.update(chrTestData,chrTestData.length);    
    md5Test.md5final();
    System.out.println("MD5 (" + strTestData +") = " + md5Test.toHexString() );

    md5Test.init();
    strTestData = new String("abcdefghijklmnopqrstuvwxyz");
    chrTestData = strTestData.toCharArray();    
    md5Test.update(chrTestData,chrTestData.length);    
    md5Test.md5final();
    System.out.println("MD5 (" + strTestData +") = " + md5Test.toHexString() );

    md5Test.init();
    strTestData = new String("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    chrTestData = strTestData.toCharArray();    
    md5Test.update(chrTestData,chrTestData.length);    
    md5Test.md5final();
    System.out.println("MD5 (" + strTestData +") = " + md5Test.toHexString() );

    md5Test.init();
    strTestData = new String("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    chrTestData = strTestData.toCharArray();    
    md5Test.update(chrTestData,chrTestData.length);    
    md5Test.md5final();
    System.out.println("MD5 (" + strTestData +") = " + md5Test.toHexString() );


    for (int i = 0; i < chrTestBuffer.length; ++i)
    {
    	chrTestBuffer[i] = (char) (i & 0xff);
    }

    long time1 = System.currentTimeMillis();
    md5Test.init();
    for (int i = 0; i < 100000; ++i) 
    {
      md5Test.update(chrTestBuffer,chrTestBuffer.length);
    }
    md5Test.md5final();
    long time2 = (System.currentTimeMillis() - time1)/1000;
    System.out.println("MD5 Speed Test: " + time2 + "sec = " + md5Test.toHexString() );

  }
}
