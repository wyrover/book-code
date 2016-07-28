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

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalMapper>

#include "numerickeypad.h"

NumericKeypad::NumericKeypad( QWidget *parent )
{
  QGridLayout *layout = new QGridLayout( this );
  
  m_lineEdit = new QLineEdit();
  m_lineEdit->setAlignment( Qt::AlignRight );
  
  QPushButton *button0 = new QPushButton( tr("0") );
  QPushButton *button1 = new QPushButton( tr("1") );
  QPushButton *button2 = new QPushButton( tr("2") );
  QPushButton *button3 = new QPushButton( tr("3") );
  QPushButton *button4 = new QPushButton( tr("4") );
  QPushButton *button5 = new QPushButton( tr("5") );
  QPushButton *button6 = new QPushButton( tr("6") );
  QPushButton *button7 = new QPushButton( tr("7") );
  QPushButton *button8 = new QPushButton( tr("8") );
  QPushButton *button9 = new QPushButton( tr("9") );
  QPushButton *buttonDot = new QPushButton( tr(".") );
  QPushButton *buttonClear = new QPushButton( tr("C") );
  
  layout->addWidget( m_lineEdit, 0, 0, 1, 3 );
  
  layout->addWidget( button1, 1, 0 );
  layout->addWidget( button2, 1, 1 );
  layout->addWidget( button3, 1, 2 );
  layout->addWidget( button4, 2, 0 );
  layout->addWidget( button5, 2, 1 );
  layout->addWidget( button6, 2, 2 );
  layout->addWidget( button7, 3, 0 );
  layout->addWidget( button8, 3, 1 );
  layout->addWidget( button9, 3, 2 );
  layout->addWidget( button0, 4, 0 );
  layout->addWidget( buttonDot, 4, 1 );
  layout->addWidget( buttonClear, 4, 2 );
  
  QSignalMapper *mapper = new QSignalMapper( this );
  
  mapper->setMapping( button0, "0" );
  mapper->setMapping( button1, "1" );
  mapper->setMapping( button2, "2" );
  mapper->setMapping( button3, "3" );
  mapper->setMapping( button4, "4" );
  mapper->setMapping( button5, "5" );
  mapper->setMapping( button6, "6" );
  mapper->setMapping( button7, "7" );
  mapper->setMapping( button8, "8" );
  mapper->setMapping( button9, "9" );
  mapper->setMapping( buttonDot, "." );
  
  connect( button0, SIGNAL(clicked()), mapper, SLOT(map()) );
  connect( button1, SIGNAL(clicked()), mapper, SLOT(map()) );
  connect( button2, SIGNAL(clicked()), mapper, SLOT(map()) );
  connect( button3, SIGNAL(clicked()), mapper, SLOT(map()) );
  connect( button4, SIGNAL(clicked()), mapper, SLOT(map()) );
  connect( button5, SIGNAL(clicked()), mapper, SLOT(map()) );
  connect( button6, SIGNAL(clicked()), mapper, SLOT(map()) );
  connect( button7, SIGNAL(clicked()), mapper, SLOT(map()) );
  connect( button8, SIGNAL(clicked()), mapper, SLOT(map()) );
  connect( button9, SIGNAL(clicked()), mapper, SLOT(map()) );
  connect( buttonDot, SIGNAL(clicked()), mapper, SLOT(map()) );
  
  connect( mapper, SIGNAL(mapped(QString)), this, SLOT(buttonClicked(QString)) );
  
  connect( buttonClear, SIGNAL(clicked()), m_lineEdit, SLOT(clear()) );
  connect( m_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(setText(QString)) );
}

const QString NumericKeypad::text() const
{
  return m_text;
}

void NumericKeypad::buttonClicked( const QString &newText )
{
  setText( m_text + newText );
}

void NumericKeypad::setText( const QString &newText )
{
  if( newText == m_text )
    return;
    
  m_text = newText;
  m_lineEdit->setText( m_text );
  
  emit textChanged( m_text );
}

