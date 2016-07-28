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
#include <QMessageBox>
#include <QReadWriteLock>

#include <QtDebug>

class TextDevice
{
public:
  TextDevice();
  
  void increase();
  void write( QString );

private:
  int count;
  QReadWriteLock lock;
};

TextDevice::TextDevice()
{
  count = 0;
}

void TextDevice::increase()
{
  QWriteLocker locker( &lock );
  count++;
}

void TextDevice::write( QString text )
{
  QReadLocker locker( &lock );
  qDebug() << QString( "Call %1: %2" ).arg( count ).arg( text );
}

bool stopThreads = false;

class IncreaseThread : public QThread
{
public:
  IncreaseThread( TextDevice *device );
  
  void run();
  
private:
  TextDevice *m_device;
};

IncreaseThread::IncreaseThread( TextDevice *device ) : QThread()
{
  m_device = device;
}

void IncreaseThread::run()
{
  while( !stopThreads )
  {
    msleep( 1200 );
    m_device->increase();
  }
}

class TextThread : public QThread
{
public:
  TextThread( QString text, TextDevice *m_device );
  
  void run();
  
private:
  QString m_text;
  TextDevice *m_device;
};

TextThread::TextThread( QString text, TextDevice *device ) : QThread()
{
  m_text = text;
  m_device = device;
}

void TextThread::run()
{
  while( !stopThreads )
  {
    m_device->write( m_text );
    sleep( 1 );
  }
}

int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  
  TextDevice device;
  IncreaseThread inc( &device );
  TextThread foo( "Foo", &device ), bar( "Bar", &device );
  
  foo.start();
  bar.start();
  inc.start();
  
  QMessageBox::information( 0, "Threading", "Close me to stop!" );
    
  stopThreads = true;
    
  foo.wait();
  bar.wait();
  inc.wait();
    
  return 0;
}
