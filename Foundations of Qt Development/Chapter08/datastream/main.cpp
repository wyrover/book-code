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

#include <QFile>
#include <QDataStream>
#include <QList>
#include <QColor>

#include <QtDebug>

struct ColorText
{
  QString text;
  QColor color;
};

QDataStream &operator<<( QDataStream &stream, const ColorText &data )
{
  stream << data.text << data.color;
  
  return stream;
}

QDataStream &operator>>( QDataStream &stream, ColorText &data )
{
  stream >> data.text;
  stream >> data.color;
  
  return stream;
}

void saveList()
{
  QList<ColorText> list;
  ColorText data;
  
  data.text = "Red";
  data.color = Qt::red;
  list << data;
  
  data.text = "Blue";
  data.color = Qt::blue;
  list << data;

  data.text = "Yellow";
  data.color = Qt::yellow;
  list << data;

  data.text = "Green";
  data.color = Qt::green;
  list << data;
  
  QFile file( "test.dat" );
  if( !file.open( QIODevice::WriteOnly ) )
    return;
    
  QDataStream stream( &file );
  stream.setVersion( QDataStream::Qt_4_2 );
  
  stream << list;
  
  file.close();
}

void loadList()
{
  QList<ColorText> list;
  
  QFile file( "test.dat" );
  if( !file.open( QIODevice::ReadOnly ) )
    return;
    
  QDataStream stream( &file );
  stream.setVersion( QDataStream::Qt_4_2 );

  stream >> list;
  
  file.close();

  foreach( ColorText data, list )
    qDebug() << data.text << "(" 
             << data.color.red() << "," 
             << data.color.green() << "," 
             << data.color.blue() << ")";
}

int main( int argc, char **argv )
{
  saveList();
  loadList();
  
  return 0;
}
