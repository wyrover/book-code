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

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "imagedialog.h"

ImageDialog::ImageDialog()
{
  ui.setupUi( this );
  
  currentImage = -1;
  
  updateTags();
  updateImages();
  
  connect( ui.previousButton, SIGNAL(clicked()), this, SLOT(previousClicked()) );
  connect( ui.nextButton, SIGNAL(clicked()), this, SLOT(nextClicked()) );
  connect( ui.addTagButton, SIGNAL(clicked()), this, SLOT(addTagClicked()) );
  connect( ui.addImageButton, SIGNAL(clicked()), this, SLOT(addImageClicked()) );
  connect( ui.tagList, SIGNAL(itemSelectionChanged()), this, SLOT(tagsChanged()) );
}
  
void ImageDialog::updateCurrentImage()
{
  if( currentImage == -1 )
  {
    ui.imageLabel->setPixmap( QPixmap() );
    ui.imageLabel->setText( tr("No Image") );
    
    ui.addTagButton->setEnabled( false );
    ui.nextButton->setEnabled( false );
    ui.previousButton->setEnabled( false );
  }
  else
  {
    ui.imageLabel->setPixmap( QPixmap::fromImage( images.getImage( imageIds[ currentImage ] ) ) );
    ui.imageLabel->setText( "" );
    
    ui.addTagButton->setEnabled( true );
    ui.nextButton->setEnabled( true );
    ui.previousButton->setEnabled( true );
  }
}
  
void ImageDialog::updateImages()
{
  int id;
  
  if( currentImage != -1 )
    id = imageIds[ currentImage ];
  else
    id = -1;
    
  imageIds = images.getIds( selectedTags() );
  currentImage = imageIds.indexOf( id );
  if( currentImage == -1 && imageIds.count() != 0 )
    currentImage = 0;
  
  ui.imagesLabel->setText( QString::number( imageIds.count() ) );
  
  updateCurrentImage();
}

void ImageDialog::updateTags()
{
  QStringList selection = selectedTags();
  
  QStringList tags = images.getTags();
  ui.tagList->clear();
  ui.tagList->addItems( tags );
  
  for( int i=0; i<ui.tagList->count(); ++i )
    if( selection.contains( ui.tagList->item(i)->text() ) )
      ui.tagList->item(i)->setSelected( true );
}

void ImageDialog::nextClicked()
{
  currentImage = (currentImage+1) % imageIds.count();
  updateCurrentImage();
}

void ImageDialog::previousClicked()
{
  currentImage --;
  if( currentImage == -1 )
    currentImage = imageIds.count()-1;
    
  updateCurrentImage();
}

void ImageDialog::tagsChanged()
{
  updateImages();
}

void ImageDialog::addImageClicked()
{
  QString filename = QFileDialog::getOpenFileName( this, tr("Open file"), QString(), tr("PNG Images (*.png)") );
  if( !filename.isNull() )
  {
    QImage image( filename );
    
    if( image.isNull() )
    {
      QMessageBox::warning( this, tr("Image Book"), tr("Failed to open the file '%1'").arg( filename ) );
      return;
    }
    
    images.addImage( image, selectedTags() );
    updateImages();
  }
}

void ImageDialog::addTagClicked()
{
  bool ok;
  QString tag = QInputDialog::getText( this, tr("Image Book"), tr("Tag:"), QLineEdit::Normal, QString(), &ok );
  if( ok )
  {
    tag = tag.toLower();
    QRegExp re( "^[a-z]+$" );
    if( re.indexIn(tag) == -1 )
    {
      QMessageBox::warning( this, tr("Image Book"), tr("This is not a valid tag. Tags consists of lower case characters a-z.") );
      return;
    }

    images.addTag( imageIds[ currentImage ], tag );
    updateTags();
  }
}

QStringList ImageDialog::selectedTags()
{
  QStringList result;
  foreach( QListWidgetItem *item, ui.tagList->selectedItems() )
    result << item->text();
  return result;
}
