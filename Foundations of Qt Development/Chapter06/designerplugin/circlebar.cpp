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

#include <QPaintEvent>
#include <QWheelEvent>

#include <QPainter>

#include "circlebar.h"

CircleBar::CircleBar( QWidget *parent ) : QWidget( parent )
{
  m_value = 0;
  
  QSizePolicy policy( QSizePolicy::Preferred, QSizePolicy::Preferred );
  policy.setHeightForWidth( true );
  setSizePolicy( policy );
}

CircleBar::CircleBar( int value, QWidget *parent ) : QWidget( parent )
{
  m_value = value;
  
  QSizePolicy policy( QSizePolicy::Preferred, QSizePolicy::Preferred );
  policy.setHeightForWidth( true );
  setSizePolicy( policy );
}

int CircleBar::heightForWidth( int width ) const
{
  return width;
}

QSize CircleBar::sizeHint() const
{
  return QSize( 100, 100 );
}

int CircleBar::value() const
{
  return m_value;
}

void CircleBar::setValue( int value )
{
  if( value < 0 )
    value = 0;
  
  if( value > 100 )
    value = 100;
    
  if( m_value == value )
    return;
    
  m_value = value;

  update();

  emit valueChanged( m_value );
}

void CircleBar::paintEvent( QPaintEvent *event )
{
  int radius = width()/2;
  double factor = m_value/100.0;
  
  QPainter p( this );
  p.setPen( Qt::black );
  p.drawEllipse( 0, 0, width()-1, width()-1 );
  p.setBrush( Qt::black );
  p.drawEllipse( int(radius*(1.0-factor)), int(radius*(1.0-factor)), int((width()-1)*factor)+1, int((width()-1)*factor)+1 );
}

void CircleBar::wheelEvent( QWheelEvent *event )
{
  event->accept();
  setValue( value() + event->delta()/20 );
}
