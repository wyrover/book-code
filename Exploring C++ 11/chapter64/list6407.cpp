// Listing 64-7. Using Shift Operators to Define the Formatting Fields
int static constexpr boolalpha_pos = 0;
int static constexpr showbase_pos  = 1;
int static constexpr showpoint_pos = 2;
int static constexpr showpos_pos   = 3;
int static constexpr skipws_pos    = 4;
int static constexpr adjust_pos    = 5;
int static constexpr adjust_size   = 2;
int static constexpr float_pos     = 7;
int static constexpr float_size    = 2;

fmtflags static constexpr boolalpha   = 1 << boolalpha_pos;
fmtflags static constexpr showbase    = 1 << showbase_pos;
fmtflags static constexpr showpos     = 1 << showpos_pos;
fmtflags static constexpr showpoint   = 1 << showpoint_pos;
fmtflags static constexpr skipws      = 1 << showpoint_pos;
fmtflags static constexpr adjustfield = 3 << adjust_pos;
fmtflags static constexpr floatfield  = 3 << float_pos;

fmtflags static constexpr left     = 0 << adjust_pos;
fmtflags static constexpr right    = 1 << adjust_pos;
fmtflags static constexpr internal = 2 << adjust_pos;

fmtflags static constexpr fixed      = 1 << float_pos;
fmtflags static constexpr scientific = 2 << float_pos;
fmtflags static constexpr hexfloat = 3 << float_pos;
