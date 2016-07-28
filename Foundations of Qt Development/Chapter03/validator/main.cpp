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

#include <QApplication>

#include <QDialog>

#include <QGridLayout>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QIntValidator>
#include <QDoubleValidator>

class ValidationDialog : public QDialog
{
public:
  ValidationDialog()
  {
    QGridLayout *layout = new QGridLayout( this );
    
    QLineEdit *intEdit = new QLineEdit( "42" );
    QLineEdit *doubleEdit = new QLineEdit( "3.14" );
    QPushButton *button = new QPushButton( "Close" );
    
    layout->addWidget( new QLabel("Integer:"), 0, 0 );
    layout->addWidget( intEdit, 0, 1 );
    layout->addWidget( new QLabel("Double:"), 1, 0 );
    layout->addWidget( doubleEdit, 1, 1 );
    layout->addWidget( button, 2, 0, 1, 2 );
    
    QIntValidator *intVal = new QIntValidator( 0, 100, this );
    intEdit->setValidator( intVal );
    
    QDoubleValidator *doubleVal = new QDoubleValidator( -32.5, 99.0, 1, this );
    doubleEdit->setValidator( doubleVal );
    
    connect( button, SIGNAL(clicked()), this, SLOT(accept()) );
  }
};

int main( int argc, char** argv )
{
  QApplication app( argc, argv );
  
  ValidationDialog w;
  w.show();
  
  return app.exec();
}
