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
#include <QGraphicsEllipseItem>

#include "handleitem.h"

int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  
  QGraphicsScene scene( 0, 0, 200, 200 );
  
  QGraphicsRectItem *rectItem = new QGraphicsRectItem( QRect( 10, 10, 50, 100 ), 0, &scene );
  QGraphicsEllipseItem *elItem = new QGraphicsEllipseItem( QRect( 80, 40, 100, 80 ), 0, &scene );
  
  HandleItem *trh = new HandleItem( rectItem, &scene, Qt::red, HandleItem::TopHandle );
  HandleItem *rrh = new HandleItem( rectItem, &scene, Qt::red, HandleItem::RightHandle );
  HandleItem *crh = new HandleItem( rectItem, &scene, Qt::red, HandleItem::CenterHandle, QList<HandleItem*>() << trh << rrh );
  
  HandleItem *teh = new HandleItem( elItem, &scene, Qt::green, HandleItem::TopHandle );
  HandleItem *reh = new HandleItem( elItem, &scene, Qt::green, HandleItem::RightHandle );
  HandleItem *ceh = new HandleItem( elItem, &scene, Qt::green, HandleItem::CenterHandle, QList<HandleItem*>() << teh << reh );

  QGraphicsView view;
  view.setScene( &scene );
  view.show();
  
  int res = app.exec();
  
  delete crh;
  delete trh;
  delete rrh;
    
  delete ceh;
  delete teh;
  delete reh;
  
  return res;
}
