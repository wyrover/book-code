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

int main( int argc, char **argv )
{
  QFile inFile( "simple.xml" );
  if( !inFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
  {
    qDebug( "Failed to open file for reading." );
    return 0;
  }

  QDomDocument document;
  if( !document.setContent( &inFile ) )
  {
    qDebug( "Failed to parse the file into a DOM tree." );
    inFile.close();
    return 0;
  }
  
  inFile.close();

  QDomElement documentElement = document.documentElement();
  QDomNodeList elements = documentElement.elementsByTagName( "bar" );
  if( elements.size() == 0 )
  {
    QDomElement bar = document.createElement( "bar" );
    documentElement.insertBefore( bar, QDomNode() );
  }
  else if( elements.size() == 1 )
  {
    QDomElement bar = elements.at(0).toElement();
    
    QDomElement baz = document.createElement( "baz" );
    baz.setAttribute( "count", QString::number( bar.elementsByTagName( "baz" ).size() + 1 ) );
      
    bar.appendChild( baz );
  }

  QFile outFile( "simple-modified.xml" );
  if( !outFile.open( QIODevice::WriteOnly | QIODevice::Text ) )
  {
    qDebug( "Failed to open file for writing." );
    return 0;
  }
  
  QTextStream stream( &outFile );
  stream << document.toString();
  
  outFile.close();
  
  return 0;
}
