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

void SpinBoxTest::testKeys()
{
  QSpinBox spinBox;
  
  spinBox.setRange( 1, 10 );
  spinBox.setValue( 5 );
  
  QTest::keyClick( &spinBox, Qt::Key_Up );
  QCOMPARE( spinBox.value(), 6 );
  
  QTest::keyClick( &spinBox, Qt::Key_Down );
  QCOMPARE( spinBox.value(), 5 );
  
  spinBox.setValue( 10 );
  QTest::keyClick( &spinBox, Qt::Key_Up );
  QCOMPARE( spinBox.value(), 10 );
  
  spinBox.setValue( 1 );
  QTest::keyClick( &spinBox, Qt::Key_Down );
  QCOMPARE( spinBox.value(), 1 );  
}

void SpinBoxTest::testClicks()
{
  QSpinBox spinBox;
  
  spinBox.setRange( 1, 10 );
  spinBox.setValue( 5 );
  
  QSize size = spinBox.size();
  QPoint upButton = QPoint( size.width()-2, 2 );
  QPoint downButton = QPoint( size.width()-2, size.height()-2 );
  
  QTest::mouseClick( &spinBox, Qt::LeftButton, 0, upButton );
  QCOMPARE( spinBox.value(), 6 );
  
  QTest::mouseClick( &spinBox, Qt::LeftButton, 0, downButton );
  QCOMPARE( spinBox.value(), 5 );
  
  spinBox.setValue( 10 );
  QTest::mouseClick( &spinBox, Qt::LeftButton, 0, upButton );
  QCOMPARE( spinBox.value(), 10 );
  
  spinBox.setValue( 1 );
  QTest::mouseClick( &spinBox, Qt::LeftButton, 0, downButton );
  QCOMPARE( spinBox.value(), 1 );  
}

void SpinBoxTest::testSetting()
{
  QSpinBox spinBox;
  
  spinBox.setRange( 1, 10 );

  spinBox.setValue( 5 );
  QCOMPARE( spinBox.value(), 5 );
  
  spinBox.setValue( 0 );
  QCOMPARE( spinBox.value(), 1 );

  spinBox.setValue( 11 );
  QCOMPARE( spinBox.value(), 10 );
}
