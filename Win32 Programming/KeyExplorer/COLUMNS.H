// All dimensions are 1/1000ths of an inch

#define COLUMN_MESSAGE_INDENT 125

#define COLUMN_MESSAGE     0
#define COLUMN_KEYCODE     1
#define COLUMN_REPEATCOUNT 2
#define COLUMN_SCANCODE    3
#define COLUMN_EXTENDED    4
#define COLUMN_CONTEXT     5
#define COLUMN_UPDOWN      6
#define COLUMN_TRANSITION  7

typedef struct{
		int * table;
		int caption;
		int start;
		int width;
	      } widthtable;

extern widthtable column_widths[8];


