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

#include "circlewidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QTimer>

CircleWidget::CircleWidget( QWidget *parent ) : QWidget( parent )
{
  r = 0;
  
  timer = new QTimer( this );
  timer->setInterval( 50 );
  
  connect( timer, SIGNAL(timeout()), this, SLOT(timeout()) );
}

QSize CircleWidget::sizeHint() const
{
  return QSize( 200, 200 );
}

void CircleWidget::timeout()
{
  if( r == 0 )
  {
    x = mx;
    y = my;
    
    color = QColor( qrand()%256, qrand()%256, qrand()%256 );
  }

  int dx = mx-x;
  int dy = my-y;

  if( dx*dx+dy*dy <= r*r )
    r++;
  else
    r--;
    
  update();
}

void CircleWidget::paintEvent( QPaintEvent* )
{
  if( r > 0 )
  {
    QPainter painter( this );
    
    painter.setRenderHint( QPainter::Antialiasing );
    
    painter.setPen( color );
    painter.setBrush( color );
    painter.drawEllipse( x-r, y-r, 2*r, 2*r );
  }
}

void CircleWidget::mousePressEvent( QMouseEvent *e )
{
  mx = e->x();
  my = e->y();
  
  timer->start();
}

void CircleWidget::mouseMoveEvent( QMouseEvent *e )
{
  mx = e->x();
  my = e->y();
}

void CircleWidget::mouseReleaseEvent( QMouseEvent *e )
{
  timer->stop();
}
