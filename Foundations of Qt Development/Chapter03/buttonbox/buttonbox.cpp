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

#include "ButtonBox.h"

#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>

#include <QVBoxLayout>

#include <QMessageBox>

ButtonBoxDialog::ButtonBoxDialog() : QDialog(0)
{
  QVBoxLayout *layout = new QVBoxLayout( this );
  
  QPushButton *button;
  QDialogButtonBox *box = new QDialogButtonBox( Qt::Horizontal );
  
  button = new QPushButton( "Ok" );
  connect( button, SIGNAL(clicked()), this, SLOT(okClicked()) );
  box->addButton( button, QDialogButtonBox::AcceptRole );

  button = new QPushButton( "Cancel" );
  connect( button, SIGNAL(clicked()), this, SLOT(cancelClicked()) );
  box->addButton( button, QDialogButtonBox::RejectRole );
  
  button = new QPushButton( "Apply" );
  connect( button, SIGNAL(clicked()), this, SLOT(applyClicked()) );
  box->addButton( button, QDialogButtonBox::ApplyRole );
  
  button = new QPushButton( "Reset" );
  connect( button, SIGNAL(clicked()), this, SLOT(resetClicked()) );
  box->addButton( button, QDialogButtonBox::ResetRole );
  
  button = new QPushButton( "Help" );
  connect( button, SIGNAL(clicked()), this, SLOT(helpClicked()) );
  box->addButton( button, QDialogButtonBox::HelpRole );
    
  layout->addWidget( new QLabel( "Try out the buttons!" ) );
  layout->addWidget( box );
}

void ButtonBoxDialog::okClicked()
{
  QMessageBox::information( this, "Demo", "Ok Clicked" );
}

void ButtonBoxDialog::cancelClicked()
{
  QMessageBox::information( this, "Demo", "Cancel Clicked" );
}

void ButtonBoxDialog::applyClicked()
{
  QMessageBox::information( this, "Demo", "Apply Clicked" );
}

void ButtonBoxDialog::resetClicked()
{
  QMessageBox::information( this, "Demo", "Reset Clicked" );
}

void ButtonBoxDialog::helpClicked()
{
  QMessageBox::information( this, "Demo", "Help Clicked" );
}
