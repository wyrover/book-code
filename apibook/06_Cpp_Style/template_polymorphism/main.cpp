/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   main.cpp
/// \author Martin Reddy
/// \brief  Showing static polymorphism using templates.
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#include "file.h"

int main(int, char **)
{
	// create a text and image file object
	apibook::TextFile txt_file;
	apibook::ImageFile img_file;

	// use the function template to work on either object
	open_file(txt_file);   // open the text file
	open_file(img_file);   // open the image file

	return 0;
}
