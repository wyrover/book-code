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
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QCloseEvent>
#include <QMessageBox>

#include <QTextEdit>

#include "documentwindow.h"

DocumentWindow::DocumentWindow( QWidget *parent ) : QTextEdit( parent )
{
  setAttribute( Qt::WA_DeleteOnClose );
  setWindowTitle( tr("%1[*]" ).arg("unnamed") );
  
  connect( document(), SIGNAL(modificationChanged(bool)), this, SLOT(setWindowModified(bool)) );
}
  
void DocumentWindow::closeEvent( QCloseEvent *event )
{
  if( isSafeToClose() )
    event->accept();
  else
    event->ignore();
}

bool DocumentWindow::isSafeToClose()
{
  if( document()->isModified() ) 
  {
    switch( QMessageBox::warning( this, tr("MDI"), 
      tr("The document has unsaved changes.\n"
         "Do you want to save it before it is closed?"), 
         QMessageBox::Discard | QMessageBox::Cancel ) )
    {
    case QMessageBox::Cancel:
      return false;
    default:
      return true;
    }
  }

  return true;
}
