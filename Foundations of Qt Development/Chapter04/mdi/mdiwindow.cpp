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

#include <QWorkspace>
#include <QSignalMapper>

#include "mdiwindow.h"

#include "documentwindow.h"

MdiWindow::MdiWindow( QWidget *parent ) : QMainWindow( parent )
{
  setWindowTitle( tr( "MDI" ) );

  workspace = new QWorkspace;
  setCentralWidget( workspace );
  
  connect( workspace, SIGNAL(windowActivated(QWidget *)), this, SLOT(enableActions()));
  mapper = new QSignalMapper( this );
  connect( mapper, SIGNAL(mapped(QWidget*)), workspace, SLOT(setActiveWindow(QWidget*)) );

  createActions();
  createMenus();
  createToolbars();
  statusBar()->showMessage( tr("Done") );
  
  enableActions();
}
  
void MdiWindow::closeEvent( QCloseEvent *event )
{
  workspace->closeAllWindows();

  if( !activeDocument() )
    event->accept();
  else
    event->ignore();
}
  
void MdiWindow::fileNew()
{
  DocumentWindow *document = new DocumentWindow;
  workspace->addWindow( document );

  connect( document, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)) );
  connect( document, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)) );
  
  document->show();
}

void MdiWindow::helpAbout()
{
  QMessageBox::about( this, tr("About MDI"), tr("A multiple document interface application.") );
}

DocumentWindow *MdiWindow::activeDocument()
{
  return qobject_cast<DocumentWindow*>(workspace->activeWindow());
}

void MdiWindow::enableActions()
{
  bool hasDocuments = (activeDocument() != 0 );
  
  closeAction->setEnabled( hasDocuments );  
  pasteAction->setEnabled( hasDocuments );  
  tileAction->setEnabled( hasDocuments );
  cascadeAction->setEnabled( hasDocuments );
  nextAction->setEnabled( hasDocuments );
  previousAction->setEnabled( hasDocuments );
  separatorAction->setVisible( hasDocuments );

  bool hasSelection = hasDocuments && activeDocument()->textCursor().hasSelection();

  cutAction->setEnabled( hasSelection );  
  copyAction->setEnabled( hasSelection );  
}

void MdiWindow::createActions()
{
  newAction = new QAction( QIcon(":/images/new.png"), tr("&New"), this );
  newAction->setShortcut( tr("Ctrl+N") );
  newAction->setStatusTip( tr("Create a new document") );
  connect( newAction, SIGNAL(triggered()), this, SLOT(fileNew()) );
  
  closeAction = new QAction( tr("&Close"), this );
  closeAction->setShortcut( tr("Ctrl+W") );
  closeAction->setStatusTip( tr("Close this document") );
  connect( closeAction, SIGNAL(triggered()), workspace, SLOT(closeActiveWindow()) );

  exitAction = new QAction( tr("E&xit"), this );
  exitAction->setShortcut( tr("Ctrl+Q") );
  exitAction->setStatusTip( tr("Quit the application") );
  connect( exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()) );
  

  cutAction = new QAction( QIcon(":/images/cut.png"), tr("Cu&t"), this );
  cutAction->setShortcut( tr("Ctrl+X") );
  cutAction->setStatusTip( tr("Cut") );
  cutAction->setEnabled(false);
  connect( cutAction, SIGNAL(triggered()), this, SLOT(editCut()) );
  
  copyAction = new QAction( QIcon(":/images/copy.png"), tr("&Copy"), this );
  copyAction->setShortcut( tr("Ctrl+C") );
  copyAction->setStatusTip( tr("Copy") );
  copyAction->setEnabled(false);
  connect( copyAction, SIGNAL(triggered()), this, SLOT(editCopy()) );

  pasteAction = new QAction( QIcon(":/images/paste.png"), tr("&Paste"), this );
  pasteAction->setShortcut( tr("Ctrl+V") );
  pasteAction->setStatusTip( tr("Paste") );
  connect( pasteAction, SIGNAL(triggered()), this, SLOT(editPaste()) );
  

  tileAction = new QAction( tr("&Tile"), this );
  tileAction->setStatusTip( tr("Tile the windows") );
  connect( tileAction, SIGNAL(triggered()), workspace, SLOT(tile()) );
  
  cascadeAction = new QAction( tr("&Cascade"), this );
  cascadeAction->setStatusTip( tr("Cascade the windows") );
  connect( cascadeAction, SIGNAL(triggered()), workspace, SLOT(cascade()) );

  nextAction = new QAction( tr("&Next window"), this );
  nextAction->setStatusTip( tr("Move to the next window") );
  connect( nextAction, SIGNAL(triggered()), workspace, SLOT(activateNextWindow()) );
  
  previousAction = new QAction( tr("&Previous window"), this );
  previousAction->setStatusTip( tr("Move to the previous window") );
  connect( previousAction, SIGNAL(triggered()), workspace, SLOT(activatePreviousWindow()) );
  
  separatorAction = new QAction( this );
  separatorAction->setSeparator( true );
  

  aboutAction = new QAction( tr("&About"), this );
  aboutAction->setStatusTip( tr("About this application") );
  connect( aboutAction, SIGNAL(triggered()), this, SLOT(helpAbout()) );

  aboutQtAction = new QAction( tr("About &Qt"), this );
  aboutQtAction->setStatusTip( tr("About the Qt toolkit") );
  connect( aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
}

void MdiWindow::createMenus()
{
  QMenu *menu;
  
  menu = menuBar()->addMenu( tr("&File") );
  menu->addAction( newAction );
  menu->addAction( closeAction );
  menu->addSeparator();
  menu->addAction( exitAction );
  
  menu = menuBar()->addMenu( tr("&Edit") );
  menu->addAction( cutAction );
  menu->addAction( copyAction );
  menu->addAction( pasteAction );
  
  windowMenu = menuBar()->addMenu( tr("&Window") );
  connect( windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowList()) );
  
  menu = menuBar()->addMenu( tr("&Help") );
  menu->addAction( aboutAction );
  menu->addAction( aboutQtAction );
}

void MdiWindow::createToolbars()
{
  QToolBar *toolbar;
  
  toolbar = addToolBar( tr("File") );
  toolbar->addAction( newAction );
  
  toolbar = addToolBar( tr("Edit") );
  toolbar->addAction( cutAction );
  toolbar->addAction( copyAction );
  toolbar->addAction( pasteAction );
}

void MdiWindow::editCut()
{
  activeDocument()->cut();
}
  
void MdiWindow::editCopy()
{
  activeDocument()->copy();
}
  
void MdiWindow::editPaste()
{
  activeDocument()->paste();
}

void MdiWindow::updateWindowList()
{
  windowMenu->clear();
  
  windowMenu->addAction( tileAction );
  windowMenu->addAction( cascadeAction );
  windowMenu->addSeparator();
  windowMenu->addAction( nextAction );
  windowMenu->addAction( previousAction );
  windowMenu->addAction( separatorAction );
  
  int i=1;
  foreach( QWidget *w, workspace->windowList() )
  {
    QString text;
    if( i<10 )
      text = tr("&%1 %2").arg( i++ ).arg( w->windowTitle() );
    else
      text = w->windowTitle();
      
    QAction *action = windowMenu->addAction( text );
    action->setCheckable( true );
    action->setChecked( w == activeDocument() );
    connect( action, SIGNAL(triggered()), mapper, SLOT(map()) );
    mapper->setMapping( action, w );
  }
}
