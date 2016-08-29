treedata Demo1Data[];

// Indicates the type of node, which we will use later to
// determine how to respond to TVN_ITEMEXPANDED messages

// Note that these numbers directly relate to the levels.  
// BOTANY_HIGH is the highest level.  BOTANY_HIGH - 1 is the next
// lower level, etc.

#define BOTANY_HIGH  16384
#define DEMO_ORDER   16384
#define DEMO_FAMILY  16383
#define DEMO_GENUS   16382
#define DEMO_SPECIES 16381
#define DEMO_COMMON  16380
#define BOTANY_LOW   16380

#define IMAGE_BOOKS_UNSELECTED  1
#define IMAGE_BOOKS_SELECTED    2
#define IMAGE_BOOK_UNSELECTED   3
#define IMAGE_BOOK_SELECTED     4
#define IMAGE_FOLDER_UNSELECTED 5
#define IMAGE_FOLDER_SELECTED   6
#define IMAGE_FLOWER_UNSELECTED 7
#define IMAGE_FLOWER_SELECTED   8
