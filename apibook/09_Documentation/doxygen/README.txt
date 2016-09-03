This directory contains sources to demonstrate using the Doxygen API
documentation tool. You will need to download Doxygen and install it
for your platform for this to work. Check out:

    http://www.doxygen.org/

This directory contains a sample class, in version.h, that has various
Doxygen style comments. There is also a Doxygen configuration file and
an API overview document that Doxygen will use to create the front
page of your API documentation.

To use this, simply run doxygen with the provided configuration file,
e.g., on Linux/Mac (assuming that 'doxygen' is in your path):

  % doxygen Doxygen.config

This will generate HTML documentation in the ./htmldoc subdirectory.
Try opening the file ./htmldoc/index.html to view the docs.

Note, the configuration file was generated with Doxygen 1.7.1. If you
are using a newer version of Doxygen, you can update the config file
by running "doxygen -u Doxygen.config".

