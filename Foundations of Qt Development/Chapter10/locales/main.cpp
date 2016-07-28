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

#include <QLocale>

#include <QDate>
#include <QTime>
#include <QString>

#include <QtDebug>

void printDates( QLocale loc )
{
  QLocale::setDefault( loc );

  QDate d1( 2006, 10, 12 );
  QDate d2( 2006, 01, 31 );
  QDate d3( 2006, 06, 06 );  
    
  qDebug() << "short";
  qDebug() << loc.toString( d1, QLocale::ShortFormat );
  qDebug() << loc.toString( d2, QLocale::ShortFormat );
  qDebug() << loc.toString( d3, QLocale::ShortFormat );

  qDebug() << "long";
  qDebug() << loc.toString( d1, QLocale::LongFormat );
  qDebug() << loc.toString( d2, QLocale::LongFormat );
  qDebug() << loc.toString( d3, QLocale::LongFormat );

  qDebug() << "default";
  qDebug() << loc.toString( d1 );
  qDebug() << loc.toString( d2 );
  qDebug() << loc.toString( d3 );
}

void printTimes( QLocale loc )
{
  QLocale::setDefault( loc );
    
  QTime t1( 6, 15, 45 );
  QTime t2( 12, 00, 00 );
  QTime t3( 18, 20, 25 );
  
  qDebug() << "short";
  qDebug() << loc.toString( t1, QLocale::ShortFormat );
  qDebug() << loc.toString( t2, QLocale::ShortFormat );
  qDebug() << loc.toString( t3, QLocale::ShortFormat );
    
  qDebug() << "long";
  qDebug() << loc.toString( t1, QLocale::LongFormat );
  qDebug() << loc.toString( t2, QLocale::LongFormat );
  qDebug() << loc.toString( t3, QLocale::LongFormat );

  qDebug() << "default";
  qDebug() << loc.toString( t1 );
  qDebug() << loc.toString( t2 );
  qDebug() << loc.toString( t3 );
}

void printValues( QLocale loc )
{
  QLocale::setDefault( loc );
    
  double v1 = 3.1415;
  double v2 = 31415;
  double v3 = 1000.001;
  
  qDebug() << loc.toString( v1 );
  qDebug() << loc.toString( v2 );
  qDebug() << loc.toString( v3 );
}

int main( int argc, char **argv )
{
  QApplication app( argc, argv );
  
  qDebug() << "Dates";
  qDebug() << "Swedish";
  printDates( QLocale( QLocale::Swedish, QLocale::Sweden ) );
  qDebug() << "US English";
  printDates( QLocale( QLocale::English, QLocale::UnitedStates ) );

  qDebug() << "Times";
  qDebug() << "Swedish";
  printTimes( QLocale( QLocale::Swedish, QLocale::Sweden ) );
  qDebug() << "US English";
  printTimes( QLocale( QLocale::English, QLocale::UnitedStates ) );

  qDebug() << "Values";
  qDebug() << "Swedish";
  printValues( QLocale( QLocale::Swedish, QLocale::Sweden ) );
  qDebug() << "US English";
  printValues( QLocale( QLocale::English, QLocale::UnitedStates ) );
}
