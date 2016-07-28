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
#include <QGLWidget>

QGraphicsItem *createItem( int x, QGraphicsScene *scene )
{
  QGraphicsRectItem *rectItem = new QGraphicsRectItem( QRect( x+40, 40, 120, 120 ), 0, scene );
  rectItem->setPen( QPen(Qt::black) );
  rectItem->setBrush( Qt::gray );
    
  QGraphicsRectItem *innerRectItem = new QGraphicsRectItem( QRect( x+50, 50, 45, 100 ), rectItem, scene );
  innerRectItem->setPen( QPen(Qt::black) );
  innerRectItem->setBrush( Qt::white );
  
  QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem( QRect( x+105, 50, 45, 100 ), rectItem, scene );
  ellipseItem->setPen( QPen(Qt::black) );
  ellipseItem->setBrush( Qt::white );

  return rectItem;
}

int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  
  QGraphicsScene scene( QRect( 0, 00, 1000, 200 ) );
  
  QGraphicsItem *item1 = createItem( 0, &scene );
  
  QGraphicsItem *item2 = createItem( 200, &scene );
  item2->translate( 300, 100 );
  item2->rotate( 30 );
  item2->translate( -300, -100 );
  
  QGraphicsItem *item3 = createItem( 400, &scene );
  item3->translate( 500, 100 );
  item3->scale( 0.5, 0.7 );
  item3->translate( -500, -100 );
  
  QGraphicsItem *item4 = createItem( 600, &scene );
  item4->translate( 700, 100 );
  item4->shear( 0.1, 0.3 );
  item4->translate( -700, -100 );
  
  QGraphicsItem *item5 = createItem( 800, &scene );
  item5->translate( 900, 100 );
  item5->scale( 0.5, 0.7 );
  item5->rotate( 30 );
  item5->shear( 0.1, 0.3 );
  item5->translate( -900, -100 );

  QGraphicsView view;
  view.setScene( &scene );
  view.setViewport( new QGLWidget() );
  view.show();
  
  return app.exec();
}
