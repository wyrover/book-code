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

#include <QDate>

#include "datetest.h"

void DateTest::testAdd()
{
  QDate date( 1979, 5, 16 );
  
  QFETCH( int, addYear );
  QFETCH( int, addMonth );
  QFETCH( int, addDay );
  
  QDate next = date.addYears( addYear ).addMonths( addMonth ).addDays( addDay );
  
  QTEST( next.year(), "year" );
  QTEST( next.month(), "month" );
  QTEST( next.day(), "day" );
}

void DateTest::testAdd_data ()
{
  QTest::addColumn<int>( "addYear" );
  QTest::addColumn<int>( "addMonth" );
  QTest::addColumn<int>( "addDay" );
  QTest::addColumn<int>( "year" );
  QTest::addColumn<int>( "month" );
  QTest::addColumn<int>( "day" );
    
  QTest::newRow( "Start date" )    << 0 << 0 << 0 << 1979 << 5 << 16;
  QTest::newRow( "One day" )       << 0 << 0 << 1 << 1979 << 5 << 17;
  QTest::newRow( "Twenty days" )   << 0 << 0 << 20 << 1979 << 6 << 5;
  QTest::newRow( "366 days" )      << 0 << 0 << 366 << 1980 << 5 << 16;
  QTest::newRow( "One month" )     << 0 << 1 << 0 << 1979 << 6 << 16;
  QTest::newRow( "Twelve months" ) << 0 << 12 << 0 << 1980 << 5 << 16;
  QTest::newRow( "28 years" )      << 28 << 0 << 0 << 2007 << 5 << 16;
}

void DateTest::testValid()
{
  QFETCH( int, year );
  QFETCH( int, month );
  QFETCH( int, day );
  
  QDate date( year, month, day );
  QTEST( date.isValid(), "valid" );
}

void DateTest::testValid_data()
{
  QTest::addColumn<int>( "year" );
  QTest::addColumn<int>( "month" );
  QTest::addColumn<int>( "day" );
  QTest::addColumn<bool>( "valid" );
    
  QTest::newRow( "Valid, normal" ) << 1973 << 8 << 16 << true;
  QTest::newRow( "Invalid, normal" ) << 1973 << 9 << 31 << false;
  QTest::newRow( "Valid, leap-year" ) << 1980 << 2 << 29 << true;
  QTest::newRow( "Invalid, leap-year" ) << 1981 << 2 << 29 << false;
}
