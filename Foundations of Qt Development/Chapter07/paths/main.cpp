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

#include <QApplication>

#include <QPainter>
#include <QPainterPath>
#include <QPixmap>

int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  
  QPixmap pixmap( 200, 200 );
  pixmap.fill( Qt::white );
    
  QPainterPath path;
  
  path.addEllipse( 80, 80, 80, 80 );
  
  path.moveTo( 120, 120 );
  path.lineTo( 120, 40 );
  path.arcTo( 40, 40, 160, 160, 90, 90 );
  path.lineTo( 120, 120 );
  
  QFont font = QApplication::font();
  font.setPixelSize( 40 );
  
  path.addText( 20, 180, font, "Path" );
  
  QPainter painter( &pixmap );
  painter.setRenderHint( QPainter::Antialiasing );
  
  painter.setPen( Qt::black );
  painter.setBrush( Qt::gray );
  
  painter.drawPath( path );
  
  pixmap.save( "path.png" );
  
  return 0;
}
