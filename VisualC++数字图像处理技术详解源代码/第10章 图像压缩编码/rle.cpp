#include <rle.h>

CK_RLE::CK_RLE(int id) : CK_Compression(id)
    {

    }

CK_RLE::~CK_RLE(void)
    {
    }

unsigned char *CK_RLE::compress(unsigned char *buf,
    int len, int &compressedLen)
    {
    int i;
    // Make the output buffer bigger in case the compressed
    // data is actually larger!
    int outBufSize = len+(len*4)/3;
    unsigned char *out = new unsigned char[outBufSize];
    unsigned char *outPtr = out;

    unsigned char byte1;
    unsigned char byte2;
    unsigned char frame_size;
    unsigned char array[129];

    while (len)
	{
	byte1 = *buf;
	buf++;
	len--;
	frame_size = 1;

	if (len)
	    {
	    byte2 = *buf;
	    buf++;
	    len--;
            frame_size = 2;
            do 
		{
		if (byte1 == byte2)
                    { 
		    while (len && (byte1 == byte2) && (frame_size < 129))
                        { 
			byte2 = *buf;
			buf++;
			len--;
                        frame_size++;
                        }

		    if (byte1 == byte2)
			{ 
			*outPtr = frame_size+126;
			outPtr++;
			*outPtr = byte1;
			outPtr++;
			if (len)
			    {
			    byte1=*buf;
			    buf++;
			    len--;
			    frame_size = 1;
			    }
			else
			    {
			    frame_size = 0;
			    }
                        }
		    else  
                       { 
		       *outPtr = 125+frame_size;
		       outPtr++;
		       *outPtr = byte1;
		       outPtr++;
		       byte1 = byte2;
		       frame_size = 1;
                       }

		    if (len)
			{ 
			byte2 = *buf;
			buf++;
			len--;
			frame_size = 2;
			}
                    }
		else        /* Prepare the array of comparisons
			       where will be stored all the identical bytes */
		    { 
		    *array = byte1;
		    array[1] = byte2;
		    while (len && (array[frame_size-2] != array[frame_size-1]) && (frame_size  < 128))
			{ 
			array[frame_size] = *buf;
			buf++;
			len--;
			frame_size++;
			}

		    /* Do we meet a sequence of all different bytes followed by identical byte? */
                    if (array[frame_size-2] == array[frame_size-1])
			{ 
			/* Yes, then don't count the two last bytes */
			*outPtr = frame_size-3;
			outPtr++;

			for (i=0; i<frame_size-2; i++)
			    {
			    *outPtr = array[i];
			    outPtr++;
			    }

			byte1 = array[frame_size-2];
			byte2 = byte1;
			frame_size = 2;
			}
                    else 
			{
			*outPtr = frame_size-1;
			outPtr++;

			for (i=0; i<frame_size; i++)
			    {
			    *outPtr = array[i];
			    outPtr++;
			    }

			if (!len)
			    {
			    frame_size = 0;
			    }
			else
			    {
			    byte1 = *buf;
			    buf++;
			    len--;
			    if (!len)
				{
				frame_size = 1;
				}
			    else
				{
				byte2 = *buf;
				buf++;
				len--;
				frame_size = 2;
				}
			    }
			}
		    }
		}
	    while (len || (frame_size >= 2));

	    if (frame_size == 1)
		{
		*outPtr = 0;
		outPtr++;
		*outPtr = byte1;
		outPtr++;
		}
	    }
	}

    compressedLen = outPtr-out;
    return out;
    }

int CK_RLE::decompress(unsigned char *buf,
    int len, 
    unsigned char *out,
    int &uncompressLen)
    {
    unsigned char header;
    unsigned char *outPtr = out;
    unsigned char i;
    int outSize = 0;

    while (len)
	{
	header = *buf;
	buf++;
	len--;

	if (!(header & 128))
	    {
	    // There are header+1 different bytes.
	    for (i=0; i<=header; i++)
		{
		if (outSize >= uncompressLen) return -1;
		*outPtr = *buf;
		outPtr++;
		outSize++;
		buf++;
		len--;
		}
	    }
	else
	    {
	    const unsigned int n = (header & 127) + 2;
	    for (i=0; i<n; i++)
		{
		if (outSize >= uncompressLen) return -1;
		*outPtr = *buf;
		outPtr++;
		outSize++;
		}
	    buf++;
	    len--;
	    }
	}

    uncompressLen = outSize;
    return 0;
    }
