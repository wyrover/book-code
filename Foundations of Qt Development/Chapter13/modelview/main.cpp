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

#include <QApplication>

#include <QTableView>

#include <QtSql>

void relModel()
{
  QSqlRelationalTableModel *model = new QSqlRelationalTableModel();
  
  model->setTable( "names" );
  model->setRelation( 0, QSqlRelation( "salaries", "id", "annual" ) );
  model->select();
  
  model->setHeaderData( 0, Qt::Horizontal, QObject::tr("Annual Pay") );
  model->setHeaderData( 1, Qt::Horizontal, QObject::tr("First Name") );
  model->setHeaderData( 2, Qt::Horizontal, QObject::tr("Last Name") );
    
  QTableView *view = new QTableView();
  view->setModel( model );
  view->show();
}

void tabModel()
{
  QSqlTableModel *model = new QSqlTableModel();
  
  model->setTable( "names" );
  model->setFilter( "lastname = 'Doe'" );
  model->select();
  
  model->removeColumn( 0 );
    
  QTableView *view = new QTableView();
  view->setModel( model );
  view->show();
}

void qryModel()
{
  QSqlQueryModel *model = new QSqlQueryModel();
  model->setQuery( "SELECT firstname, lastname FROM names" );
    
  QTableView *view = new QTableView();
  view->setModel( model );
  view->show();
}


int main( int argc, char **argv )
{
  QApplication app( argc, argv );

  QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );

  db.setDatabaseName( ":memory:" );
  
  if( !db.open() )
  {
    qDebug() << db.lastError();
    qFatal( "Failed to connect." );
  }
    
  qDebug( "Connected!" );
  
  QSqlQuery qry;

  qry.prepare( "CREATE TABLE IF NOT EXISTS names (id INTEGER UNIQUE PRIMARY KEY, firstname VARCHAR(30), lastname VARCHAR(30))" );
  qry.exec();
    
  qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (1, 'John', 'Doe')" );
  qry.exec();

  qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (2, 'Jane', 'Doe')" );
  qry.exec();

  qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (3, 'James', 'Doe')" );
  qry.exec();

  qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (4, 'Judy', 'Doe')" );
  qry.exec();

  qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (5, 'Richard', 'Roe')" );
  qry.exec();

  qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (6, 'Jane', 'Roe')" );
  qry.exec();

  qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (7, 'John', 'Noakes')" );
  qry.exec();

  qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (8, 'Donna', 'Doe')" );
  qry.exec();

  qry.prepare( "INSERT INTO names (id, firstname, lastname) VALUES (:id, :firstname, :lastname)" );
  qry.bindValue( ":id", 9 );
  qry.bindValue( ":firstname", "Ralph" );
  qry.bindValue( ":lastname", "Roe" );
  qry.exec();

  qry.prepare( "CREATE TABLE IF NOT EXISTS salaries (id INTEGER UNIQUE PRIMARY KEY, annual INTEGER)" );
  qry.exec();

  qry.prepare( "INSERT INTO salaries (id, annual) VALUES (1, 1000)" );
  qry.exec();

  qry.prepare( "INSERT INTO salaries (id, annual) VALUES (2, 900)" );
  qry.exec();

  qry.prepare( "INSERT INTO salaries (id, annual) VALUES (3, 900)" );
  qry.exec();

  qry.prepare( "INSERT INTO salaries (id, annual) VALUES (5, 1100)" );
  qry.exec();

  qry.prepare( "INSERT INTO salaries (id, annual) VALUES (6, 1000)" );
  qry.exec();

  qry.prepare( "INSERT INTO salaries (id, annual) VALUES (8, 1200)" );
  qry.exec();

  qry.prepare( "INSERT INTO salaries (id, annual) VALUES (9, 1200)" );
  qry.exec();

  relModel();
  tabModel();
  qryModel();
  
  return app.exec();
}
