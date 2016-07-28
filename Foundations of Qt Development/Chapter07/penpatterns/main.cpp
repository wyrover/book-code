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
  QPixmap pixmap( 200, 100 );
  pixmap.fill( Qt::white );

  QPainter painter( &pixmap );
  
  QPen pen( Qt::black );
    
  pen.setStyle( Qt::SolidLine );  
  painter.setPen( pen );
  painter.drawLine( QPoint( 10, 10 ), QPoint( 190, 10 ) );
  
  pen.setStyle( Qt::DashDotLine );
  painter.setPen( pen );
  painter.drawLine( QPoint( 10, 50 ), QPoint( 190, 50 ) );
  
  pen.setDashPattern( QVector<qreal>() << 1 << 1 << 1 << 1 << 2 << 2 << 2 << 2 << 4 << 4 << 4 << 4 << 8 << 8 << 8 << 8 );
  pen.setStyle( Qt::CustomDashLine );
  painter.setPen( pen );
  painter.drawLine( QPoint( 10, 90 ), QPoint( 190, 90 ) );

  pixmap.save( "penpatterns.png" );

  return 0;
}