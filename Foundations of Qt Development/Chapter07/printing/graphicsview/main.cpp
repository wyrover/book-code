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

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>

#include <QPrinter>
#include <QPrintDialog>

#include <QPainter>

int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  
  QGraphicsScene scene( QRect( -50, -50, 400, 200 ) );
  
  QGraphicsRectItem *rectItem = new QGraphicsRectItem( QRect( -25, 25, 200, 40 ), 0, &scene );
  rectItem->setPen( QPen( Qt::red, 3, Qt::DashDotLine ) );
  rectItem->setBrush( Qt::gray );
    
  QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem( "Foundations of Qt", 0, &scene );
  textItem->setPos( 50, 0 );
  
  QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem( QRect( 170, 20, 100, 75 ), 0, &scene );
  ellipseItem->setPen( QPen(Qt::darkBlue) );
  ellipseItem->setBrush( Qt::blue );
    
  QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem( QPolygonF( QVector<QPointF>() << QPointF( 10, 10 ) << QPointF( 0, 90 ) << QPointF( 40, 70 ) << QPointF( 80, 110 ) << QPointF( 70, 20 ) ), 0, &scene );
  polygonItem->setPen( QPen(Qt::darkGreen) );
  polygonItem->setBrush( Qt::yellow );
  
  QPrinter printer;
  QPrintDialog dlg( &printer );
  if( dlg.exec() == QDialog::Accepted )
  {
    QPainter painter( &printer );

    scene.render( &painter, printer.pageRect(), scene.sceneRect(), Qt::KeepAspectRatio );
  }

  return 0;
}
