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

#include <QMessageBox>
#include <QFileDialog>

#include "httpdialog.h"

HttpDialog::HttpDialog() : QDialog()
{
  file = 0;

  ui.setupUi( this );

  connect( ui.getButton, SIGNAL(clicked()), this, SLOT(getClicked()) );
  
  connect( &http, SIGNAL(stateChanged(int)), this, SLOT(httpStateChanged(int)) );
  connect( &http, SIGNAL(dataSendProgress(int,int)), this, SLOT(httpDataSent(int,int)) );
  connect( &http, SIGNAL(dataReadProgress(int,int)), this, SLOT(httpDataReceived(int,int)) );
  connect( &http, SIGNAL(responseHeaderReceived(QHttpResponseHeader)), this, SLOT(httpHeaderDone(QHttpResponseHeader)) );
  connect( &http, SIGNAL(readyRead(QHttpResponseHeader)), this, SLOT(httpDataDone(QHttpResponseHeader)) );
  connect( &http, SIGNAL(requestStarted(int)), this, SLOT(httpStarted(int)) );
  connect( &http, SIGNAL(requestFinished(int,bool)), this, SLOT(httpFinished(int,bool)) );
  connect( &http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)) );
}

void HttpDialog::getClicked()
{
  QUrl url( ui.requestEdit->text(), QUrl::TolerantMode );
    
  if( !url.isValid() )
  {
    ui.hostLabel->setText( "" );
    ui.pathLabel->setText( "" );
    ui.portLabel->setText( "" );
    ui.userLabel->setText( "" );
    ui.passwordLabel->setText( "" );    
    
    QMessageBox::warning( this, tr("Invalid URL"), tr("The URL '%1' is invalid.").arg( ui.requestEdit->text() ) );
    
    return;
  }
    
  ui.hostLabel->setText( url.host() );
  ui.pathLabel->setText( url.path() );
  ui.portLabel->setText( QString::number(url.port()==-1?80:url.port()) );
  ui.userLabel->setText( url.userName() );
  ui.passwordLabel->setText( url.password() );

  http.setHost( url.host(), url.port()==-1?80:url.port() );
  if( !url.userName().isEmpty() )
    http.setUser( url.userName(), url.password() );
  
  QString fileName = QFileDialog::getSaveFileName( this );
  if( fileName.isEmpty() )
    return;
    
  file = new QFile( fileName, this );
  if( !file->open( QIODevice::WriteOnly ) )
  {
    QMessageBox::warning( this, tr("Could not write"), tr("Could not open the file %f for writing.").arg( fileName ) );

    delete file;
    file = 0;
    
    return;
  }
  
  http.get( url.path(), file );
  ui.getButton->setEnabled( false );  
}

#include <QtDebug>

void HttpDialog::httpStateChanged( int state )
{
  QString stateText;
  
  switch( state )
  {
    case QHttp::Unconnected:
      stateText = "Unconnected";
      break;
    case QHttp::HostLookup:
      stateText = "HostLookup";
      break;
    case QHttp::Connecting:
      stateText = "Connecting";
      break;
    case QHttp::Sending:
      stateText = "Sending";
      break;
    case QHttp::Reading:
      stateText = "Reading";
      break;
    case QHttp::Connected:
      stateText = "Connected";
      break;
    case QHttp::Closing:
      stateText = "Closing";
      break;
    default:
      stateText = "Undefined";
      break;
  }
  
  ui.statusList->addItem( QString("stateChanged( %1 )").arg( stateText ) );
  qDebug() << QString("stateChanged( %1 )").arg( stateText );
}

void HttpDialog::httpDataSent( int done, int total )
{
  ui.statusList->addItem( QString("dataSendProgress( done: %1, total: %2 )").arg( done ).arg( total ) );
  qDebug() << QString("dataSendProgress( done: %1, total: %2 )").arg( done ).arg( total );
}

void HttpDialog::httpDataReceived( int done, int total )
{
  ui.statusList->addItem( QString("dataReadProgress( done: %1, total: %2 )").arg( done ).arg( total ) );
  qDebug() << QString("dataReadProgress( done: %1, total: %2 )").arg( done ).arg( total );
}

void HttpDialog::httpHeaderDone( QHttpResponseHeader header )
{
  ui.statusList->addItem( QString("responseHeaderReceived(code: %1, reason: %2, version: %3.%4 )").arg( header.statusCode() ).arg( header.reasonPhrase() ).arg( header.majorVersion() ).arg( header.minorVersion() ) );
  qDebug() << QString("responseHeaderReceived(code: %1, reason: %2, version: %3.%4 )").arg( header.statusCode() ).arg( header.reasonPhrase() ).arg( header.majorVersion() ).arg( header.minorVersion() );
}

void HttpDialog::httpDataDone( QHttpResponseHeader header )
{
  ui.statusList->addItem( QString("readReady(code: %1, reason: %2, version: %3.%4 )").arg( header.statusCode() ).arg( header.reasonPhrase() ).arg( header.majorVersion() ).arg( header.minorVersion() ) );
  qDebug() << QString("readReady(code: %1, reason: %2, version: %3.%4 )").arg( header.statusCode() ).arg( header.reasonPhrase() ).arg( header.majorVersion() ).arg( header.minorVersion() );
}

void HttpDialog::httpStarted( int id )
{
  ui.statusList->addItem( QString("requestStarted( %1 )").arg( id ) );
  qDebug() << QString("requestStarted( %1 )").arg( id );
}

void HttpDialog::httpFinished( int id, bool error )
{
  ui.statusList->addItem( QString("requestFinished( %1, %2 )").arg( id ).arg( error?"True":"False" ) );
  qDebug() << QString("requestFinished( %1, %2 )").arg( id ).arg( error?"True":"False" );
  if( error )
    QMessageBox::warning( this, tr("Http: requestFinished"), http.errorString() );
}

void HttpDialog::httpDone( bool error )
{
  ui.statusList->addItem( QString("done( %1 )").arg( error?"True":"False" ) );
  qDebug() << QString("done( %1 )").arg( error?"True":"False" );
  
  if( error )
  {
    QMessageBox::warning( this, tr("Http: done"), http.errorString() );
      
    if( file )
    {
      file->close();
      file->remove();
      
      delete file;
      file = 0;
    }
  }

  if( file )
  {
    file->close();
    
    delete file;
    file = 0;
  }
    
  ui.getButton->setEnabled( true );
}
