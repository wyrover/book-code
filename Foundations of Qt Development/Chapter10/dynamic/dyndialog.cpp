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

#include <QVBoxLayout>

#include <QGroupBox>
#include <QRadioButton>

#include <QEvent>

#include <QTranslator>

#include "dyndialog.h"

extern QTranslator *qTranslator;

DynDialog::DynDialog() : QDialog( 0 )
{
  languages = new QGroupBox( this );
  english = new QRadioButton( this );
  swedish = new QRadioButton( this );
  
  english->setChecked( true );
  qTranslator->load( "english" );
  
  QVBoxLayout *baseLayout = new QVBoxLayout( this );
  baseLayout->addWidget( languages );
  
  QVBoxLayout *radioLayout = new QVBoxLayout( languages );
  radioLayout->addWidget( english );
  radioLayout->addWidget( swedish );
  
  connect( english, SIGNAL(toggled(bool)), this, SLOT(languageChanged()) );
  connect( swedish, SIGNAL(toggled(bool)), this, SLOT(languageChanged()) );
  
  translateUi();
}
  
void DynDialog::changeEvent( QEvent *event )
{
  if( event->type() == QEvent::LanguageChange )
  {
    translateUi();
  }
  else
    QDialog::changeEvent( event );
}

void DynDialog::languageChanged()
{
  if( english->isChecked() )
    qTranslator->load( "english" );
  else
    qTranslator->load( "swedish" );
}

void DynDialog::translateUi()
{
  languages->setTitle( tr("Languages") );
  
  english->setText( tr("English") );
  swedish->setText( tr("Swedish") );
}
