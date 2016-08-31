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
// MessageId: PRINTERR_FIRST
//
// MessageText:
//
//  All PRINTERR codes are greater than this one
//
#define PRINTERR_FIRST                   0xE0000001L

//
// MessageId: PRINTERR_MEMLOCK_FAILED
//
// MessageText:
//
//  Unable to GlobalLock the PRINTDLG hDevNames structure
//
#define PRINTERR_MEMLOCK_FAILED          0xE0000002L

//
// MessageId: PRINTERR_SETABORT_FAILED
//
// MessageText:
//
//  Severity=Error
//  SetAbort function returned error code
//
#define PRINTERR_SETABORT_FAILED         0xE0000003L

//
// MessageId: PRINTERR_ABORTDLG_FAILED
//
// MessageText:
//
//  Unable to create Abort Dialog
//
#define PRINTERR_ABORTDLG_FAILED         0xE0000004L

//
// MessageId: PRINTERR_STARTDOC_FAILED
//
// MessageText:
//
//  StartDoc function returned error code
//
#define PRINTERR_STARTDOC_FAILED         0xE0000005L

//
// MessageId: PRINTERR_STARTPAGE_FAILED
//
// MessageText:
//
//  StartPage function returned error code
//
#define PRINTERR_STARTPAGE_FAILED        0xE0000006L

//
// MessageId: PRINTERR_ENDPAGE_FAILED
//
// MessageText:
//
//  EndPage function returned error code
//
#define PRINTERR_ENDPAGE_FAILED          0xE0000007L

//
// MessageId: PRINTERR_NO_DC
//
// MessageText:
//
//  PRINTDLG.hDC field was NULL
//
#define PRINTERR_NO_DC                   0xE0000008L

//
// MessageId: PRINTERR_NO_FMTLINE
//
// MessageText:
//
//  FormatLine function NULL
//
#define PRINTERR_NO_FMTLINE              0xE0000009L

//
// MessageId: PRINTERR_STRUCTSIZE
//
// MessageText:
//
//  Incorrect structure size
//
#define PRINTERR_STRUCTSIZE              0xE000000AL

//
// MessageId: PRINTERR_MARGINS
//
// MessageText:
//
//  Vertical margins overlap
//
#define PRINTERR_MARGINS                 0xE000000BL

//
// MessageId: PRINTERR_LAST
//
// MessageText:
//
//  All PRINTERR codes less than this one
//
#define PRINTERR_LAST                    0xE000000CL

