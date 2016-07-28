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

#include "mulmodel.h"

MulModel::MulModel( int rows, int columns, QObject *parent ) : QAbstractTableModel( parent )
{
  m_rows = rows;
  m_columns = columns;
}

int MulModel::rowCount( const QModelIndex &parent ) const
{
  return m_rows;
}

int MulModel::columnCount( const QModelIndex &parent ) const
{
  return m_columns;
}

Qt::ItemFlags MulModel::flags( const QModelIndex &index ) const
{
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant MulModel::data( const QModelIndex &index, int role ) const
{
  switch( role )
  {
  case Qt::DisplayRole:
    return (index.row()+1) * (index.column()+1);
  
  case Qt::ToolTipRole:
    return QString( "%1 x %2" ).arg( index.row()+1 ).arg( index.column()+1 );
  
  default:
    return QVariant();
  }
}

QVariant MulModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if( role != Qt::DisplayRole )
    return QVariant();
    
  return QString::number( section+1 );
}
