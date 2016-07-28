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
 
#include <QStringList>
#include <QStack>
#include <QQueue>

#include <QtDebug>

void stringList()
{
  QString text;
  
  QStringList list;
  list << "foo" << "bar" << "baz";
  foreach( text, list )
    qDebug() << text;
  
  qDebug() << "---";

  QString all = list.join(",");
  qDebug() << all;

  qDebug() << "---";

  list.replaceInStrings( "a", "oo" );
  foreach( text, list )
    qDebug() << text;      
  
  qDebug() << "---";

  list << all.split(",");
  foreach( text, list )
    qDebug() << text;
}

void stackDemo()
{
  QStack<QString> stack;
  
  stack.push( "foo" );
  stack.push( "bar" );
  stack.push( "baz" );
  
  QString result;
  while( !stack.isEmpty() )
    result += stack.pop();
  qDebug() << result;
}

void queueDemo()
{
  QQueue<QString> queue;
  
  queue.enqueue( "foo" );
  queue.enqueue( "bar" );
  queue.enqueue( "baz" );
  
  QString result;
  while( !queue.isEmpty() )
    result += queue.dequeue();
  qDebug() << result;
}

int main( void )
{
  qDebug() << "String list";
  stringList();
  
  qDebug() << "Stack";
  stackDemo();
  
  qDebug() << "Queue";
  queueDemo();
  
  return 0;
}
