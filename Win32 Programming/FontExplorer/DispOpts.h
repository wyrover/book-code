typedef struct {
        BOOL debug;             // debug mode enabled

        BOOL hrule;             // display horizontal rule
        BOOL vrule;             // display vertical rule
        BOOL chrule;            // display character rule

        BOOL colors;            // use colors for regions       
        BOOL white;             // use white background

        BOOL track;             // enable track kerning
        int  trackval;          // track kerning value (ignored if track FALSE)

        BOOL kerning;           // enable pair kerning

        BOOL basal;             // Use basal TextOut algorithm
               } display_options;
