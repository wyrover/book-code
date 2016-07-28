// Listing 64-1. An Initial Definition of Formatting Flags
typedef int fmtflags;
fmtflags const showbase  = 0x01;
fmtflags const boolalpha = 0x02;
fmtflags const skipws    = 0x04;
fmtflags const showpoint = 0x08;
fmtflags const showpos   = 0x10;
// etc. for other flags...
