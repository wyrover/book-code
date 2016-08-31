/*
 * qrencode - QR Code encoder
 *
 * Masking.
 * Copyright (C) 2006-2011 Kentaro Fukuchi <kentaro@fukuchi.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#if HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "windows.h"
#include "qrencode.h"
#include "stdio.h"
#include "libqr.h"

#define OUT_FILE					"D:/test.bmp"								// Output file name

void save_to_file(unsigned int w, unsigned int h, unsigned int bits, unsigned int *buffer)
{
    BITMAPFILEHEADER kFileHeader;
    BITMAPINFOHEADER kInfoHeader;
    FILE*			f;
    unsigned int bytes;

    bytes = w * h * bits;
    kFileHeader.bfType = 0x4d42;  // "BM"
    kFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bytes;
    kFileHeader.bfReserved1 = 0;
    kFileHeader.bfReserved2 = 0;
    kFileHeader.bfOffBits =	sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    kInfoHeader.biWidth = w;
    kInfoHeader.biHeight = -((int)h);
    kInfoHeader.biPlanes = 1;
    kInfoHeader.biBitCount = bits * 8;
    kInfoHeader.biCompression = BI_RGB;
    kInfoHeader.biSizeImage = 0;
    kInfoHeader.biXPelsPerMeter = 0;
    kInfoHeader.biYPelsPerMeter = 0;
    kInfoHeader.biClrUsed = 0;
    kInfoHeader.biClrImportant = 0;


    if (!(fopen_s(&f, OUT_FILE, "wb"))){
        fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
        fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
        fwrite(buffer, sizeof(unsigned char), bytes, f);

        fclose(f);
    }
}

int QREncodeUrl(QRImage* qrimg)
{
    QRcode* pQRC;
    unsigned char *src;
    unsigned int dst_w, dst_h, src_w, src_h, totalpixels = 0, totalbytes = 0, bits = 4, offset = 0;
    unsigned int x,y,r,c;
    unsigned int *dest, *buffer;
    int ret = -1;
    unsigned int scaler = qrimg->scaler;

    pQRC = QRcode_encodeString(qrimg->url, qrimg->version, QR_ECLEVEL_L, QR_MODE_8, 1);
    do {
        if (pQRC == NULL) break;

        dst_h = dst_w = pQRC->width * scaler + qrimg->margin * 2;
        src_w = src_h = pQRC->width;

        totalpixels = dst_w * dst_h;
        totalbytes = totalpixels * bits;
        buffer = (unsigned int *)malloc(totalbytes);
        if (! buffer) break;
        
        // clear with backcolor
        dest = buffer;
        for (y = 0; y < totalpixels; y++) {
            (*dest++)= qrimg->backcolor;
        }

        // 每一个像素放大 scaler 倍
        src = pQRC->data;           // data size: width * width
        for (y = 0; y < src_h ; y++) {
            for (x = 0; x < src_w ; x++) {
                offset = (qrimg->margin + y * scaler) * dst_w + (qrimg->margin + x * scaler);  // 定位到放大后的像素位置
                dest = buffer + offset;
                for (r = 0; r < scaler; r++) {// 遍历放大后的像素块
                    for (c = 0; c < scaler ; c++) {
                        if (*src & 1) {
                            *(dest + r * dst_w + c) = qrimg->forecolor;
                        }
                    }
                }
                ++src;
            }
        }
        qrimg->data = (unsigned char *)buffer;
        qrimg->count = totalbytes;
        qrimg->result = totalbytes;
        qrimg->width = dst_w;
        qrimg->height = dst_h;
        ret = totalbytes;

        //save_to_file(dst_w, dst_h, bits, buffer);
        QRcode_free(pQRC);
    } while (0);

    return ret;
}

void FreeQRData(QRImage* qr)
{
    free(qr->data);
}

/*
void main(){
    QRImage qrimg;
    qrimg.backcolor = 0xffffffff;
    qrimg.forecolor = 0xff0000ff;
    qrimg.version = 6;
    qrimg.scaler = 4;
    qrimg.margin = 7;
    strcpy(qrimg.url, "www.baiduasdf.com");

    QREncodeUrl(&qrimg);
}

*/