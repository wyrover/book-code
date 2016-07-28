// Listing 64-13. Declaring Formatting Flags with Bitfields
struct fmtflags
{
   bool skipws_f :         1;
   bool boolalpha_f:       1;
   bool showpoint_f:       1;
   bool showbase_f:        1;
   bool showpos_f:         1;
   unsigned adjustfield_f: 2;
   unsigned floatfield_f:  2;

   static unsigned constexpr left     = 0;
   static unsigned constexpr right    = 1;
   static unsigned constexpr internal = 2;

   static unsigned constexpr fixed      = 1;
   static unsigned constexpr scientific = 2;
   static unsigned constexpr hexfloat = 3;
};
