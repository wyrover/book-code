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

void DateTest::testAddDay()
{
  QDate date( 1979, 5, 16 );
  QCOMPARE( date.year(), 1979 );
  QCOMPARE( date.month(), 5 );
  QCOMPARE( date.day(), 16 );
  
  QDate next = date.addDays( 1 );
  QCOMPARE( next.year(), 1979 );
  QCOMPARE( next.month(), 5 );
  QCOMPARE( next.day(), 17 );
  
  next = date.addDays( 20 );
  QCOMPARE( next.year(), 1979 );
  QCOMPARE( next.month(), 6 );
  QCOMPARE( next.day(), 5 );

  next = date.addDays( 366 );
  QCOMPARE( next.year(), 1980 );
  QCOMPARE( next.month(), 5 );
  QCOMPARE( next.day(), 16 );
}

void DateTest::testAddMonth()
{
  QDate date( 1973, 8, 16 );
  QCOMPARE( date.year(), 1973 );
  QCOMPARE( date.month(), 8 );
  QCOMPARE( date.day(), 16 );
  
  QDate next = date.addMonths( 1 );
  QCOMPARE( next.year(), 1973 );
  QCOMPARE( next.month(), 9 );
  QCOMPARE( next.day(), 16 );
  
  next = date.addMonths( 12 );
  QCOMPARE( next.year(), 1974 );
  QCOMPARE( next.month(), 8 );
  QCOMPARE( next.day(), 16 );
}

void DateTest::testAddYear()
{
  QDate date( 1979, 12, 31 );
  QCOMPARE( date.year(), 1979 );
  QCOMPARE( date.month(), 12 );
  QCOMPARE( date.day(), 31 );
  
  QDate next = date.addYears( 28 );
  QCOMPARE( next.year(), 2007 );
  QCOMPARE( next.month(), 12 );
  QCOMPARE( next.day(), 31 );
}

void DateTest::testValid()
{
  QDate date;
  
  date = QDate();
  QVERIFY( !date.isValid() );
  
  date = QDate( 1979, 5, 16 );
  QVERIFY( date.isValid() );

  date = QDate( 1980, 2, 29 );
  QVERIFY( date.isValid() );
  
  date = QDate( 1979, 2, 29 );
  QVERIFY( !date.isValid() );
}
