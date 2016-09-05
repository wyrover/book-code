This directory creates a unit test using GMock, the mock object
library from Google. You will need to install the GMock run time into
a "gmock" subdirectory for this to work properly (which is why this
directory is not built by default), i.e., on Linux/Mac you should have
a directory structure like:

  wargame/
      gmock/
          include/
          lib/
          bin/
          share/

You can download GMock from:

  http://code.google.com/p/gmock/

