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

#include <QScrollBar>
#include <QGridLayout>
#include <QLabel>

#include "singleitemview.h"

SingleItemView::SingleItemView( QWidget *parent ) : QAbstractItemView( parent )
{
  QGridLayout *layout = new QGridLayout( this->viewport() );
  label = new QLabel();

  layout->addWidget( label, 0, 0 );
  
  label->setAlignment( Qt::AlignCenter );
  label->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
  label->setText( tr("<i>No data.</i>") );
}
 
QRect SingleItemView::visualRect( const QModelIndex &index ) const
{
  if( selectionModel()->selection().indexes().count() != 1 )
    return QRect();
    
  if( currentIndex() != index )
    return QRect();
    
  return rect();  
}

QRegion SingleItemView::visualRegionForSelection( const QItemSelection &selection ) const
{
  if( selection.indexes().count() != 1 )
    return QRect();
    
  if( currentIndex() != selection.indexes()[0] )
    return QRect();
    
  return rect();
}

bool SingleItemView::isIndexHidden( const QModelIndex &index ) const
{
  if( selectionModel()->selection().indexes().count() != 1 )
    return true;
    
  if( currentIndex() != index )
    return true;
    
  return false;
}

QModelIndex SingleItemView::indexAt( const QPoint &point ) const
{
  if( selectionModel()->selection().indexes().count() != 1 )
    return QModelIndex();
    
  return currentIndex();  
}

int SingleItemView::horizontalOffset() const
{
  return horizontalScrollBar()->value();
}

int SingleItemView::verticalOffset() const
{
  return verticalScrollBar()->value();
}

QModelIndex SingleItemView::moveCursor( CursorAction cursorAction, Qt::KeyboardModifiers modifiers )
{
  return currentIndex();
}

void SingleItemView::setSelection( const QRect &rect, QItemSelectionModel::SelectionFlags flags )
{
  // do nothing
}

void SingleItemView::scrollTo( const QModelIndex &index, ScrollHint hint )
{
  // cannot scroll
}

void SingleItemView::dataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight )
{
  updateText();  
}

void SingleItemView::selectionChanged( const QItemSelection &selected, const QItemSelection &deselected )
{
  updateText();
}

void SingleItemView::updateText()
{
  switch( selectionModel()->selection().indexes().count() )
  {
    case 0:
      label->setText( tr("<i>No data.</i>") );
      break;
      
    case 1:
      label->setText( model()->data( currentIndex() ).toString() );
      break;
      
    default:
      label->setText( tr("<i>Too many items selected.<br>Can only show one item at a time.</i>") );
      break;
  }
}
