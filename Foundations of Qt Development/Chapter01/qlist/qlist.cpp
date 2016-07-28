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
 
#include <QList>
#include <QString>

#include <QtDebug>

void fillAndPrint()
{
  QList<QString> list;
  list << "foo" << "bar" << "baz";

  QString s;
  foreach( s, list )
    qDebug() << s;
}

void constIterators()
{
  QList<int> list;
  list << 23 << 27 << 52 << 52;
  
  QListIterator<int> javaIter( list );
  while( javaIter.hasNext() )
    qDebug() << javaIter.next();
    
  QList<int>::const_iterator stlIter;
  for( stlIter = list.begin(); stlIter != list.end(); ++stlIter )
    qDebug() << (*stlIter);
}

void mutableIterators()
{
  QList<int> list;
  list << 27 << 33 << 61 << 62;
  
  QMutableListIterator<int> javaIter( list );
  while( javaIter.hasNext() )
  {
    int value = javaIter.next() + 1;
    javaIter.setValue( value );
    qDebug() << value;
  }
  
  QList<int>::Iterator stlIter;
  for( stlIter = list.begin(); stlIter != list.end(); ++stlIter )
  {
    (*stlIter) = (*stlIter)*2;
    qDebug() << (*stlIter);
  }
}

void insertAndPrint()
{
  QList<QString> list;

  list << "first";
  list.append( "second" );
  list.prepend( "third" );
  list.insert( 1, "fourth" );
  list.insert( 4, "fifth" );
  
  QString s;
  foreach( s, list )
    qDebug() << s;
}

void miscExamples()
{
  QList<int> list;
  for( int i=0; i<10; i++ )
    list << i;
    
  int sum = list[5] + list.at(7);
}

int main( void )
{
  qDebug() << "Fill and print";
  fillAndPrint();
  
  qDebug() << "Const iterators";  
  constIterators();
  
  qDebug() << "Mutable iterators";
  mutableIterators();

  qDebug() << "Insert and print";
  insertAndPrint();  

  return 0;
}
