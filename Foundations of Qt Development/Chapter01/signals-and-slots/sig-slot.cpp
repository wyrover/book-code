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
 
#include "myclass.h"

#include <QtDebug>

int main( int argc, char **argv )
{
  QObject parent;
  MyClass *a, *b, *c;

  a = new MyClass( "foo", &parent );
  b = new MyClass( "bar", &parent );
  c = new MyClass( "baz", &parent );

  QObject::connect(
    a, SIGNAL(textChanged(const QString&)),
    b, SLOT(setText(const QString&)) );
  QObject::connect(
    b, SIGNAL(textChanged(const QString&)),
    c, SLOT(setText(const QString&)) );
  QObject::connect(
    c, SIGNAL(textChanged(const QString&)),
    b, SLOT(setText(const QString&)) );

  qDebug() << "--- After creation ---";
  qDebug() << "a:" << a->text() << "\nb:" << b->text() << "\nc:" << c->text();

  c->setText( "test" );

  qDebug() << "--- After test ---";
  qDebug() << "a:" << a->text() << "\nb:" << b->text() << "\nc:" << c->text();

  a->setText( "Qt" );

  qDebug() << "--- After Qt ---";
  qDebug() << "a:" << a->text() << "\nb:" << b->text() << "\nc:" << c->text();

  return a->getLengthOfText() - c->getLengthOfText();
}
