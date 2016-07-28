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

#include "mybutton.h"

#include <QStyle>
#include <QStyleOptionButton>
#include <QPainter>

MyButton::MyButton( QWidget *parent ) : QAbstractButton( parent )
{
}

QSize MyButton::sizeHint() const
{
  return QSize( fontMetrics().width( text() )+10, fontMetrics().height()+10 );
}

void MyButton::paintEvent( QPaintEvent* )
{
  QPainter painter( this );
  
  QStyleOptionButton option;
  option.init( this );
  if( isDown() )
    option.state |= QStyle::State_Sunken;
  else if( isChecked() )
    option.state |= QStyle::State_On;
    
  style()->drawControl( QStyle::CE_PushButtonBevel, &option, &painter, this );
    
  painter.setFont( font() );
  
  if( !isEnabled() )
    painter.setPen( Qt::darkGray );
  else if( isDown() )
    painter.setPen( Qt::red );
  else
    painter.setPen( Qt::darkRed );

  painter.drawText( rect(), Qt::AlignCenter, text() );
}
