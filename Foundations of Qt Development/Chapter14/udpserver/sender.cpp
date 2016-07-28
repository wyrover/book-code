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

#include "sender.h"

#include <QImage>
#include <QUdpSocket>
#include <QTimer>
#include <QByteArray>
#include <QDataStream>

Sender::Sender()
{
  socket = new QUdpSocket( this );
  
  image = new QImage( "test.png" );
  if( image->isNull() )
    qFatal( "Failed to open test.png" );

  QTimer *timer = new QTimer( this );
  timer->setInterval( 250 );
  timer->start();
  
  connect( timer, SIGNAL(timeout()), this, SLOT(broadcastLine()) );
}

void Sender::broadcastLine()
{
  QByteArray buffer( 6+3*image->width(), 0 );
  QDataStream stream( &buffer, QIODevice::WriteOnly );
  stream.setVersion( QDataStream::Qt_4_0 );
  
  stream << (quint16)image->width() << (quint16)image->height();
  
  quint16 y = qrand() % image->height();
  
  stream << y;
  
  for( int x=0; x<image->width(); ++x )
  {
    QRgb rgb = image->pixel( x, y );
    
    stream << (quint8)qRed( rgb ) << (quint8)qGreen( rgb ) << (quint8)qBlue( rgb );
  }
  
  socket->writeDatagram( buffer, QHostAddress::Broadcast, 9988 );
}
