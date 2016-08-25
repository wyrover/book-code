#include <iostream>
#include "CTargaImage.h"

CTargaImage::CTargaImage() : m_pImageData(NULL)
{
}

CTargaImage::~CTargaImage()
{
    Release();
}

void CTargaImage::SwapRedBlue()
{
    switch (m_colorDepth) {
    case 32: {
        unsigned char temp;
        rgba_t* source = (rgba_t*)m_pImageData;

        for (int pixel = 0; pixel < (m_width * m_height); ++pixel) {
            temp = source[pixel].b;
            source[pixel].b = source[pixel].r;
            source[pixel].r = temp;
        }
    }
    break;

    case 24: {
        unsigned char temp;
        rgb_t* source = (rgb_t*)m_pImageData;

        for (int pixel = 0; pixel < (m_width * m_height); ++pixel) {
            temp = source[pixel].b;
            source[pixel].b = source[pixel].r;
            source[pixel].r = temp;
        }
    }
    break;

    default:
        // ignore other color depths
        break;
    }
}

bool CTargaImage::Load(const char *filename)
{
    FILE *pFile = fopen(filename, "rb");

    if (!pFile)
        return false;

    tgaheader_t tgaHeader;
    // read the TGA header
    fread(&tgaHeader, 1, sizeof(tgaheader_t), pFile);

    // see if the image type is one that we support (RGB, RGB RLE, GRAYSCALE, GRAYSCALE RLE)
    if (((tgaHeader.imageTypeCode != TGA_RGB) && (tgaHeader.imageTypeCode != TGA_GRAYSCALE) &&
         (tgaHeader.imageTypeCode != TGA_RGB_RLE) && (tgaHeader.imageTypeCode != TGA_GRAYSCALE_RLE)) ||
        tgaHeader.colorMapType != 0) {
        fclose(pFile);
        return false;
    }

    // get image width and height
    m_width = tgaHeader.width;
    m_height = tgaHeader.height;
    // colormode -> 3 = BGR, 4 = BGRA
    int colorMode = tgaHeader.bpp / 8;

    // we don't handle less than 24 bit
    if (colorMode < 3) {
        fclose(pFile);
        return false;
    }

    m_imageSize = m_width * m_height * colorMode;
    // allocate memory for TGA image data
    m_pImageData = new unsigned char[m_imageSize];

    // skip past the id if there is one
    if (tgaHeader.idLength > 0)
        fseek(pFile, SEEK_CUR, tgaHeader.idLength);

    // read image data
    if (tgaHeader.imageTypeCode == TGA_RGB || tgaHeader.imageTypeCode == TGA_GRAYSCALE) {
        fread(m_pImageData, 1, m_imageSize, pFile);
    } else {
        // this is an RLE compressed image
        unsigned char id;
        unsigned char length;
        rgba_t color = { 0, 0, 0, 0 };
        unsigned int i = 0;

        while (i < m_imageSize) {
            id = fgetc(pFile);

            // see if this is run length data
            if (id >= 128) { // & 0x80)
                // find the run length
                length = (unsigned char)(id - 127);
                // next 3 (or 4) bytes are the repeated values
                color.b = (unsigned char)fgetc(pFile);
                color.g = (unsigned char)fgetc(pFile);
                color.r = (unsigned char)fgetc(pFile);

                if (colorMode == 4)
                    color.a = (unsigned char)fgetc(pFile);

                // save everything in this run
                while (length > 0) {
                    m_pImageData[i++] = color.b;
                    m_pImageData[i++] = color.g;
                    m_pImageData[i++] = color.r;

                    if (colorMode == 4)
                        m_pImageData[i++] = color.a;

                    --length;
                }
            } else {
                // the number of non RLE pixels
                length = unsigned char(id + 1);

                while (length > 0) {
                    color.b = (unsigned char)fgetc(pFile);
                    color.g = (unsigned char)fgetc(pFile);
                    color.r = (unsigned char)fgetc(pFile);

                    if (colorMode == 4)
                        color.a = (unsigned char)fgetc(pFile);

                    m_pImageData[i++] = color.b;
                    m_pImageData[i++] = color.g;
                    m_pImageData[i++] = color.r;

                    if (colorMode == 4)
                        m_pImageData[i++] = color.a;

                    --length;
                }
            }
        }
    }

    fclose(pFile);

    switch (tgaHeader.imageTypeCode) {
    case TGA_RGB:
    case TGA_RGB_RLE:
        if (3 == colorMode) {
            m_imageDataFormat = IMAGE_RGB;
            m_imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
            m_colorDepth = 24;
        } else {
            m_imageDataFormat = IMAGE_RGBA;
            m_imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
            m_colorDepth = 32;
        }

        break;

    case TGA_GRAYSCALE:
    case TGA_GRAYSCALE_RLE:
        m_imageDataFormat = IMAGE_LUMINANCE;
        m_imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
        m_colorDepth = 8;
        break;
    }

    if ((tgaHeader.imageDesc & TOP_LEFT) == TOP_LEFT)
        FlipVertical();

    // swap the red and blue components in the image data
    SwapRedBlue();
    return (m_pImageData != NULL);
}

bool CTargaImage::FlipVertical()
{
    if (!m_pImageData)
        return false;

    if (m_colorDepth == 32) {
        rgba_t* tmpBits = new rgba_t[m_width];

        if (!tmpBits)
            return false;

        int lineWidth = m_width * 4;
        rgba_t* top = (rgba_t*)m_pImageData;
        rgba_t* bottom = (rgba_t*)(m_pImageData + lineWidth * (m_height - 1));

        for (int i = 0; i < (m_height / 2); ++i) {
            memcpy(tmpBits, top, lineWidth);
            memcpy(top, bottom, lineWidth);
            memcpy(bottom, tmpBits, lineWidth);
            top = (rgba_t*)((unsigned char*)top + lineWidth);
            bottom = (rgba_t*)((unsigned char*)bottom - lineWidth);
        }

        delete [] tmpBits;
        tmpBits = 0;
    } else if (m_colorDepth == 24) {
        rgb_t* tmpBits = new rgb_t[m_width];

        if (!tmpBits)
            return false;

        int lineWidth = m_width * 3;
        rgb_t* top = (rgb_t*)m_pImageData;
        rgb_t* bottom = (rgb_t*)(m_pImageData + lineWidth * (m_height - 1));

        for (int i = 0; i < (m_height / 2); ++i) {
            memcpy(tmpBits, top, lineWidth);
            memcpy(top, bottom, lineWidth);
            memcpy(bottom, tmpBits, lineWidth);
            top = (rgb_t*)((unsigned char*)top + lineWidth);
            bottom = (rgb_t*)((unsigned char*)bottom - lineWidth);
        }

        delete [] tmpBits;
        tmpBits = 0;
    }

    return true;
}

void CTargaImage::Release()
{
    delete [] m_pImageData;
    m_pImageData = NULL;
}

bool CTargaImage::ConvertRGBToRGBA(unsigned char alphaValue)
{
    if ((m_colorDepth == 24) && (m_imageDataFormat == IMAGE_RGB)) {
        rgba_t *newImage = new rgba_t[m_width * m_height];

        if (!newImage)
            return false;

        rgba_t *dest = newImage;
        rgb_t *src = (rgb_t*)m_pImageData;

        for (int x = 0; x < m_height; x++) {
            for (int y = 0; y < m_width; y++) {
                dest->r = src->r;
                dest->g = src->g;
                dest->b = src->b;
                dest->a = alphaValue;
                ++src;
                ++dest;
            }
        }

        delete [] m_pImageData;
        m_pImageData = (unsigned char*)newImage;
        m_colorDepth = 32;
        m_imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
        m_imageDataFormat = IMAGE_RGBA;
        return true;
    }

    return false;
}

bool CTargaImage::ConvertRGBAToRGB()
{
    if ((m_colorDepth == 32) && (m_imageDataFormat == IMAGE_RGBA)) {
        rgb_t *newImage = new rgb_t[m_width * m_height];

        if (!newImage)
            return false;

        rgb_t *dest = newImage;
        rgba_t *src = (rgba_t*)m_pImageData;

        for (int x = 0; x < m_height; x++) {
            for (int y = 0; y < m_width; y++) {
                dest->r = src->r;
                dest->g = src->g;
                dest->b = src->b;
                ++src;
                ++dest;
            }
        }

        delete [] m_pImageData;
        m_pImageData = (unsigned char*)newImage;
        m_colorDepth = 24;
        m_imageDataType = IMAGE_DATA_UNSIGNED_BYTE;
        m_imageDataFormat = IMAGE_RGB;
        return true;
    }

    return false;
}

