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

#include <QBuffer>
#include <QImageReader>
#include <QMessageBox>

#include "clientdialog.h"

ClientDialog::ClientDialog() : QDialog()
{
  ui.setupUi( this );
  
  connect( ui.getButton, SIGNAL(clicked()), this, SLOT(getClicked()) );
  
  connect( &socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpError(QAbstractSocket::SocketError)) );
  connect( &socket, SIGNAL(readyRead()), this, SLOT(tcpReady()) );
}

void ClientDialog::getClicked()
{
  ui.getButton->setEnabled( false );
  
  ui.imageLabel->setPixmap( QPixmap() );
  ui.imageLabel->setText( tr("<i>Getting image...</i>") );
  
  dataSize = 0;
  
  socket.abort();
  socket.connectToHost( ui.serverEdit->text(), 9876 );
}

void ClientDialog::tcpReady()
{
  if( dataSize == 0 )
  {
    QDataStream stream( &socket );
    stream.setVersion( QDataStream::Qt_4_0 );
  
    if( socket.bytesAvailable() < sizeof(quint32) )
      return;
      
    stream >> dataSize;
  }
  
  if( dataSize > socket.bytesAvailable() )
    return;

  QByteArray array = socket.read( dataSize );
  QBuffer buffer(&array);
  buffer.open( QIODevice::ReadOnly );
  
  QImageReader reader(&buffer, "PNG");
  QImage image = reader.read();

  if( !image.isNull() )
  {
    ui.imageLabel->setPixmap( QPixmap::fromImage( image ) );
    ui.imageLabel->setText( tr("") );
  }
  else
  {
    ui.imageLabel->setText( tr("<i>Invalid image received!</i>") );
  }
  
  ui.getButton->setEnabled( true );
}

void ClientDialog::tcpError( QAbstractSocket::SocketError error )
{
  if( error == QAbstractSocket::RemoteHostClosedError )
    return;
 
  QMessageBox::warning( this, tr("Error"), tr("TCP error: %1").arg( socket.errorString() ) );  
  ui.imageLabel->setText( tr("<i>No Image</i>") );
  ui.getButton->setEnabled( true );
}
