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

#include <QPluginLoader>
#include <QDir>
#include <QPixmap>

#include "filterinterface.h"

#include "filterdialog.h"

FilterDialog::FilterDialog( QWidget *parent ) : QDialog( parent )
{
  ui.setupUi( this );
  ui.originalLabel->setPixmap( QPixmap( "source.jpeg" ) );
  
  connect( ui.filterList, SIGNAL(currentTextChanged(QString)), this, SLOT(filterChanged(QString)) );
  
  findFilters();  
  filterChanged( QString() );
}

void FilterDialog::findFilters()
{
  foreach( QObject *couldBeFilter, QPluginLoader::staticInstances() )
  {
    FilterInterface *filter = qobject_cast<FilterInterface*>( couldBeFilter );
    if( filter )
    {
      filters[ filter->name() ] = filter;
      ui.filterList->addItem( filter->name() );
    }
  }
   
  QDir path( "./plugins" );
  
  foreach( QString filename, path.entryList(QDir::Files) )
  {
    QPluginLoader loader( path.absoluteFilePath( filename ) );
    QObject *couldBeFilter = loader.instance();
    if( couldBeFilter )
    {
      FilterInterface *filter = qobject_cast<FilterInterface*>( couldBeFilter );
      if( filter )
      {
        filters[ filter->name() ] = filter;
        ui.filterList->addItem( filter->name() );
      }
    }
  }
}

void FilterDialog::filterChanged( QString filter )
{
  if( filter.isEmpty() )
  {
    ui.filteredLabel->setPixmap( *(ui.originalLabel->pixmap() ) );
  }
  else
  {
    QImage filtered = filters[ filter ]->filter( ui.originalLabel->pixmap()->toImage() );
    ui.filteredLabel->setPixmap( QPixmap::fromImage( filtered ) );
  }
}

