// The following macros take units in 0.1 mm and convert to fractional
// and integer parts of mm or inches

#define mm_1_Int(mm_1) ((mm_1)/10)
#define mm_1_Frac(mm_1) ((mm_1) % 10)
#define in_001_Int(mm_1) ((mm_1)/254)
#define in_001_Frac(mm_1) ((((mm_1) * 1000) / 254) % 1000)

#define MM_1_FMT "%d.%d"
#define IN_001_FMT "%d.%03d"
