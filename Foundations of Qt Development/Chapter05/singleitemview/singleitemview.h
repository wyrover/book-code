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

#ifndef SINGLEITEMVIEW_H
#define SINGLEITEMVIEW_H

#include <QAbstractItemView>

class QLabel;

class SingleItemView : public QAbstractItemView
{
  Q_OBJECT
  
public:
  SingleItemView( QWidget *parent = 0 );
  
  QModelIndex indexAt( const QPoint &point ) const;
  void scrollTo( const QModelIndex &index, ScrollHint hint = EnsureVisible );
  QRect visualRect( const QModelIndex &index ) const;

protected:
  int horizontalOffset() const;
  bool isIndexHidden( const QModelIndex &index ) const;
  QModelIndex moveCursor( CursorAction cursorAction, Qt::KeyboardModifiers modifiers );
  void setSelection( const QRect &rect, QItemSelectionModel::SelectionFlags flags );
  int verticalOffset() const;
  QRegion visualRegionForSelection( const QItemSelection &selection ) const;
    
protected slots:
  void dataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight );
  void selectionChanged( const QItemSelection &selected, const QItemSelection &deselected );
  
private:
  void updateText();
  
  QLabel *label;
};

#endif // SINGLEITEMVIEW_H
