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

#include "listwidgetdialog.h"

#include <QListWidget>
#include <QPushButton>

#include <QGridLayout>

ListWidgetDialog::ListWidgetDialog() : QDialog(0)
{
  QPushButton *left, *right;

  QGridLayout *layout = new QGridLayout( this );
  layout->addWidget( left = new QPushButton( "<<" ), 0, 1 );
  layout->addWidget( right = new QPushButton( ">>" ), 1, 1 );
  layout->addWidget( leftList = new QListWidget(), 0, 0, 3, 1 );
  layout->addWidget( rightList = new QListWidget(), 0, 2, 3, 1 );
  
  connect( left, SIGNAL(clicked()), this, SLOT(moveLeft()) );
  connect( right, SIGNAL(clicked()), this, SLOT(moveRight()) );

  QStringList items;
  items << "Argentine" << "Brazilian" << "South African" 
        << "USA West" << "Monaco" << "Belgian" << "Spanish" 
        << "Swedish" << "French" << "British" << "German" 
        << "Austrian" << "Dutch" << "Italian" << "USA East" 
        << "Canadian";  
  leftList->addItems( items );
}
  
void ListWidgetDialog::moveLeft()
{
  if( rightList->selectedItems().count() != 1 )
    return;
    
  QListWidgetItem *item = rightList->takeItem( rightList->currentRow() );
  leftList->addItem( item );
}

void ListWidgetDialog::moveRight()
{
  if( leftList->selectedItems().count() != 1 )
    return;
    
  QListWidgetItem *item = leftList->takeItem( leftList->currentRow() );
  rightList->addItem( item );
}
