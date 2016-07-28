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

#include <QWidget>
#include <QCheckBox>
#include <QTextEdit>
#include <QLabel>
#include <QStackedWidget>
#include <QPushButton>

#include <QGridLayout>

#include <QMessageBox>

#include "wizard.h"

class PageOne : public QWidget
{
public:
  PageOne( QWidget *parent = 0 );
  
  QCheckBox *acceptDeal;
};

PageOne::PageOne( QWidget *parent ) : QWidget(parent)
{
  QGridLayout *layout = new QGridLayout( this );
  
  QTextEdit *textEdit = new QTextEdit();
  textEdit->setReadOnly( true );
  textEdit->setHtml( tr("<h1>The Rules</h1>"
                        "<p>The rules are to be followed!</p>") );
  
  acceptDeal = new QCheckBox( tr("I accept") );
  
  layout->addWidget( textEdit, 0, 0, 1, 2 );
  layout->addWidget( acceptDeal, 1, 1 );
}



class PageTwo : public QWidget
{
public:
  PageTwo( QWidget *parent = 0 );
  
  QCheckBox *doThis;
  QCheckBox *doThat;
  QCheckBox *extra;
};

PageTwo::PageTwo( QWidget *parent ) : QWidget(parent)
{
  QGridLayout *layout = new QGridLayout( this );

  doThis = new QCheckBox( tr("Do this") );
  doThat = new QCheckBox( tr("Do that") );
  extra = new QCheckBox( tr("Add something extra") );
  
  layout->addWidget( doThis, 0, 0 );
  layout->addWidget( doThat, 1, 0 );
  layout->addWidget( extra, 2, 0 );
  
  layout->setRowMinimumHeight( 3, 0 );
}



class PageThree : public QWidget
{
public:
  PageThree( QWidget *parent = 0 );
};

PageThree::PageThree( QWidget *parent ) : QWidget(parent)
{
  QGridLayout *layout = new QGridLayout( this );
  
  layout->addWidget( new QLabel( tr("All is ready. Press finish to get it done!") ), 0, 0 );
}



Wizard::Wizard() : QDialog()
{
  QGridLayout *layout = new QGridLayout( this );
  
  QPushButton *cancel = new QPushButton( tr("Cancel") );
  next = new QPushButton( tr("Next") );
  previous = new QPushButton( tr("Previous" ) );
  
  pages = new QStackedWidget();
  
  layout->addWidget( pages, 0, 0, 1, 5 );  
  layout->setColumnMinimumWidth( 0, 50 );
  layout->addWidget( previous, 1, 1 );
  layout->addWidget( next, 1, 2 );
  layout->setColumnMinimumWidth( 3, 5 );
  layout->addWidget( cancel, 1, 4 );

  previous->setEnabled( false );
  next->setEnabled( false );
  
  connect( next, SIGNAL(clicked()), this, SLOT(doNext()) );
  connect( previous, SIGNAL(clicked()), this, SLOT(doPrev()) );
  connect( cancel, SIGNAL(clicked()), this, SLOT(reject()) );
  
  pages->addWidget( pageOne = new PageOne( pages ) );
  pages->addWidget( pageTwo = new PageTwo( pages ) );
  pages->addWidget( pageThree = new PageThree( pages ) );
  
  connect( pageOne->acceptDeal, SIGNAL(toggled(bool)), next, SLOT(setEnabled(bool)) );
}

void Wizard::doNext()
{
  switch( pages->currentIndex() )
  {
    case 0:
      previous->setEnabled( true );
      
      disconnect( pageOne->acceptDeal, SIGNAL(toggled(bool)), next, SLOT(setEnabled(bool)) );
      
      break;
    case 1:
      next->setText( tr("Finish") );
      
      break;
    case 2:
      QMessageBox::information( this, tr("Finishing"), tr("Here is where the action takes place.") );
      accept();
      
      return;
  }
  
  pages->setCurrentIndex( pages->currentIndex()+1 );
}

void Wizard::doPrev()
{
  switch( pages->currentIndex() )
  {
    case 1:
      previous->setEnabled( false );
      next->setEnabled( pageOne->acceptDeal->isChecked() );

      connect( pageOne->acceptDeal, SIGNAL(toggled(bool)), next, SLOT(setEnabled(bool)) );
            
      break;
    case 2:
      next->setText( tr("Next") );
      
      break;
  }

  pages->setCurrentIndex( pages->currentIndex()-1 );
}
