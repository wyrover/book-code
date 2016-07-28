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

#include <QMetaObject>

#include "objecttreemodel.h"

ObjectTreeModel::ObjectTreeModel( QObject *root, QObject *parent ) : QAbstractItemModel( parent )
{
  m_root = root;
}

Qt::ItemFlags ObjectTreeModel::flags(const QModelIndex &index) const
{
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ObjectTreeModel::data( const QModelIndex &index, int role) const
{
  if( !index.isValid() )
    return QVariant();
    
  if( role == Qt::DisplayRole )
  {
    switch( index.column() )
    {
    case 0:
      return static_cast<QObject*>( index.internalPointer() )->objectName();
      
    case 1:
      return static_cast<QObject*>( index.internalPointer() )->metaObject()->className();
      
    default:
      break;
    }
  }
  else if( role == Qt::ToolTipRole )
  {
    switch( index.column() )
    {
    case 0:
      return QString( "The name of the object." );
      
    case 1:
      return QString( "The name of the class." );
      
    default:
      break;
    }
  }

  return QVariant();
}

QVariant ObjectTreeModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
  if( role != Qt::DisplayRole || orientation != Qt::Horizontal )
    return QVariant();
    
  switch( section )
  {
  case 0:
    return QString( "Object" );
    
  case 1:
    return QString( "Class" );
    
  default:
    return QVariant();
  }
}

int ObjectTreeModel::rowCount(const QModelIndex &parent ) const
{
  QObject *parentObject;
  
  if( !parent.isValid() )
    parentObject = m_root;
  else
    parentObject = static_cast<QObject*>( parent.internalPointer() );
    
  return parentObject->children().count();
}

int ObjectTreeModel::columnCount(const QModelIndex &parent ) const
{
  return 2;
}

QModelIndex ObjectTreeModel::index(int row, int column, const QModelIndex &parent ) const
{
  QObject *parentObject;
  
  if( !parent.isValid() )
    parentObject = m_root;
  else
    parentObject = static_cast<QObject*>( parent.internalPointer() );
  
  if( row < parentObject->children().count() )
    return createIndex( row, column, parentObject->children().at( row ) );
  else
    return QModelIndex();
}

QModelIndex ObjectTreeModel::parent(const QModelIndex &index) const
{
  if( !index.isValid() )
    return QModelIndex();
    
  QObject *indexObject = static_cast<QObject*>( index.internalPointer() );
  QObject *parentObject = indexObject->parent();
  
  if( parentObject == m_root )
    return QModelIndex();
    
  QObject *grandParentObject = parentObject->parent();
  
  return createIndex( grandParentObject->children().indexOf( parentObject ), 0, parentObject );
}
