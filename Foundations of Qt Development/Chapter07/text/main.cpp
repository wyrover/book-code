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
#include <QTextDocument>

int main( int argc, char **argv )
{
  QApplication app( argc, argv );

  QPixmap pixmap( 200, 330 );
  pixmap.fill( Qt::white );

  QPainter painter( &pixmap );
  painter.setPen( Qt::black );
    
  QPoint point = QPoint( 10, 20 );
  painter.drawText( point, "You can draw text from a point..." );
  painter.drawLine( point+QPoint(-5, 0), point+QPoint(5, 0) );
  painter.drawLine( point+QPoint(0, -5), point+QPoint(0, 5) );
  
  QRect rect = QRect(10, 30, 180, 20);
  painter.drawText( rect, Qt::AlignCenter, 
                    "...or you can draw it inside a rectangle." );
  painter.drawRect( rect );
  
  rect.translate( 0, 30 );
  
  QFont font = QApplication::font();
  font.setPixelSize( rect.height() );
  painter.setFont( font );
  
  painter.drawText( rect, Qt::AlignRight, "Right." );
  painter.drawText( rect, Qt::AlignLeft, "Left." );
  painter.drawRect( rect );
  
  rect.translate( 0, rect.height()+10 );
  rect.setHeight( QFontMetrics( font ).height() );

  painter.drawText( rect, Qt::AlignRight, "Right." );
  painter.drawText( rect, Qt::AlignLeft, "Left." );
  painter.drawRect( rect );
  
  QTextDocument doc;
  doc.setHtml( "<p>A QTextDocument can be used to present formatted text "
               "in a nice way.</p>"
               "<p align=center>It can be <b>formatted</b> "
               "<font size=+2>in</font> <i>different</i> ways.</p>"
               "<p>The text can be really long and contain many "
               "paragraphs. It is properly wrapped and such...</p>" );

  rect.translate( 0, rect.height()+10 );
  rect.setHeight( 160 );
  doc.setTextWidth( rect.width() );
  painter.save();
  painter.translate( rect.topLeft() );
  doc.drawContents( &painter, rect.translated( -rect.topLeft() ) );
  painter.restore();
  painter.drawRect( rect );

  rect.translate( 0, 160 );
  rect.setHeight( doc.size().height()-160 );
  painter.setBrush( Qt::gray );
  painter.drawRect( rect );

  pixmap.save( "text.png" );

  return 0;
}
