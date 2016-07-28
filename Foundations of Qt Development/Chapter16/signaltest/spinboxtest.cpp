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

#include <QSpinBox>

#include "spinboxtest.h"

Q_DECLARE_METATYPE ( Qt::Key )

void SpinBoxTest::testKeys()
{
  QSpinBox spinBox;
  spinBox.setRange( 1, 10 );
  
  QFETCH( Qt::Key, key );
  QFETCH( int, startValue );
  
  spinBox.setValue( startValue );
  
  QSignalSpy spy( &spinBox, SIGNAL(valueChanged(int)) );
  
  QTest::keyClick( &spinBox, key );
  QTEST( spinBox.value(), "endValue" );
  
  QFETCH( bool, willSignal );
  if( willSignal )
  {
    QCOMPARE( spy.count(), 1 );
    QTEST( spy.takeFirst()[0].toInt(), "endValue" );
  }
  else
    QCOMPARE( spy.count(), 0 );
}

void SpinBoxTest::testKeys_data()
{
  QTest::addColumn<Qt::Key>( "key" );
  QTest::addColumn<int>( "startValue" );
  QTest::addColumn<int>( "endValue" );
  QTest::addColumn<bool>( "willSignal" );
    
  QTest::newRow( "Up" ) << Qt::Key_Up << 5 << 6 << true;
  QTest::newRow( "Down" ) << Qt::Key_Down << 5 << 4 << true;
  QTest::newRow( "Up, limit" ) << Qt::Key_Up << 10 << 10 << false;
  QTest::newRow( "Down, limit" ) << Qt::Key_Down << 1 << 1 << false;
}

void SpinBoxTest::testClicks()
{
  QSpinBox spinBox;  
  spinBox.setRange( 1, 10 );
  
  QSize size = spinBox.size();
  QPoint upButton = QPoint( size.width()-2, 2 );
  QPoint downButton = QPoint( size.width()-2, size.height()-2 );

  QFETCH( QString, direction );
  QFETCH( int, startValue );
  
  spinBox.setValue( startValue );

  QSignalSpy spy( &spinBox, SIGNAL(valueChanged(int)) );
  
  if( direction.toLower() == "up" )
    QTest::mouseClick( &spinBox, Qt::LeftButton, 0, upButton );
  else if (direction.toLower() == "down" )
    QTest::mouseClick( &spinBox, Qt::LeftButton, 0, downButton );
  else
    QWARN( "Unknown direction - no clicks issued." );
  
  QTEST( spinBox.value(), "endValue" );
  
  QFETCH( bool, willSignal );
  if( willSignal )
  {
    QCOMPARE( spy.count(), 1 );
    QTEST( spy.takeFirst()[0].toInt(), "endValue" );
  }
  else
    QCOMPARE( spy.count(), 0 );
}

void SpinBoxTest::testClicks_data()
{
  QTest::addColumn<QString>( "direction" );
  QTest::addColumn<int>( "startValue" );
  QTest::addColumn<int>( "endValue" );
  QTest::addColumn<bool>( "willSignal" );
    
  QTest::newRow( "Up" ) << "Up" << 5 << 6 << true;
  QTest::newRow( "Down" ) << "Down" << 5 << 4 << true;
  QTest::newRow( "Up, limit" ) << "Up" << 10 << 10 << false;
  QTest::newRow( "Down, limit" ) << "Down" << 1 << 1 << false;
}

void SpinBoxTest::testSetting()
{
  QSpinBox spinBox;
  spinBox.setRange( 1, 10 );
  
  QFETCH( int, startValue );
  spinBox.setValue( startValue );
  
  QSignalSpy spy( &spinBox, SIGNAL(valueChanged(int)) );  
  
  QFETCH( int, value );
  spinBox.setValue( value );
  QTEST( spinBox.value(), "endValue" );
  
  QFETCH( bool, willSignal );
  if( willSignal )
  {
    QCOMPARE( spy.count(), 1 );
    QTEST( spy.takeFirst()[0].toInt(), "endValue" );
  }
  else
    QCOMPARE( spy.count(), 0 );
}

void SpinBoxTest::testSetting_data()
{
  QTest::addColumn<int>( "startValue" );
  QTest::addColumn<int>( "value" );
  QTest::addColumn<int>( "endValue" );
  QTest::addColumn<bool>( "willSignal" );
    
  QTest::newRow( "Valid" ) << 1 << 5 << 5 << true;
  QTest::newRow( "Over" ) << 9 << 11 << 10 << true;
  QTest::newRow( "Under" ) << 2 << 0 << 1 << true;
  QTest::newRow( "Valid, no change" ) << 5 << 5 << 5 << false;
  QTest::newRow( "Over, no change" ) << 10 << 11 << 10 << false;
  QTest::newRow( "Under, no change" ) << 1 << 0 << 1 << false;
}
