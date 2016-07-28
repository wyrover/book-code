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

#include <QCloseEvent>
#include <QContextMenuEvent>
#include <QEvent>
#include <QFocusEvent>
#include <QHideEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QWheelEvent>

#include "eventwidget.h"

EventWidget::EventWidget( QWidget *parent ) : QWidget( parent )
{
}

void EventWidget::closeEvent( QCloseEvent * event )
{
  emit gotEvent( tr("closeEvent") );
}

void EventWidget::enterEvent( QEvent * event )
{
  emit gotEvent( tr("enterEvent") );
}

void EventWidget::leaveEvent( QEvent * event )
{
  emit gotEvent( tr("leaveEvent") );
}

void EventWidget::hideEvent( QHideEvent * event )
{
  emit gotEvent( tr("hideEvent") );
}

void EventWidget::showEvent( QShowEvent * event )
{
  emit gotEvent( tr("showEvent") );
}

void EventWidget::paintEvent( QPaintEvent * event )
{
  emit gotEvent( tr("paintEvent") );
}

void EventWidget::contextMenuEvent( QContextMenuEvent * event )
{
  emit gotEvent( tr("contextMenuEvent( x:%1, y:%2, reason:%3 )")
    .arg(event->x())
    .arg(event->y())
    .arg(event->reason()==2?"Other":
         event->reason()==1?"Keyboard":
                            "Mouse") );
}

void EventWidget::focusInEvent( QFocusEvent * event )
{
  emit gotEvent( tr("focusInEvent( reason:%1 )")
    .arg( event->reason()==0?"MouseFocusReason":
          event->reason()==1?"TabFocusReason":
          event->reason()==2?"BacktabFocusReason":
          event->reason()==3?"ActiveWindowFocusReason":
          event->reason()==4?"PopupFocusReason":
          event->reason()==5?"ShortcutFocusReason":
          event->reason()==6?"MenuBarFocusReason":
                             "OtherFocusReason" ) );
}

void EventWidget::focusOutEvent( QFocusEvent * event )
{
  emit gotEvent( tr("focusOutEvent( reason:%1 )")
    .arg( event->reason()==0?"MouseFocusReason":
          event->reason()==1?"TabFocusReason":
          event->reason()==2?"BacktabFocusReason":
          event->reason()==3?"ActiveWindowFocusReason":
          event->reason()==4?"PopupFocusReason":
          event->reason()==5?"ShortcutFocusReason":
          event->reason()==6?"MenuBarFocusReason":
                             "OtherFocusReason" ) );
}

void EventWidget::keyPressEvent( QKeyEvent * event )
{
  emit gotEvent( tr("keyPressEvent( text:%1, modifiers:%2 )")
    .arg( event->text() )
    .arg( event->modifiers()==0?tr("NoModifier"):(
         (event->modifiers()&Qt::ShiftModifier      ==0?tr(""):
            tr("ShiftModifier "))+
         (event->modifiers()&Qt::ControlModifier    ==0?tr(""):
            tr("ControlModifier "))+
         (event->modifiers()&Qt::AltModifier        ==0?tr(""):
            tr("AltModifier "))+
         (event->modifiers()&Qt::MetaModifier       ==0?tr(""):
            tr("MetaModifier "))+
         (event->modifiers()&Qt::KeypadModifier     ==0?tr(""):
            tr("KeypadModifier "))+
         (event->modifiers()&Qt::GroupSwitchModifier==0?tr(""):
            tr("GroupSwitchModifier")) ) ) );
}

void EventWidget::keyReleaseEvent( QKeyEvent * event )
{
  emit gotEvent( tr("keyReleaseEvent( text:%1, modifiers:%2 )")
    .arg( event->text() )
    .arg( event->modifiers()==0?tr("NoModifier"):(
         (event->modifiers()&Qt::ShiftModifier      ==0?tr(""):
            tr("ShiftModifier "))+
         (event->modifiers()&Qt::ControlModifier    ==0?tr(""):
            tr("ControlModifier "))+
         (event->modifiers()&Qt::AltModifier        ==0?tr(""):
            tr("AltModifier "))+
         (event->modifiers()&Qt::MetaModifier       ==0?tr(""):
            tr("MetaModifier "))+
         (event->modifiers()&Qt::KeypadModifier     ==0?tr(""):
            tr("KeypadModifier "))+
         (event->modifiers()&Qt::GroupSwitchModifier==0?tr(""):
            tr("GroupSwitchModifier")) ) ) );
}

void EventWidget::mouseDoubleClickEvent( QMouseEvent * event )
{
  emit gotEvent( tr("mouseDoubleClickEvent( x:%1, y:%2, button:%3 )")
    .arg( event->x() )
    .arg( event->y() )
    .arg( event->button()==Qt::LeftButton? "LeftButton":
          event->button()==Qt::RightButton?"RightButton":
          event->button()==Qt::MidButton?  "MidButton":
          event->button()==Qt::XButton1?   "XButton1":
                                           "XButton2" ) );
}

void EventWidget::mouseMoveEvent( QMouseEvent * event )
{
  emit gotEvent( tr("mouseMoveEvent( x:%1, y:%2, button:%3 )")
    .arg( event->x() )
    .arg( event->y() )
    .arg( event->button()==Qt::LeftButton? "LeftButton":
          event->button()==Qt::RightButton?"RightButton":
          event->button()==Qt::MidButton?  "MidButton":
          event->button()==Qt::XButton1?   "XButton1":
                                           "XButton2" ) );
}

void EventWidget::mousePressEvent( QMouseEvent * event )
{
  emit gotEvent( tr("mousePressEvent( x:%1, y:%2, button:%3 )")
    .arg( event->x() )
    .arg( event->y() )
    .arg( event->button()==Qt::LeftButton? "LeftButton":
          event->button()==Qt::RightButton?"RightButton":
          event->button()==Qt::MidButton?  "MidButton":
          event->button()==Qt::XButton1?   "XButton1":
                                           "XButton2" ) );
}

void EventWidget::mouseReleaseEvent( QMouseEvent * event )
{
  emit gotEvent( tr("mouseReleaseEvent( x:%1, y:%2, button:%3 )")
    .arg( event->x() )
    .arg( event->y() )
    .arg( event->button()==Qt::LeftButton? "LeftButton":
          event->button()==Qt::RightButton?"RightButton":
          event->button()==Qt::MidButton?  "MidButton":
          event->button()==Qt::XButton1?   "XButton1":
                                           "XButton2" ) );
}

void EventWidget::resizeEvent( QResizeEvent * event )
{
  emit gotEvent( tr("resizeEvent( w:%1, h:%2 )")
    .arg( event->size().width() )
    .arg( event->size().height() ) );
}

void EventWidget::wheelEvent( QWheelEvent * event )
{
  emit gotEvent( tr("wheelEvent( x:%1, y:%2, delta:%3, orientation:%4 )")
    .arg( event->x() )
    .arg( event->y() )
    .arg( event->delta() ).arg( event->orientation()==1?
      "Horizontal":"Vertical" ) );
}
