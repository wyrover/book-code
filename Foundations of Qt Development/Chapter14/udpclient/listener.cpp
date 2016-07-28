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

#include "listener.h"

#include <QImage>
#include <QPixmap>
#include <QUdpSocket>
#include <QDataStream>

Listener::Listener( QWidget *parent ) : QLabel( parent )
{
  setText( "Waiting for data." );

  image = 0;  
  
  socket = new QUdpSocket( this );
  socket->bind( 9988 );
  
  connect( socket, SIGNAL(readyRead()), this, SLOT(dataPending()) );
}

void Listener::dataPending()
{
  while( socket->hasPendingDatagrams() )
  {
    QByteArray buffer( socket->pendingDatagramSize(), 0 );
    socket->readDatagram( buffer.data(), buffer.size() );
    
    QDataStream stream( buffer );
    stream.setVersion( QDataStream::Qt_4_0 );
    
    quint16 width, height, y;
    stream >> width >> height >> y;
    
    if( !image )
      image = new QImage( width, height, QImage::Format_RGB32 );
    else if( image->width() != width || image->height() != height )
    {
      delete image;
      image = new QImage( width, height, QImage::Format_RGB32 );
    }

    for( int x=0; x<width; ++x )
    {
      quint8 red, green, blue;
      stream >> red >> green >> blue;
      
      image->setPixel( x, y, qRgb( red, green, blue ) );
    }
  }
  
  setText( "" );
  setPixmap( QPixmap::fromImage( *image ) );
  resize( image->size() );
}
