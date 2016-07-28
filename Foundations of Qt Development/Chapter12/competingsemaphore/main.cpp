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
#include <QThread>
#include <QSemaphore>
#include <QMutex>

#include <QtDebug>

const int bufferSize = 20;

QChar buffer[ bufferSize ];
QSemaphore freeSpace( bufferSize );
QSemaphore availableData( 0 );

QSemaphore atEnd( 0 );

class TextProducer : public QThread
{
public:
  TextProducer( QString text );
  
  void run();
  
private:
  QString m_text;
};

TextProducer::TextProducer( QString text ) : QThread()
{
  atEnd.release();
  m_text = text;
}

void TextProducer::run()
{
  static int index = 0;
  static QMutex indexMutex;
  
  for( int i=0; i<m_text.length(); ++i )
  {
    freeSpace.acquire();
    indexMutex.lock();
    buffer[ index++ % bufferSize ] = m_text.data()[ i ];
    indexMutex.unlock();
    
    if( i == m_text.length()-1 )
      atEnd.acquire();
      
    availableData.release();
  }
}

class TextConsumer : public QThread
{
public:
  void run();
};

void TextConsumer::run()
{
  int i = 0;
  
  while( atEnd.available() || availableData.available() )
  {
    availableData.acquire();
    qDebug() << buffer[ i ];
    i = (i+1) % bufferSize;
    freeSpace.release();
  }
}

int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  
  TextProducer p1( "this text is written using lower case characters."
    "it will compete with a text written using upper case characters." );
  TextProducer p2( "THIS TEXT IS WRITTEN USING UPPER CASE CHARACTERS!"
    "IT WILL COMPETE WITH A TEXT WRITTEN USING LOWER CASE CHARACTERS!" );
  TextConsumer consumer;
  
  p1.start();
  p2.start();
  consumer.start();
  
  p1.wait();
  p2.wait();
  consumer.wait();
    
  return 0;
}
