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
#include <QSlider>
#include <QModelIndex>

#include "bardelegate.h"

BarDelegate::BarDelegate( QObject *parent ) : QAbstractItemDelegate( parent ) { }

void BarDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  if( option.state & QStyle::State_Selected )
    painter->fillRect( option.rect, option.palette.highlight() );

  int value = index.model()->data( index, Qt::DisplayRole ).toInt();
  double factor = double(value)/100.0;
  
  painter->save();

  if( factor > 1 )
  {
    painter->setBrush( Qt::red );
    factor = 1;
  }
  else
    painter->setBrush( QColor( 0, int(factor*255), 255-int(factor*255) ) );

  painter->setPen( Qt::black );
  painter->drawRect( option.rect.x()+2, option.rect.y()+2, int(factor*(option.rect.width()-5)), option.rect.height()-5 );
  painter->restore();
}

QSize BarDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  return QSize( 45, 15 ); 
}

QWidget *BarDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  QSlider *slider = new QSlider( parent );
  
  slider->setAutoFillBackground( true );
  slider->setOrientation( Qt::Horizontal );
  slider->setRange( 0, 100 );
  slider->installEventFilter( const_cast<BarDelegate*>(this) );
  
  return slider;
}

void BarDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  editor->setGeometry( option.rect );
}

void BarDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
  int value = index.model()->data( index, Qt::DisplayRole ).toInt();
  static_cast<QSlider*>( editor )->setValue( value );
}

void BarDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
  model->setData( index, static_cast<QSlider*>( editor )->value() );
}

