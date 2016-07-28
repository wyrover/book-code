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

#include <QtTest>
#include <QtSql>

#include "imagecollection.h"

#include "imagecollectiontest.h"

void ImageCollectionTest::testTags()
{
  ImageCollection c;
  
  // Make sure that the collection is empty
  QCOMPARE( c.getTags().count(), 0 );
  
  // At least one image is needed to be able to add tags
  c.addImage( QImage( "test.png" ), QStringList() );
  
  // Verify that we have one image and get the id for it
  QList<int> ids = c.getIds( QStringList() );
  QCOMPARE( ids.count(), 1 );
  int id = ids[0];
  
  // Add one tag, total one
  c.addTag( id, "Foo" );
  QCOMPARE( c.getTags().count(), 1 );

  // Add one tag, total two
  c.addTag( id, "Bar" );
  QCOMPARE( c.getTags().count(), 2 );
  
  // Add one tag, total three
  c.addTag( id, "Baz" );
  QCOMPARE( c.getTags().count(), 3 );
  
  // Add a duplicate tag, total three
  c.addTag( id, "Foo" );
  QCOMPARE( c.getTags().count(), 3 );
  
  // Try to add a tag to a nonexisting id
  QEXPECT_FAIL("", "The tag will be added to the non-existing image.", Continue);
  c.addTag( id+1, "Foz" );
  QCOMPARE( c.getTags().count(), 3 );
  
  QSqlDatabase::removeDatabase( QLatin1String( QSqlDatabase::defaultConnection ) );
}

void ImageCollectionTest::testImages()
{
  ImageCollection c;
  
  QCOMPARE( c.getIds( QStringList() ).count(), 0 );
  
  QImage image( "test.png" );
  c.addImage( image, QStringList() );
   
  // Verify that we have one image and get the id for it
  QList<int> ids = c.getIds( QStringList() );
  QCOMPARE( ids.count(), 1 );
  int id = ids[0];
  
  QImage fromDb = c.getImage( id );
  QVERIFY( pixelCompareImages( image, fromDb ) );
  
// Will call qFatal and end the application
//  QTest::ignoreMessage( QtFatalMsg, "Failed to get image id" );
//  fromDb = c.getImage( id+1 );
//  QVERIFY( fromDb.isNull() );
  
  QSqlDatabase::removeDatabase( QLatin1String( QSqlDatabase::defaultConnection ) );
}

void ImageCollectionTest::testImagesFromTags()
{
  ImageCollection c;
  
  QCOMPARE( c.getIds( QStringList() ).count(), 0 );
  
  QImage image( "test.png" );
  
  QStringList tags;
  tags << "Foo" << "Bar";
  
  c.addImage( image, tags );
  QCOMPARE( c.getTags().count(), 2 );
  QCOMPARE( c.getIds( QStringList() ).count(), 1 );
  QCOMPARE( c.getIds( QStringList() << "Foo" ).count(), 1 );
  QCOMPARE( c.getIds( QStringList() << "Bar" ).count(), 1 );
  QCOMPARE( c.getIds( tags ).count(), 1 );
  QCOMPARE( c.getIds( QStringList() << "Baz" ).count(), 0 );
  
  tags.clear();
  tags << "Baz";
  c.addImage( image, tags );
  QCOMPARE( c.getTags().count(), 3 );
  QCOMPARE( c.getIds( QStringList() ).count(), 2 );
  QCOMPARE( c.getIds( QStringList() << "Foo" ).count(), 1 );
  QCOMPARE( c.getIds( QStringList() << "Bar" ).count(), 1 );
  QCOMPARE( c.getIds( QStringList() << "Baz" ).count(), 1 );

  tags.clear();
  tags << "Bar" << "Baz";
  c.addImage( image, tags );
  QCOMPARE( c.getTags().count(), 3 );
  QCOMPARE( c.getIds( QStringList() ).count(), 3 );
  QCOMPARE( c.getIds( QStringList() << "Foo" ).count(), 1 );
  QCOMPARE( c.getIds( QStringList() << "Bar" ).count(), 2 );
  QCOMPARE( c.getIds( tags ).count(), 3 );
  QCOMPARE( c.getIds( QStringList() << "Baz" ).count(), 2 );
  
  QSqlDatabase::removeDatabase( QLatin1String( QSqlDatabase::defaultConnection ) );
}

bool ImageCollectionTest::pixelCompareImages( const QImage &a, const QImage &b )
{
  if( a.size() != b.size() )
    return false;
    
  if( a.format() != b.format() )
    return false;
  
  for( int x=0; x<a.width(); ++x )
    for( int y=0; y<a.height(); ++y )
      if( a.pixel(x,y) != b.pixel(x,y) )
        return false;
        
  return true;
}
