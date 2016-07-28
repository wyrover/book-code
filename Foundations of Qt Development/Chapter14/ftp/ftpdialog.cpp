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

#include <QFileDialog>
#include <QMessageBox>

#include "ftpdialog.h"

FtpDialog::FtpDialog() : QDialog()
{
  file = 0;
  
  ui.setupUi( this );
  
  connect( ui.connectButton, SIGNAL(clicked()), this, SLOT(connectClicked()) );
  connect( ui.disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectClicked()) );
  connect( ui.cdButton, SIGNAL(clicked()), this, SLOT(cdClicked()) );
  connect( ui.upButton, SIGNAL(clicked()), this, SLOT(upClicked()) );
  connect( ui.getButton, SIGNAL(clicked()), this, SLOT(getClicked()) );
  
  connect( ui.dirList, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()) );
  
  connect( &ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(ftpFinished(int,bool)) );
  connect( &ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(ftpListInfo(QUrlInfo)) );
  connect( &ftp, SIGNAL(dataTransferProgress(qint64,qint64)), this, SLOT(ftpProgress(qint64,qint64)) );
  
  ui.disconnectButton->setEnabled( false );
  ui.cdButton->setEnabled( false );
  ui.upButton->setEnabled( false );
  ui.getButton->setEnabled( false );
}

void FtpDialog::connectClicked()
{
  ui.connectButton->setEnabled( false );
  
  ftp.connectToHost( "ftp.trolltech.com" );
  ui.statusLabel->setText( tr("Connecting to host...") );
}

void FtpDialog::disconnectClicked()
{
  ui.disconnectButton->setEnabled( false );
  ui.cdButton->setEnabled( false );
  ui.upButton->setEnabled( false );
  ui.getButton->setEnabled( false );
  
  ftp.close();
}

void FtpDialog::cdClicked()
{
  ui.disconnectButton->setEnabled( false );
  ui.cdButton->setEnabled( false );
  ui.upButton->setEnabled( false );
  ui.getButton->setEnabled( false );

  ftp.cd( ui.dirList->selectedItems()[0]->text() );
  ui.statusLabel->setText( tr("Changing directory...") );
}

void FtpDialog::upClicked()
{
  ui.disconnectButton->setEnabled( false );
  ui.cdButton->setEnabled( false );
  ui.upButton->setEnabled( false );
  ui.getButton->setEnabled( false );

  ftp.cd("..");
  ui.statusLabel->setText( tr("Changing directory...") );
}

void FtpDialog::getClicked()
{
  QString fileName = QFileDialog::getSaveFileName( this, tr("Get File"), ui.dirList->selectedItems()[0]->text() );
  if( fileName.isEmpty() )
    return;
    
  file = new QFile( fileName, this );
  if( !file->open( QIODevice::WriteOnly ) )
  {
    QMessageBox::warning( this, tr("Error"), tr("Failed to open file %1 for writing.").arg( fileName ) );
    
    delete file;
    file = 0;
    
    return;
  }

  ui.disconnectButton->setEnabled( false );
  ui.cdButton->setEnabled( false );
  ui.upButton->setEnabled( false );
  ui.getButton->setEnabled( false );

  ftp.get( ui.dirList->selectedItems()[0]->text(), file );
  ui.statusLabel->setText( tr("Downloading file...") );
}

void FtpDialog::ftpProgress( qint64 done, qint64 total )
{
  if( total == 0 )
    return;
    
  ui.statusLabel->setText( tr("Downloading file... (%1%)").arg( QString::number( double(done)*100/double(total), 'f', 1 ) ) );
}

void FtpDialog::selectionChanged()
{
  if( ui.dirList->selectedItems().count() == 1 )
  {
    if( files.indexOf( ui.dirList->selectedItems()[0]->text() ) == -1 )
    {
      ui.cdButton->setEnabled( ui.disconnectButton->isEnabled() );
      ui.getButton->setEnabled( false );
    }
    else
    {
      ui.cdButton->setEnabled( false );
      ui.getButton->setEnabled( ui.disconnectButton->isEnabled() );
    }
  }
  else
  {
    ui.cdButton->setEnabled( false );
    ui.getButton->setEnabled( false );
  }
}

void FtpDialog::getFileList()
{
  ui.disconnectButton->setEnabled( false );
  ui.cdButton->setEnabled( false );
  ui.upButton->setEnabled( false );
  ui.getButton->setEnabled( false );

  ui.dirList->clear();
  files.clear();
  
  if( ftp.state() == QFtp::LoggedIn )
    ftp.list();
}

void FtpDialog::ftpListInfo( QUrlInfo info )
{
  ui.dirList->addItem( info.name() );
  if( info.isFile() )
    files << info.name();
}

void FtpDialog::ftpFinished( int request, bool error )
{
  if( error )
  {
    switch( ftp.currentCommand() )
    {
      case QFtp::ConnectToHost:
        QMessageBox::warning( this, tr("Error"), tr("Failed to connect to host.") );
        ui.connectButton->setEnabled( true );
        
        break; 
      case QFtp::Login:
        QMessageBox::warning( this, tr("Error"), tr("Failed to login.") );
        ui.connectButton->setEnabled( true );
        
        break; 
      case QFtp::List:
        QMessageBox::warning( this, tr("Error"), tr("Failed to get file list.\nClosing connection.") );
        ftp.close();
        
        break;
      case QFtp::Cd:
        QMessageBox::warning( this, tr("Error"), tr("Failed to change directory.") );
        getFileList();
        
        break;
      case QFtp::Get:
        QMessageBox::warning( this, tr("Error"), tr("Failed to get file?") );
        file->close();
        file->remove();
        
        delete file;
        file = 0;
        
        ui.disconnectButton->setEnabled( true );
        ui.upButton->setEnabled( true );
        selectionChanged();
        
        break;
    }
    
    ui.statusLabel->setText( tr("Ready.") );
  }
  else
  {
    switch( ftp.currentCommand() )
    {
      case QFtp::ConnectToHost:
        ftp.login();

        break;
      case QFtp::Login:
        getFileList();
        
        break;
      case QFtp::Close:
        ui.connectButton->setEnabled( true );
        getFileList();
        
        break;
      case QFtp::List:
        ui.disconnectButton->setEnabled( true );
        ui.upButton->setEnabled( true );
        ui.statusLabel->setText( tr("Ready.") );
        
        break;
      case QFtp::Cd:
        getFileList();
        
        break;
      case QFtp::Get:
        file->close();
        
        delete file;
        file = 0;
        
        ui.disconnectButton->setEnabled( true );
        ui.upButton->setEnabled( true );
        selectionChanged();

        ui.statusLabel->setText( tr("Ready.") );

        break;        
    }
  }
}
