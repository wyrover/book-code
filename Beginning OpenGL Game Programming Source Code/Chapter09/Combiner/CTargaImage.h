#ifndef __TARGA_LIB
#define __TARGA_LIB

enum TGATypes {
    TGA_NODATA = 0,
    TGA_INDEXED = 1,
    TGA_RGB = 2,
    TGA_GRAYSCALE = 3,
    TGA_INDEXED_RLE = 9,
    TGA_RGB_RLE = 10,
    TGA_GRAYSCALE_RLE = 11
};

// Image Data Formats
#define IMAGE_RGB       0
#define IMAGE_RGBA      1
#define IMAGE_LUMINANCE 2

// Image data types
#define IMAGE_DATA_UNSIGNED_BYTE 0

// Pixel data transfer from file to screen:
// These masks are AND'd with the imageDesc in the TGA header,
// bit 4 is left-to-right ordering
// bit 5 is top-to-bottom
#define BOTTOM_LEFT  0x00   // first pixel is bottom left corner
#define BOTTOM_RIGHT 0x10   // first pixel is bottom right corner
#define TOP_LEFT     0x20   // first pixel is top left corner
#define TOP_RIGHT    0x30   // first pixel is top right corner

// TGA header
struct tgaheader_t {
    unsigned char  idLength;
    unsigned char  colorMapType;
    unsigned char  imageTypeCode;
    unsigned char  colorMapSpec[5];
    unsigned short xOrigin;
    unsigned short yOrigin;
    unsigned short width;
    unsigned short height;
    unsigned char  bpp;
    unsigned char  imageDesc;
};

struct rgba_t {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct rgb_t {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

class CTargaImage
{
private:
    unsigned char  m_colorDepth;
    unsigned char  m_imageDataType;
    unsigned char  m_imageDataFormat;
    unsigned char *m_pImageData;
    unsigned short m_width;
    unsigned short m_height;
    unsigned long  m_imageSize;

    // swap the red and blue components in the image data
    void SwapRedBlue();

public:
    CTargaImage();
    virtual ~CTargaImage();

    // loading and unloading
    bool Load(const char *filename);
    void Release();

    // flips image vertically
    bool FlipVertical();

    unsigned short GetWidth()
    {
        return m_width;
    }
    unsigned short GetHeight()
    {
        return m_height;
    }
    unsigned char  GetImageFormat()
    {
        return m_imageDataFormat;
    }

    // converts RGB format to RGBA format and vice versa
    bool ConvertRGBAToRGB();
    bool ConvertRGBToRGBA(unsigned char alphaValue);

    // returns the current image data
    unsigned char *GetImage()
    {
        return m_pImageData;
    }
};

#endif