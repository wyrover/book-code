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

#include "handleitem.h"

#include <QPainter>
#include <QPointF>

HandleItem::HandleItem( QGraphicsItem *item, QGraphicsScene *scene, QColor color, HandleItem::HandleRole role, QList<HandleItem*> handles ) : QGraphicsItem( 0, scene )
{
  m_role = role;
  m_color = color;
  
  m_item = item;
  m_handles = handles;
  
  m_pressed = false;
  setZValue( 100 );

  setFlag( ItemIsMovable );
}

void HandleItem::paint( QPainter *paint, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  paint->setPen( m_color );
  paint->setBrush( m_color );
    
  QRectF rect = boundingRect();
  QVector<QPointF> points;
    
  switch( m_role )
  {
  case CenterHandle:
    paint->drawEllipse( rect );
    break;
  case RightHandle:
    points << rect.center()+QPointF(3,0) << rect.center()+QPointF(-3,-5) << rect.center()+QPointF(-3,5);
    paint->drawConvexPolygon( QPolygonF(points) );
    break;
  case TopHandle:
    points << rect.center()+QPointF(0,-3) << rect.center()+QPointF(-5,3) << rect.center()+QPointF(5,3);
    paint->drawConvexPolygon( QPolygonF(points) );
    break;
  }
}

QRectF HandleItem::boundingRect() const
{
  QPointF point = m_item->boundingRect().center();

  switch( m_role )
  {
  case CenterHandle:
    return QRectF( point-QPointF(5, 5), QSize( 10, 10 ) );
  case RightHandle:
    point.setX( m_item->boundingRect().right() );
    return QRectF( point-QPointF(3, 5), QSize( 6, 10 ) );
  case TopHandle:
    point.setY( m_item->boundingRect().top() );
    return QRectF( point-QPointF(5, 3), QSize( 10, 6 ) );
  }
  
  return QRectF();
}

QVariant HandleItem::itemChange( GraphicsItemChange change, const QVariant &data )
{
  if( change == ItemPositionChange && m_pressed )
  {
    QPointF movement = data.toPoint() - pos();
    QPointF center = m_item->boundingRect().center();
    
    switch( m_role )
    {
    case CenterHandle:
      m_item->moveBy( movement.x(), movement.y() );
      
      foreach( HandleItem *handle, m_handles )
        handle->translate( movement.x(), movement.y() );

      break;
    case RightHandle:
      if( 2*movement.x() + m_item->sceneBoundingRect().width() <= 5 )
        return QGraphicsItem::itemChange( change, pos() );
    
      movement.setY( 0 );
      
      m_item->translate( center.x(), center.y() );      
      m_item->scale( 1.0+2.0*movement.x()/(m_item->sceneBoundingRect().width()), 1 );
      m_item->translate( -center.x(), -center.y() );
        
      break;
    case TopHandle:
      if( -2*movement.y() + m_item->sceneBoundingRect().height() <= 5 )
        return QGraphicsItem::itemChange( change, pos() );
    
      movement.setX( 0 );
      
      m_item->translate( center.x(), center.y() );
      m_item->scale( 1, 1.0-2.0*movement.y()/(m_item->sceneBoundingRect().height()) );
      m_item->translate( -center.x(), -center.y() );
      break;
    }
    
    return QGraphicsItem::itemChange( change, pos()+movement );
  }
  
  return QGraphicsItem::itemChange( change, data );
}

void HandleItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  m_pressed = true;  
  QGraphicsItem::mousePressEvent( event );
}

void HandleItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  m_pressed = false;
  QGraphicsItem::mouseReleaseEvent( event );
}
