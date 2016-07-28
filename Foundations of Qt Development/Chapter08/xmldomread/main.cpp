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
#include <QTextStream>

#include <QDomDocument>
#include <QDomElement>
#include <QDomText>

#include <QtDebug>

int main( int argc, char **argv )
{
  QFile file( "simple.xml" );
  if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
  {
    qDebug( "Failed to open file for reading." );
    return 0;
  }

  QDomDocument document;
  if( !document.setContent( &file ) )
  {
    qDebug( "Failed to parse the file into a DOM tree." );
    file.close();
    return 0;
  }
  
  file.close();

  QDomElement documentElement = document.documentElement();
  
  QDomNode node = documentElement.firstChild();
  while( !node.isNull() )
  {
    if( node.isElement() )
    {
      QDomElement element = node.toElement();
      qDebug() << "ELEMENT" << element.tagName();
      qDebug() << "ELEMENT ATTRIBUTE NAME" << element.attribute( "name", "not set" );
    }
    
    if( node.isText() )
    {
      QDomText text = node.toText();
      qDebug() << text.data();
    }
    
    node = node.nextSibling();
  }

  return 0;
}
