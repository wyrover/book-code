/*
 * qrencode - QR Code encoder
 *
 * Input data splitter.
 * Copyright (C) 2006-2011 Kentaro Fukuchi <kentaro@fukuchi.org>
 *
 * The following data / specifications are taken from
 * "Two dimensional symbol -- QR-code -- Basic Specification" (JIS X0510:2004)
 *  or
 * "Automatic identification and data capture techniques -- 
 *  QR Code 2005 bar code symbology specification" (ISO/IEC 18004:2006)
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

#ifndef __LIBQR_H__
#define __LIBQR_H__

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
    __out int result;
    __in unsigned int version;         ///< version of the symbol
    __out unsigned int width;           ///< width of the symbol
    __out unsigned int height;           ///< width of the symbol
    __in unsigned int margin;
    __out unsigned char *data; ///< symbol data
    __out unsigned int count;
    __in unsigned int scaler;
    __in unsigned int backcolor;
    __in unsigned int forecolor;
    char url[1000];
} QRImage;

void save_to_file(unsigned int w, unsigned int h, unsigned int bits, unsigned int *buffer);
int QREncodeUrl(QRImage* qrimg);
void FreeQRData(QRImage* qr);

#if defined(__cplusplus)
}
#endif

#endif /* __LIBQR_H__ */
