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

#include <QtSql>
#include <QImageReader>
#include <QImageWriter>

#include "imagecollection.h"

ImageCollection::ImageCollection()
{
  QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
  
  db.setDatabaseName( ":memory:" );
  if( !db.open() )
    qFatal( "Failed to open database" );
  
  populateDatabase();
}

void ImageCollection::populateDatabase()
{
  QSqlQuery qry;
  
  qry.prepare( "CREATE TABLE IF NOT EXISTS images (id INTEGER UNIQUE PRIMARY KEY, data BLOB)" );
  if( !qry.exec() )
    qFatal( "Failed to create table images" );
  
  qry.prepare( "CREATE TABLE IF NOT EXISTS tags (id INTEGER, tag VARCHAR(30))" );
  if( !qry.exec() )
    qFatal( "Failed to create table tags" );
} 
  
QList<int> ImageCollection::getIds( QStringList tags )
{
  QSqlQuery qry;
  
  if( tags.count() == 0 )
    qry.prepare( "SELECT images.id FROM images" );
  else
    qry.prepare( "SELECT id FROM tags WHERE tag IN ('" + tags.join("','") + "') GROUP BY id" );
    
  if( !qry.exec() )
    qFatal( "Failed to get IDs" );
  
  QList<int> result;
  while( qry.next() )
    result << qry.value(0).toInt();
    
  return result;
}
  
QStringList ImageCollection::getTags()
{
  QSqlQuery qry;
  
  qry.prepare( "SELECT tag FROM tags GROUP BY tag" );
  if( !qry.exec() )
    qFatal( "Failed to get tags" );
  
  QStringList result;
  while( qry.next() )
    result << qry.value(0).toString();
  
  return result;
}
  
void ImageCollection::addTag( int id, QString tag )
{
  QSqlQuery qry;
  
  qry.prepare( "INSERT INTO tags (id, tag) VALUES (:id, :tag)" );
  qry.bindValue( ":id", id );
  qry.bindValue( ":tag", tag );
  if( !qry.exec() )
    qFatal( "Failed to add tag" );
}

QImage ImageCollection::getImage( int id )
{
  QSqlQuery qry;
  
  qry.prepare( "SELECT data FROM images WHERE id = :id" );
  qry.bindValue( ":id", id );
  if( !qry.exec() )
    qFatal( "Failed to get image" );
  if( !qry.next() )
    qFatal( "Failed to get image id" );

  QByteArray array = qry.value(0).toByteArray();
  QBuffer buffer(&array);
  buffer.open( QIODevice::ReadOnly );
  
  QImageReader reader(&buffer, "PNG");
  QImage image = reader.read();
  
  return image;
}

void ImageCollection::addImage( QImage image, QStringList tags )
{
  QBuffer buffer;
  QImageWriter writer(&buffer, "PNG");
  writer.write(image);

  QSqlQuery qry;
  
  int id;
  
  qry.prepare( "SELECT COUNT(*) FROM images" );
  qry.exec();
  qry.next();
  id = qry.value(0).toInt() + 1;
  
  qry.prepare( "INSERT INTO images (id, data) VALUES (:id, :data)" );
  qry.bindValue( ":id", id );
  qry.bindValue( ":data", buffer.data() );
  qry.exec();
  
  foreach( QString tag, tags )
    addTag( id, tag );
}
