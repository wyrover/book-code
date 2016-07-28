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

#include "mainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QApplication>

#include "preferencedialog.h"
#include "otherdialog.h"

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent )
{
  setAttribute( Qt::WA_DeleteOnClose );
    
  QAction *fileNewAction = new QAction( tr("&New"), this );
  connect( fileNewAction, SIGNAL(triggered()), this, SLOT(fileNew()) );
  
  QAction *fileCloseAction = new QAction( tr( "&Close"), this );
  connect( fileCloseAction, SIGNAL(triggered()), this, SLOT(close()) );
  
  QAction *fileExitAction = new QAction( tr("E&xit"), this );
  connect( fileExitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()) );
  
  QAction *dialogPreferencesAction = new QAction( tr("Preferences..."), this );
  connect( dialogPreferencesAction, SIGNAL(triggered()), this, SLOT(dialogPreferences()) );
  
  QAction *dialogOtherAction = new QAction( tr("Other..."), this );
  connect( dialogOtherAction, SIGNAL(triggered()), this, SLOT(dialogOther()) );
  
  QMenu *file = menuBar()->addMenu( tr("&File") );
  file->addAction( fileNewAction );
  file->addAction( fileCloseAction );
  file->addSeparator();
  file->addAction( fileExitAction );
  
  QMenu *dialogs = menuBar()->addMenu( tr("&Dialogs") );
  dialogs->addAction( dialogPreferencesAction );
  dialogs->addAction( dialogOtherAction );
}

void MainWindow::fileNew()
{
  (new MainWindow())->show();
}

void MainWindow::dialogPreferences()
{
  PreferenceDialog dlg;
  
  dlg.exec();
}

void MainWindow::dialogOther()
{
  OtherDialog dlg;
  
  dlg.exec();
}
