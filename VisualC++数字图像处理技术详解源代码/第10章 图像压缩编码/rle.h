#ifndef _CK_RLE_H
#define _CK_RLE_H

#ifndef _CK_COMPRESSION_H
#include <ck_compression.h>
#endif

class CK_RLE : public CK_Compression
    {
    public:
	CK_RLE(int id = 1);
	virtual ~CK_RLE(void);

	unsigned char *compress(unsigned char *inputBuffer,
	    int inputNumBytes, int &compressedNumBytes);

	 int decompress(unsigned char *inputBuffer,
	    int inputNumBytes, 
	    unsigned char *outputBuffer,
	    int &outputNumBytes);

    };

#endif

 
