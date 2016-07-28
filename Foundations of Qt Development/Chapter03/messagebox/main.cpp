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

#include <QMessageBox>
#include <QInputDialog>

#include <QIcon>

class Tester : public QWidget
{
public:
  void test()
  {
    QMessageBox::information( 
        this, 
        tr("Application Name"), 
        tr("An information message.") );
        
    QMessageBox::warning( 
        this, 
        tr("Application Name"), 
        tr("A warning message.") );
        
    QMessageBox::critical( 
      this, 
      tr("Application Name"), 
      tr("A critical message.") );

    switch( QMessageBox::question( 
                this, 
                tr("Application Name"), 
                tr("An information message."), 
                
                QMessageBox::Yes | 
                QMessageBox::No | 
                QMessageBox::Cancel, 
                
                QMessageBox::Cancel ) )
    {
      case QMessageBox::Yes:
        qDebug( "yes" );
        break;
      case QMessageBox::No:
        qDebug( "no" );
        break;
      case QMessageBox::Cancel:
        qDebug( "cancel" );
        break;
      default:
        qDebug( "close" );
        break;
    }

    setWindowIcon( QIcon("cut.png") );
    
{
    bool ok;
    int value = QInputDialog::getInteger( 
                    this, 
                    tr("Integer"), 
                    tr("Enter an angle:"), 
                    90, 
                    0, 
                    360, 
                    1, 
                    &ok );
    if( ok )
    {
    }
}

{
    bool ok;    
    QStringList items;
    items << tr("Foo") << tr("Bar") << tr("Baz");
    QString item = QInputDialog::getItem( 
                      this, 
                      tr("Item"), 
                      tr("Pick an item:"), 
                      items, 
                      0, 
                      false, 
                      &ok );
    if( ok && !item.isEmpty() )
    {
    }
}

{    
    bool ok;    
    QString text = QInputDialog::getText( 
                      this, 
                      tr("String"), 
                      tr("Enter a city name:"), 
                      QLineEdit::Normal, 
                      tr("Alingsås"), 
                      &ok );
    if( ok && !text.isEmpty() )
    {
    }
}
  }
};

int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  
  Tester t;
  t.test();
  
  return 0;
}
