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

#include <QtPlugin>
#include <QExtensionManager>
#include <QDesignerFormEditorInterface>
#include <QTimer>

#include "circlebar.h"
#include "circlebarplugin.h"

CircleBarPlugin::CircleBarPlugin( QObject *parent )
{
    m_initialized = false;
}

bool CircleBarPlugin::isInitialized() const
{
    return m_initialized;
}

void CircleBarPlugin::initialize( QDesignerFormEditorInterface *core )
{
    if( m_initialized )
        return;

    m_initialized = true;
}

bool CircleBarPlugin::isContainer() const
{
    return false;
}

QIcon CircleBarPlugin::icon() const
{
    return QIcon();
}

QString CircleBarPlugin::toolTip() const
{
    return "";
}

QString CircleBarPlugin::whatsThis() const
{
    return "";
}

QString CircleBarPlugin::codeTemplate() const
{
    return "";
}

QString CircleBarPlugin::includeFile() const
{
    return "circlebar.h";
}

QString CircleBarPlugin::name() const
{
    return "CircleBar";
}

QString CircleBarPlugin::domXml() const
{
    return "<widget class=\"CircleBar\" name=\"circleBar\">\n"
           "</widget>\n";
}

QString CircleBarPlugin::group() const
{
    return "Book Widgets";
}

QWidget *CircleBarPlugin::createWidget( QWidget *parent )
{
    return new CircleBar( parent );
}

Q_EXPORT_PLUGIN2( circleBarPlugin, CircleBarPlugin )
