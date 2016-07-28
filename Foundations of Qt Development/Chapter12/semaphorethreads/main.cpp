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

#include <QtDebug>

const int bufferSize = 20;

QChar buffer[ bufferSize ];
QSemaphore freeSpace( bufferSize );
QSemaphore availableData( 0 );

bool atEnd = false;

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
  m_text = text;
}

void TextProducer::run()
{
  for( int i=0; i<m_text.length(); ++i )
  {
    freeSpace.acquire();
    buffer[ i % bufferSize ] = m_text.data()[ i ];

    if( i == m_text.length()-1 )
      atEnd = true;

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
  
  while( !atEnd || availableData.available() )
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
  
  TextProducer producer( "Lorem ipsum dolor sit amet, "
                         "consectetuer adipiscing elit. "
                         "Praesent tortor." );
  TextConsumer consumer;
  
  producer.start();
  consumer.start();
  
  producer.wait();
  consumer.wait();
    
  return 0;
}
