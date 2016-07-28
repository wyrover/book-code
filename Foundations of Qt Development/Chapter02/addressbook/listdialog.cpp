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

#include <QStringList>

#include "listdialog.h"
#include "editdialog.h"

ListDialog::ListDialog() : QDialog()
{
  ui.setupUi( this );
  
  connect( ui.addButton, SIGNAL(clicked()), this, SLOT(addItem()) );
  connect( ui.editButton, SIGNAL(clicked()), this, SLOT(editItem()) );
  connect( ui.deleteButton, SIGNAL(clicked()), this, SLOT(deleteItem()) );
}

void ListDialog::addItem()
{
  EditDialog dlg( this );
  
  if( dlg.exec() == Accepted )
    ui.list->addItem( dlg.name() + " -- " + dlg.number() );
}

void ListDialog::editItem()
{
  if( !ui.list->currentItem() )
    return;
  
  QStringList parts = ui.list->currentItem()->text().split( "--" );
  
  EditDialog dlg( this );
  dlg.setName( parts[0].trimmed() );
  dlg.setNumber( parts[1].trimmed() );
  
  if( dlg.exec() == Accepted )
    ui.list->currentItem()->setText( dlg.name() + " -- " + dlg.number() );
}

void ListDialog::deleteItem()
{
  delete ui.list->currentItem();
}
