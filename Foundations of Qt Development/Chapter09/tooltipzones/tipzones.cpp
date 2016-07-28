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

#include <QPainter>

#include <QPaintEvent>
#include <QEvent>
#include <QHelpEvent>

#include "tipzones.h"

TipZones::TipZones() : QWidget()
{
}

void TipZones::paintEvent( QPaintEvent* )
{
  QRect redRect, greenRect, blueRect, yellowRect;
  
  redRect = QRect( 0, 0, width()/2, height()/2 );
  greenRect = QRect( width()/2, 0, width()/2, height()/2 );
  blueRect = QRect( 0, height()/2, width()/2, height()/2 );
  yellowRect = QRect( width()/2, height()/2, width()/2, height()/2 );
  
  QPainter p( this );
  
  p.setPen( Qt::black );
  
  p.setBrush( Qt::red );
  p.drawRect( redRect );
  
  p.setBrush( Qt::green );
  p.drawRect( greenRect );

  p.setBrush( Qt::blue );
  p.drawRect( blueRect );

  p.setBrush( Qt::yellow );
  p.drawRect( yellowRect );
}
  
bool TipZones::event( QEvent *event )
{
  if( event->type() == QEvent::ToolTip )
  {
    QHelpEvent *helpEvent = static_cast<QHelpEvent*>( event );
    
    QRect redRect, greenRect, blueRect, yellowRect;
  
    redRect = QRect( 0, 0, width()/2, height()/2 );
    greenRect = QRect( width()/2, 0, width()/2, height()/2 );
    blueRect = QRect( 0, height()/2, width()/2, height()/2 );
    yellowRect = QRect( width()/2, height()/2, width()/2, height()/2 );

    if( redRect.contains( helpEvent->pos() ) )
      setToolTip( tr("Red") );
    else if( greenRect.contains( helpEvent->pos() ) )
      setToolTip( tr("Green") );
    else if( blueRect.contains( helpEvent->pos() ) )
      setToolTip( tr("Blue") );
    else 
      setToolTip( tr("Yellow") );
  }

  return QWidget::event( event );  
}
