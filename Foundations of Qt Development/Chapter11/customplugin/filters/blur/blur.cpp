/*
 * Copyright (c) 2006-2007, Johan Thelin
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, 
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,  
 *       this list of conditions and the following disclaimer in the documentation 
 *       and/or other materials provided with the distribution.
 *     * Neither the name of APress nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <QtPlugin>

#include "blur.h"

QString Blur::name() const
{
  return "Blur";
}

QImage Blur::filter( const QImage &image ) const
{
  QImage result( image.width(), image.height(), image.format() );
  
  for( int y=0; y<image.height(); ++y )
    for( int x=0; x<image.width(); ++x )
    {
      QRgb pixel;
      int r, g, b;
      r = g = b = 0;
      
      pixel = getSafePixel( image, x-1, y );
      r += qRed( pixel );
      g += qGreen( pixel );
      b += qBlue( pixel );
      
      pixel = getSafePixel( image, x, y-1 );
      r += qRed( pixel );
      g += qGreen( pixel );
      b += qBlue( pixel );
      
      pixel = getSafePixel( image, x, y );
      r += qRed( pixel );
      g += qGreen( pixel );
      b += qBlue( pixel );
      
      pixel = getSafePixel( image, x+1, y );
      r += qRed( pixel );
      g += qGreen( pixel );
      b += qBlue( pixel );
      
      pixel = getSafePixel( image, x, y+1 );
      r += qRed( pixel );
      g += qGreen( pixel );
      b += qBlue( pixel );
      
      result.setPixel( x, y, qRgb( r/5, g/5, b/5 ) );
    }
      
  return result;
}

QRgb Blur::getSafePixel( const QImage &image, int x, int y ) const
{
  if( x < 0 )
    x = 0;
  if( x >= image.width() )
    x = image.width()-1;
    
  if( y < 0 )
    y = 0;
  if( y >= image.height() )
    y = image.height()-1;
    
  return image.pixel( x, y );
}

Q_EXPORT_PLUGIN2( blur, Blur )
