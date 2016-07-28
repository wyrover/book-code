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

#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QGroupBox>

#include <QGridLayout>

#include "whatsthisdialog.h"

WhatsThisDialog::WhatsThisDialog() : QDialog()
{
  QGroupBox *groupBox = new QGroupBox( tr("Group") );
  QGridLayout *gbLayout = new QGridLayout( groupBox );
  
  QCheckBox *checkBox = new QCheckBox( tr("Check!") );
  QLabel *label = new QLabel( tr("label") );
  QPushButton *pushButton = new QPushButton( tr("Push me!") );
  
  gbLayout->addWidget( checkBox, 0, 0 );
  gbLayout->addWidget( label, 0, 1 );
  gbLayout->addWidget( pushButton, 1, 0, 1, 2 );
  
  QGridLayout *dlgLayout = new QGridLayout( this );
  
  dlgLayout->addWidget( groupBox, 0, 0 );

#ifndef true
  checkBox->setWhatsThis( tr("This is a simple <i>What's This help</i> for the check box.") );
#endif
  checkBox->setWhatsThis( tr("<p style='white-space:pre'>This is a simple <i>What's This help</i> "
                             "for the check box.</p>") );
  groupBox->setWhatsThis( tr("This is a group box <i>What's This help</i>.<br>"
                             "Notice that it appears between and around the contained widgets.<br>"
                             "It is also spanning several lines.") );
  label->setWhatsThis( tr("<p>It is possible to do lists.</p>"
                          "<ul>"
                            "<li>You can <i>format</i> text.</li>"
                            "<li><b>Bold</b> is possible too.</li>"
                            "<li>And the <font color='#22aaff'>color</font> and "
                              "<font size='+2'>size</font>.</li>"
                          "</ul>"
                          "<p>You can do ordered lists as well.</p>"
                          "<ol>"
                            "<li>First.</li>"
                            "<li>Second.</li>"
                            "<li>Third.</li>"
                          "</ol>") );
  pushButton->setWhatsThis( tr("<img src=':/images/qt.png'>"
                               "You can also insert images into your <i>What's This help</i>.") );
}
