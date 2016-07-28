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
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QXmlDefaultHandler>

#include <QtDebug>

class MyHandler : public QXmlDefaultHandler
{
public:
  bool startDocument();
  bool endDocument();
  
  bool startElement( const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts );
  bool endElement( const QString &namespaceURI, const QString &localName, const QString &qName );
  
  bool characters( const QString &ch );
};

bool MyHandler::startDocument()
{
  qDebug() << "Start of the document.";
  return true;
}

bool MyHandler::endDocument()
{
  qDebug() << "End of the document.";
  return true;
}

bool MyHandler::startElement( const QString &namespaceURI, const QString &localName, 
  const QString &qName, const QXmlAttributes &atts )
{
  qDebug() << "Start of element" << qName;
  for( int i=0; i<atts.length(); ++i )
    qDebug() << "  " << atts.qName(i) << "=" << atts.value(i);

  return true;
}

bool MyHandler::endElement( const QString &namespaceURI, const QString &localName, 
  const QString &qName )
{
  qDebug() << "End of element" << qName;
  return true;
}

bool MyHandler::characters( const QString &ch )
{
  qDebug() << "Characters" << ch;
  return true;
}

int main( int argc, char **argv )
{
  QFile file( "simple.xml" );
  if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
  {
    qDebug( "Failed to open file for reading." );
    return 0;
  }

  QXmlInputSource source( &file );
  
  MyHandler handler;
  
  QXmlSimpleReader reader;
  reader.setContentHandler( &handler );
  reader.parse( source );
  
  file.close();
  
  return 0;
}
