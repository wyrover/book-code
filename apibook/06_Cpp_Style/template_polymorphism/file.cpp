/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   file.cpp
/// \author Martin Reddy
/// \brief  Showing static polymorphism using templates.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "file.h"
#include <iostream>

using std::cout;
using std::endl;

namespace apibook {

void TextFile::Open()
{ 
	cout << "TextFile.Open()" << endl;
}

void ImageFile::Open()
{
	cout << "ImageFile.Open()" << endl;
}

}
