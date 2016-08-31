//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: MSG_HELLO
//
// MessageText:
//
//  Hello
//
#define MSG_HELLO                        0x00000001L

//
// MessageId: MSG_GOODBYE
//
// MessageText:
//
//  Good-bye
//
#define MSG_GOODBYE                      0x00000002L

//
// MessageId: MSG_CAT
//
// MessageText:
//
//  Cat
//
#define MSG_CAT                          0xC0000003L

//
// MessageId: MSG_FISH
//
// MessageText:
//
//  Fish
//
#define MSG_FISH                         0x80000004L

