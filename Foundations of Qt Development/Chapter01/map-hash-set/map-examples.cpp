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
 
#include <QMap>
#include <QHash>
#include <QSet>

#include <QMultiMap>
#include <QMultiHash>

#include <QString>

#include <QtDebug>

void mapStringIntDemo()
{
  QMap<QString, int> map;
  
  map["foo"] = 42;
  map["bar"] = 13;
  map["baz"] = 9;

  QString key;
  foreach( key, map.keys() )
    qDebug() << key << " = " << map[key];

  QMap<QString, int>::ConstIterator ii;
  for( ii = map.constBegin(); ii != map.constEnd(); ++ii )
    qDebug() << ii.key() << " = " << ii.value();

  int sum; 
  sum = map["foo"] + map.value("ingenting");
  
  foreach( key, map.keys() )
    qDebug() << key << " = " << map[key];

  sum = map["foo"] + map["ingenting"];
  
  foreach( key, map.keys() )
    qDebug() << key << " = " << map[key];
}

void hashStringIntDemo()
{
  QHash<QString, int> hash;
  
  hash["foo"] = 42;
  hash["bar"] = 13;
  hash["baz"] = 9;

  QString key;
  foreach( key, hash.keys() )
    qDebug() << key << " = " << hash[key];

  int sum; 
  sum = hash["foo"] + hash.value("ingenting");
  
  foreach( key, hash.keys() )
    qDebug() << key << " = " << hash[key];

  sum = hash["foo"] + hash["ingenting"];
  
  foreach( key, hash.keys() )
    qDebug() << key << " = " << hash[key];
}

class Person
{
public:
  Person( QString name, QString number );
  
  QString name() const;
  QString number() const;

private:
  QString m_name, m_number;
};

Person::Person( QString name, QString number ) : m_name( name ), m_number( number ) {}
QString Person::name() const { return m_name; }
QString Person::number() const { return m_number; }

bool operator==( const Person &a, const Person &b )
{
  return (a.name() == b.name()) && (a.number() == b.number());
}

uint qHash( const Person &key )
{
  return qHash( key.name() ) ^ qHash( key.number() );
}

void hashPersons()
{
  QHash<Person, int> hash;
  
  hash[ Person( "Anders", "8447070" ) ] = 10;
  hash[ Person( "Micke", "7728433" ) ] = 20;
  
  qDebug() << hash.value( Person( "Anders", "8447070" ) ); // 10
  qDebug() << hash.value( Person( "Anders", "8447071" ) ); // 0
  qDebug() << hash.value( Person( "Micke", "7728433" ) ); // 20
  qDebug() << hash.value( Person( "Michael", "7728433" ) ); // 0
}

void setStringDemo()
{
  QSet<QString> set;
  
  set << "Ada" << "C++" << "Ruby";
  
  for( QSet<QString>::ConstIterator ii = set.begin(); ii != set.end(); ++ii )
    qDebug() << *ii;

  if( set.contains( "FORTRAN" ) )
    qDebug() << "FORTRAN is in the set.";
  else
    qDebug() << "FORTRAN is out.";
}

void multimapStringIntDemo()
{
  QMultiMap<QString, int> multi;
  
  multi.insert( "foo", 10 );
  multi.insert( "foo", 20 );
  multi.insert( "bar", 30 );

  QSet<QString> keys;
  foreach( QString key, multi.keys() )
    keys << key;
    
  foreach( QString key, keys )
    foreach( int value, multi.values(key) )
      qDebug() << key << ": " << value;

  QMultiMap<QString, int>::ConstIterator ii = multi.find( "foo" );
  while( ii != multi.end() && ii.key() == "foo" )
  {
    qDebug() << ii.value();
    ++ii;
  }
}

void multihashStringIntDemo()
{
  QMultiHash<QString, int> multi;
  
  multi.insert( "foo", 10 );
  multi.insert( "foo", 20 );
  multi.insert( "bar", 30 );

  QSet<QString> keys;
  foreach( QString key, multi.keys() )
    keys << key;
    
  foreach( QString key, keys )
    foreach( int value, multi.values(key) )
      qDebug() << key << ": " << value;

  QMultiHash<QString, int>::ConstIterator ii = multi.find( "foo" );
  while( ii != multi.end() && ii.key() == "foo" )
  {
    qDebug() << ii.value();
    ++ii;
  }
}

int main( void )
{
  mapStringIntDemo();
  hashStringIntDemo();
  hashPersons();
  setStringDemo();
  multimapStringIntDemo();
  multihashStringIntDemo();
  
  return 0;
}
