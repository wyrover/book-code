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

#include <QProcess>

#include "processdialog.h"

ProcessDialog::ProcessDialog() : QDialog()
{
  process = 0;
  
  ui.setupUi( this );
  
  connect( ui.uicButton, SIGNAL(clicked()), this, SLOT(runUic()) );
}

void ProcessDialog::runUic()
{
  ui.uicButton->setEnabled( false );
  ui.textEdit->setText( "" );
  
  if( process )
    delete process;
  process = new QProcess( this );
  
  connect( process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)) );
  connect( process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(handleFinish(int,QProcess::ExitStatus)) );
  connect( process, SIGNAL(readyReadStandardError()), this, SLOT(handleReadStandardError()) );
  connect( process, SIGNAL(readyReadStandardOutput()), this, SLOT(handleReadStandardOutput()) );
  connect( process, SIGNAL(started()), this, SLOT(handleStarted()) );
  connect( process, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(handleStateChange(QProcess::ProcessState)) );

  QStringList arguments;
  arguments << "-tr" << "MYTR" << "processdialog.ui";
  process->start( "uic", arguments );
}

void ProcessDialog::handleError( QProcess::ProcessError error )
{
  QString errorText;
  
  switch( error )
  {
    case QProcess::FailedToStart:
      errorText = "Failed to start";
      break;
    case QProcess::Crashed:
      errorText = "Crashed";
      break;
    case QProcess::Timedout:
      errorText = "Timed out";
      break;
    case QProcess::WriteError:
      errorText = "Write error";
      break;
    case QProcess::ReadError:
      errorText = "Read error";
      break;
    case QProcess::UnknownError:
      errorText = "Unknown error";
      break;
  }
  
  ui.textEdit->append( QString( "<p><b><font color=red>%1</font></b><p>" ).arg( errorText ) );
}

void ProcessDialog::handleFinish( int code, QProcess::ExitStatus status )
{
  QString statusText;
  
  switch( status )
  {
    case QProcess::NormalExit:
      statusText = "Normal exit";
      break;
    case QProcess::CrashExit:
      statusText = "Crash exit";
      break;
  }
  
  ui.textEdit->append( QString( "<p><b>%1 (%2)</b><p>" ).arg( statusText ).arg( code ) );
}

void ProcessDialog::handleReadStandardError()
{
  QString errorText = process->readAllStandardError();
  ui.textEdit->append( QString( "<font color=red>%1</font>" ).arg( errorText ) );
}

void ProcessDialog::handleReadStandardOutput()
{
  QString outputText = process->readAllStandardOutput();
  ui.textEdit->insertPlainText( outputText );
}

void ProcessDialog::handleStarted()
{
  ui.textEdit->append( QString("<p><b>Started</b><p>" ) );
}

void ProcessDialog::handleStateChange( QProcess::ProcessState state )
{
  QString stateText;
  
  switch( state )
  {
    case QProcess::NotRunning:
      stateText = "Not running";

      ui.uicButton->setEnabled( true );
      break;
    case QProcess::Starting:
      stateText = "Starting";
      break;
    case QProcess::Running:
      stateText = "Running";
      break;
  }
  
  ui.textEdit->append( QString( "<p>New status: <b>%1</b><p>" ).arg( stateText ) );
}
