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
#include <QPixmap>
#include <QPainter>

int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  QPixmap pixmap( 200, 190 );
  pixmap.fill( Qt::white );

  QPainter painter( &pixmap );
  painter.setRenderHint( QPainter::Antialiasing );
  painter.setPen( Qt::black );
    
  painter.drawEllipse( 10, 10, 10, 80 );
  painter.drawEllipse( 30, 10, 20, 80 );
  painter.drawEllipse( 60, 10, 40, 80 );
  painter.drawEllipse( QRect( 110, 10, 80, 80 ) );

  painter.drawArc( 10, 100, 10, 80, 30*16, 240*16 );
  painter.drawArc( 30, 100, 20, 80, 45*16, 200*16 );
  painter.drawArc( 60, 100, 40, 80, 60*16, 160*16 );
  painter.drawArc( QRect( 110, 100, 80, 80 ), 75*16, 120*16 );

  pixmap.save( "circles.png" );

  return 0;
}
