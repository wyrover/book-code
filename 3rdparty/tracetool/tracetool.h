// tracetool.h
//
// Author : Thierry Parent
// Version : 12.4
//
// HomePage :  http://www.codeproject.com/csharp/TraceTool.asp
// Download :  http://sourceforge.net/projects/tracetool/
// See License.txt for license information (GPL)

#pragma once

#ifdef _DEBUG
    #define TRACE_TOOL
#endif

#ifndef NOEXCEPT
	#define NOEXCEPT    throw()
#endif

#ifdef _X86_
	inline void debugBreak() NOEXCEPT{ _asm { int 3 } }
#else
	//inline void debugBreak() NOEXCEPT{ DebugBreak(); }
#endif

#ifdef TRACE_TOOL
    #define STRINGIFY(x) #x
    #define TOSTRING(x) STRINGIFY(x)
    #define STRINGIFY_W(x) L#x
    #define TOSTRING_W(x) STRINGIFY_W(x)
    #define __STR2WSTR(str) L##str
    #define _STR2WSTR(str) __STR2WSTR(str)
    #define __FUNCTIONW__ _STR2WSTR(__FUNCTION__)
	//Serialization.cpp(226) : warning C4189: “nCount”: 局部变量已初始化但不引用
	//#define DO_PRAGMA(x) __pragma (#x)
	#define TODO(x) __pragma(message("TODO: " TOSTRING(x)) ) \
		__pragma(message(__FILE__ "(" TOSTRING(__LINE__) ") : warning C999999: “TODO”: " TOSTRING(x)) )
	#define FIXME(x) __pragma(message("FIX: " TOSTRING(x) "(" TOSTRING(__LINE__) ")") )
	//#define TODO(...) DO_PRAGMA(message ("TODO - " #__VA_ARGS__))
	//#define FIXME(...) DO_PRAGMA(message ("FIX - " #__VA_ARGS__))
    #define INIT_CONSOLE                    cactus::InitConsole()
    #define TRACE_CLEAR                     TTrace::ClearAll()
    #define TRACE_FlUSH                     TTrace::Flush()
    #define TRACE_SEND_FORMAT(format, ...)  TTrace::Debug()->SendFormat(format, __VA_ARGS__)
	#define TRACE_IF(cond, format, ...) \
				if (cond) TTrace::Debug()->SendFormat(format, __VA_ARGS__)	
    #define TRACE_SEND_W(x)                 TTrace::Debug()->SendW(x, TOSTRING_W(__FILE__) L":" TOSTRING_W(__LINE__))
    #define TRACE_SEND_A(x)                 TTrace::Debug()->SendA(x, __FILE__ ":" TOSTRING(__LINE__))

    #define TRACE_SEND2_W(x,y)              TTrace::Debug()->SendW(x, y)
    #define TRACE_SEND2_A(x,y)              TTrace::Debug()->SendA(x, y)

    #define TRACE_ENTER_METHOD              TTrace::Debug()->EnterMethod(__FUNCTION__, __FILE__ ":" TOSTRING(__LINE__))
    #define TRACE_EXIT_METHOD               TTrace::Debug()->ExitMethod(__FUNCTION__, __FILE__ ":" TOSTRING(__LINE__))

    #ifdef _UNICODE
        #define TRACE_SEND(x)           TRACE_SEND_W(x);
        #define TRACE_SEND2(x,y)        TRACE_SEND2_W(x,y);
    #else
        #define TRACE_Send(x)           TRACE_SEND_A(x);
        #define TRACE_Send2(x,y)        TRACE_SEND2_A(x,y);
    #endif


	#define TRACE_MSGBOX_A(x)					::MessageBoxA(NULL, x, "TRACE_TOOL_MSGBOX", MB_OK)
	#define TRACE_MSGBOX_W(x)					::MessageBoxW(NULL, x, L"TRACE_TOOL_MSGBOX", MB_OK)

	#define TRACE_REGKEY_W(x,y)			cactus::trace_regkey(x, y)

#else
	#define TODO(...) 
	#define FIXME(...) 
    #define INIT_CONSOLE
    #define TRACE_CLEAR
    #define TRACE_FlUSH
    #define TRACE_SEND_FORMAT(format, ...)
	#define TRACE_IF(cond, format, ...)
    #define TRACE_SEND(x)
    #define TRACE_SEND2(x,y)
	#define TRACE_SEND2_W(x,y)
	#define TRACE_SEND2_A(x,y)
	#define TRACE_SEND_W
	#define TRACE_SEND_A
    #define TRACE_ENTER_METHOD
    #define TRACE_EXIT_METHOD
	#define TRACE_MSGBOX_A(x)					
	#define TRACE_MSGBOX_W(x)					
#endif

#define CACTUS_API_CORE

#include <Windows.h>
#include <deque>
#include <string>

namespace cactus
{
CACTUS_API_CORE void InitConsole();
CACTUS_API_CORE void trace_regkey(const wchar_t* rootkey, const wchar_t* key);
} // namespace cactus

using namespace std ;

class TTrace ;
class TraceNodeBase ;
class TraceNode ;
class TraceNodeEx ;
class WinTrace ;
class TraceOptions ;
class TMemberNode ;

//-------------------------------------------------------------------------

const int WMD_TRACETOOL            = 123 ;  // identification code 'traceTool'

// Icones
const int CST_ICO_DEFAULT          = -1 ;     // default
const int CST_ICO_FORM             = 0 ;      // Form
const int CST_ICO_COMPONENT        = 1 ;      // component
const int CST_ICO_CONTROL          = 3 ;      // control
const int CST_ICO_PROP             = 5 ;      // property
const int CST_ICO_MENU             = 15 ;     // menu
const int CST_ICO_MENU_ITEM        = 16 ;     // item menu
const int CST_ICO_COLLECT_ITEM     = 21 ;     // collection
const int CST_ICO_WARNING          = 22 ;     // Warning
const int CST_ICO_ERROR            = 23 ;     // Error
const int CST_ICO_INFO             = 24 ;     // Information (default)

// viewer kind

/// <summary> viewer kind : default viewer, no icon</summary>
const int CST_VIEWER_NONE   = 0;
/// <summary> viewer kind : dump viewer </summary>
const int CST_VIEWER_DUMP   = 1;
/// <summary> viewer kind : xml viewer </summary>
const int CST_VIEWER_XML    = 2;
/// <summary> viewer kind : table viewer </summary>
const int CST_VIEWER_TABLE  = 3;
/// <summary> viewer kind : stack </summary>
const int CST_VIEWER_STACK  = 4;
/// <summary> viewer kind : bitmap viewer </summary>
const int CST_VIEWER_BITMAP = 5;
/// <summary> viewer kind : object structure </summary>
const int CST_VIEWER_OBJECT = 6;
/// <summary> viewer kind : object value </summary>
const int CST_VIEWER_VALUE  = 7;
/// <summary> viewer kind : enter method </summary>
const int CST_VIEWER_ENTER  = 8;
/// <summary> viewer kind : exit method </summary>
const int CST_VIEWER_EXIT   = 9;
/// <summary> viewer kind : text added to default viewer </summary>
const int CST_VIEWER_TXT    = 10;



// plugin
const int CST_PLUG_ONACTION        = 1 ;     // Action performed event
const int CST_PLUG_ONBEFOREDELETE  = 2 ;     // before delete event
const int CST_PLUG_ONTIMER         = 4 ;     // timer event

// resource kind
const int CST_RES_BUT_RIGHT        = 1 ;     // Button on right
const int CST_RES_BUT_LEFT         = 2 ;     // Button on left
const int CST_RES_LABEL_RIGHT      = 3 ;     // Label on right
const int CST_RES_LABELH_RIGHT     = 4 ;     // Label on right HyperLink
const int CST_RES_LABEL_LEFT       = 5 ;     // Label on left
const int CST_RES_LABELH_LEFT      = 6 ;     // Label on left hyperlink
const int CST_RES_MENU_ACTION      = 7 ;     // Item menu in the Actions Menu
const int CST_RES_MENU_WINDOW      = 8 ;     // Item menu in the Windows Menu. Call CreateResource on the main win trace to create this menu item

// resource id
const int CST_ACTION_CUT           = 1 ;     // cut same as copy then delete
const int CST_ACTION_COPY          = 2 ;     // copy
const int CST_ACTION_DELETE        = 3 ;     // delete selected
const int CST_ACTION_SELECT_ALL    = 4 ;     // select all
const int CST_ACTION_RESIZE_COLS   = 5 ;     // resize columns
const int CST_ACTION_VIEW_INFO     = 6 ;     // view trace info
const int CST_ACTION_VIEW_PROP     = 7 ;     // view properties
const int CST_ACTION_PAUSE         = 8 ;     // Pause on
const int CST_ACTION_SAVE          = 9 ;     // SaveToFile
const int CST_ACTION_CLEAR_ALL     = 10 ;    // clear all
const int CST_ACTION_CLOSE_WIN     = 11 ;    // Close win
const int CST_ACTION_RESUME        = 12 ;    // resume from Pause

const int CST_ACTION_LABEL_INFO    = 20 ;    // TracesInfo label
const int CST_ACTION_LABEL_LOGFILE = 21 ;    // LabelLogFile label
const int CST_ACTION_VIEW_MAIN     = 50 ;    // View Main trace
const int CST_ACTION_VIEW_ODS      = 51 ;    // ODS
const int CST_ACTION_OPEN_XML      = 52 ;    // XML trace -> Tracetool XML traces
const int CST_ACTION_EVENTLOG      = 53 ;    // Event log
const int CST_ACTION_TAIL          = 54 ;    // Tail

//================
// Command
//================

// INTERNAL
//--------------------------------------------------------------------------

const int CST_ENTER_DEBUG_MODE     = 107 ;   // VIEWER INTERNAL : enter debug mode
const int CST_LEAVE_DEBUG_MODE     = 108 ;   // VIEWER INTERNAL : leave debug mode
const int CST_OPEN_TAIL            = 109 ;   // VIEWER INTERNAL : Open tail file
const int CST_OPEN_XML             = 113 ;   // VIEWER INTERNAL : Open xml file on a new window (don't confuse with CST_LOADXML)
const int CST_GET_OBJECT           = 700 ;   // VIEWER INTERNAL : the user interface ask to retreive an object. param : node
const int CST_FLUSH                = 800 ;   // special case to be interpreted by the sender thread (not to be send)


// Wintrace / WinWatch. New commands should be added before 80
//--------------------------------------------------------------------------

const int CST_GOTO_FIRST_NODE      = 80 ;    // WinTrace.GotoFirstNode()
const int CST_GOTO_LAST_NODE       = 81 ;    // WinTrace.GotoLastNode()
const int CST_FIND_NEXT            = 82 ;    // WinTrace.FindNext(forward)
const int CST_GOTO_BOOKMARK        = 83 ;    // WinTrace.GotoBookmark(pos)
const int CST_CLEAR_BOOKMARK       = 84 ;    // WinTrace.ClearBookmark()
const int CST_CLEAR_FILTER         = 85 ;    // param : node
const int CST_ADD_FILTER           = 86 ;    // WinTrace.AddFilter(column,compare,text)
const int CST_APPLY_FILTER         = 87 ;    // WinTrace.ApplyFilter(ConditionAnd, ShowMatch,IncludeChildren). param : integer (3 bools)
const int CST_TREE_COLUMNWIDTH     = 93 ;    // Columns widths
const int CST_TREE_MULTI_COLUMN    = 95 ;    // change the columns titles
const int CST_TREE_COLUMNTITLE     = 96 ;    // change the tree to display multiple column
const int CST_DISPLAY_TREE         = 97 ;    // display tree windows
const int CST_TREE_NAME            = 98 ;    // param : the new name of the tree (use CST_USE_TREE just before to specify the tree)
const int CST_USE_TREE             = 99 ;    // param : Id (CLSID for example) of the tree to use for other command.
const int CST_CLEAR_ALL            = 104 ;   // no param
const int CST_CLOSE_WIN            = 105 ;   // no param. Close winwatch or wintrace
const int CST_WINWATCH_NAME        = 110 ;   // param : window name
const int CST_WINWATCH_ID          = 111 ;   // param : Window id
const int CST_WATCH_NAME           = 112 ;   // param : watch name
const int CST_SAVETOTEXT           = 559 ;   // save to text file, parameter : filename
const int CST_SAVETOXML            = 560 ;   // save to  XML file, parameter : filename
const int CST_LOADXML              = 561 ;   // load an XML file to the current wintrace
const int CST_LOGFILE              = 562 ;   // set the log file for a wintrace

// Wintrace plugins
//--------------------------------------------------------------------------

const int CST_LINKTOPLUGIN         = 563 ;   // link a wintrace to a plugin
const int CST_CREATE_RESOURCE      = 564 ;   // create a resource on a wintrace
const int CST_SET_TEXT_RESOURCE    = 565 ;   // set the text resource
const int CST_DISABLE_RESOURCE     = 566 ;   // disable a resource

// TTrace
//--------------------------------------------------------------------------

const int CST_FIND_TEXT            = 100 ;   // TTrace.Find (text, bool Sensitive, bool WholeWord , bool highlight ). param : int (Sensitive+WholeWord+highlight) , string
const int CST_SHOW                 = 102 ;   // param : 1 : show.  0 : hide
const int CST_CLOSE_VIEWER         = 106 ;   // no param : quit tracetool

// Node
//--------------------------------------------------------------------------

const int CST_TRACE_ID             = 101 ;   // param : CLSID
const int CST_ICO_INDEX            = 103 ;   // param : image index
const int CST_GOTO_NEXTSIBLING     = 114 ;   // ITraceNode.GotoNextSibling ()  param : node
const int CST_GOTO_PREVSIBLING     = 115 ;   // ITraceNode.GotoPrevSibling ()  param : node
const int CST_GOTO_FIRST_CHILD     = 116 ;   // ITraceNode.GotoFirstChild  ()  param : node
const int CST_GOTO_LAST_CHILD      = 117 ;   // ITraceNode.GotoLastChild   ()  param : node
const int CST_SET_BOOKMARK         = 122 ;   // ITraceNode.SetBookmark (bool enabled)  param : int
const int CST_VISIBLE_NODE         = 123 ;   // ITraceNode.SetVisible  (visible)       param : int
const int CST_CLEAR_NODE           = 300 ;   // param : the node to clear + flag
const int CST_CLEAR_SUBNODES       = 301 ;   // param : the parent node
const int CST_THREAD_ID            = 302 ;   // param : thread ID
const int CST_PROCESS_NAME         = 303 ;   // param process name
const int CST_MESSAGE_TIME         = 304 ;   // param : the time of the message
const int CST_THREAD_NAME          = 305 ;   // param : thread name (java)
const int CST_IP                   = 306 ;   // Client IP adress
const int CST_NEW_NODE             = 550 ;   // param : parent node ID
const int CST_LEFT_MSG             = 551 ;   // param : left msg
const int CST_RIGHT_MSG            = 552 ;   // param : right msg
const int CST_SELECT_NODE          = 553 ;   // set the node as 'Selected' by the user.  param : Node id
const int CST_USE_NODE             = 555 ;   // use an existing node. param : Node id
const int CST_APPEND_LEFT_MSG      = 556 ;   // param : left msg to append
const int CST_APPEND_RIGHT_MSG     = 557 ;   // param : right msg to append
const int CST_FOCUS_NODE           = 558 ;   // Focus to the node.
const int CST_FONT_DETAIL          = 567 ;   // param : ColId Bold Italic Color size  Fontname
const int CST_BACKGROUND_COLOR     = 568 ;   // param : background color

// Members
//--------------------------------------------------------------------------

const int CST_CREATE_MEMBER        = 500 ;   // param : Member name
const int CST_MEMBER_FONT_DETAIL   = 501 ;   // param : ColId Bold Italic Color size  Fontname
const int CST_MEMBER_COL2          = 502 ;   // param : info col 2
const int CST_MEMBER_COL3          = 504 ;   // param : info col 3
const int CST_MEMBER_VIEWER_KIND   = 503 ;   // param : viewer id
const int CST_ADD_MEMBER           = 505 ;   // add member to upper level. No param (for now)


//====================================================================================

/// <summary>
/// The list of command to send
/// </summary>

template <class _Ty,    class _Ax = allocator<_Ty> >
class CommandDeque : public deque<_Ty, _Ax>
{
public :
    // formatting Helper functions.

    /// back : code only
    void Add(const int msg)
    {
        char message [10] ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
        sprintf_s(message, 10, "%5d", msg);
#else
        sprintf(message, "%5d", msg);
#endif
        push_back(message) ;
    } ;

    //-------------------------------------------------------------------------

    /// front : code only
    void AddFront(const int msg)
    {
        char message[10];
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
        sprintf_s(message, 10, "%5d", msg);
#else
        sprintf(message, "%5d", msg);
#endif
        push_front(message) ;
    }

    //-------------------------------------------------------------------------

    /// back : code + string
    void Add(const int msg, const char *StrValue)
    {
        char * message ;

        if (StrValue == NULL) {
            message = (char*)malloc(5 + 1) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, 6, "%5d", msg);
#else
            sprintf(message, "%5d", msg);
#endif
        } else {
            size_t MsgLen = 5 + strlen(StrValue) + 1 ;
            message = (char*)malloc(MsgLen) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, MsgLen, "%5d%s", msg, StrValue);
#else
            sprintf(message, "%5d%s", msg, StrValue);
#endif
        }

        push_back(message) ;
        free(message) ;
    }

    //-------------------------------------------------------------------------

    /// back : code + string + string
    void Add(const int msg, const char *StrValue1, const char *StrValue2)
    {
        char * message ;

        if (StrValue1 == NULL && StrValue2 == NULL) {
            message = (char*)malloc(5 + 1) ; // +1 for eol
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, 6, "%5d", msg);
#else
            sprintf(message, "%5d", msg);
#endif
        } else if (StrValue1 == NULL) {
            size_t MsgLen = 5 + strlen(StrValue2) + 2 ; // +1 for pipe, +1 for eol
            message = (char*)malloc(MsgLen) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, MsgLen, "%5d|%s", msg, StrValue2);
#else
            sprintf(message, "%5d|%s", msg, StrValue2);
#endif
        } else if (StrValue2 == NULL) {
            size_t MsgLen = 5 + strlen(StrValue1) + 1 ; // +1 for eol
            message = (char*)malloc(MsgLen) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, MsgLen, "%5d%s", msg, StrValue1);
#else
            sprintf(message, "%5d%s", msg, StrValue1);
#endif
        } else { // both are ok
            size_t MsgLen = 5 + strlen(StrValue1) + strlen(StrValue2) + 2 ;
            message = (char*)malloc(MsgLen) ; // +1 for pipe, +1 for eol
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, MsgLen, "%5d%s|%s", msg, StrValue1, StrValue2);
#else
            sprintf(message, "%5d%s|%s", msg, StrValue1, StrValue2);
#endif
        }

        push_back(message) ;
        free(message) ;
    }

    //-------------------------------------------------------------------------

    /// front : code + string
    void AddFront(const int msg, const char * StrValue)
    {
        char * message ;

        if (StrValue == NULL) {
            message = (char*)malloc(5 + 1) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, 6, "%5d", msg);
#else
            sprintf(message, "%5d", msg);
#endif
        } else {
            size_t MsgLen = 5 + strlen(StrValue) + 1 ;
            message = (char*)malloc(MsgLen) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, MsgLen, "%5d%s", msg, StrValue);
#else
            sprintf(message, "%5d%s", msg, StrValue);
#endif
        }

        push_front(message) ;
        free(message) ;
    }

    //-------------------------------------------------------------------------

    /// back : code + int
    void Add(const int msg, const int intValue)
    {
        char message[20];
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)   // visual studio 2005 : deprecated function
        sprintf_s(message, 20, "%5d%11d", msg, intValue);      // 5 + 11 + 1
#else
        sprintf(message, "%5d%11d", msg, intValue);      // 5 + 11 + 1
#endif
        push_back(message) ;
    }

    //-------------------------------------------------------------------------

    /// front : code + int
    void AddFront(const int msg, const int intValue)
    {
        char message[20];
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
        sprintf_s(message, 20, "%5d%11d", msg, intValue);         // 5 + 11 + 1
#else
        sprintf(message, "%5d%11d", msg, intValue);         // 5 + 11 + 1
#endif
        push_front(message) ;
    }
    //-------------------------------------------------------------------------

    /// back : code + int + string
    void Add(const int msg, const int intValue, const char * StrValue)
    {
        char * message ;

        if (StrValue == NULL) {
            message = (char*)malloc(17) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, 17, "%5d%11d", msg, intValue);
#else
            sprintf(message, "%5d%11d", msg, intValue);
#endif
        } else {
            size_t messLen = 17 + strlen(StrValue) ;
            message = (char*)malloc(messLen) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, messLen, "%5d%11d%s", msg, intValue, StrValue); // 5 + 11 + xxx + 1
#else
            sprintf(message, "%5d%11d%s", msg, intValue, StrValue);  // 5 + 11 + xxx + 1
#endif
        }

        push_back(message) ;
        free(message) ;
    }

    //-------------------------------------------------------------------------

    /// front : code + int + string
    void AddFront(const int msg, const int intValue, const char * StrValue)
    {
        char * message ;

        if (StrValue == NULL) {
            message = (char*)malloc(17) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, 17, "%5d%11d", msg, intValue);             // 5 + 11 + 1
#else
            sprintf(message, "%5d%11d", msg, intValue);              // 5 + 11 + 1
#endif
        } else {
            int mess_len = 17 + strlen(StrValue) ;
            message = (char*)malloc(mess_len) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)   // visual studio 2005 : deprecated function
            sprintf_s(message, mess_len, "%5d%11d%s" , msg, intValue, StrValue); // 5 + 11 + xxx + 1
#else
            sprintf(message, "%5d%11d%s", msg, intValue, StrValue);  // 5 + 11 + xxx + 1
#endif
        }

        push_front(message) ;
        free(message) ;
    }

    //-------------------------------------------------------------------------
    /// back : code + int + int + string
    void Add(const int msg, const int int1, const int int2 , const char * StrValue)
    {
        char * message ;

        if (StrValue == NULL) {
            message = (char*)malloc(28) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, 28, "%5d%11d%11d", msg, int1, int2);               // 5 + 11 + 11 + 1
#else
            sprintf(message, "%5d%11d%11d", msg, int1, int2);                // 5 + 11 + 11 + 1
#endif
        } else {
            size_t mess_len = 28 + strlen(StrValue) ;
            message = (char*)malloc(mess_len) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, mess_len, "%5d%11d%11d%s", msg, int1, int2 , StrValue);  // 5 + 11 + 11 + xxx + 1
#else
            sprintf(message, "%5d%11d%11d%s", msg, int1, int2 , StrValue);  // 5 + 11 + 11 + xxx + 1
#endif
        }

        push_back(message) ;
        free(message) ;
    }
    //-------------------------------------------------------------------------
    /// back : code + int + int + int + string
    void Add(const int msg, const int int1, const int int2 , const int int3 , const char * StrValue)
    {
        char * message ;

        if (StrValue == NULL) {
            message = (char*)malloc(39) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, 39, "%5d%11d%11d%11d", msg, int1, int2 , int3);              // 5 + 11 + 11 + 11 + 1
#else
            sprintf(message, "%5d%11d%11d%11d", msg, int1, int2 , int3);               // 5 + 11 + 11 + 11 + 1
#endif
        } else {
            size_t mess_len = 39 + strlen(StrValue) ;
            message = (char*)malloc(mess_len) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, mess_len, "%5d%11d%11d%11d%s", msg, int1, int2 , int3 , StrValue);  // 5 + 11 + 11 + 11 + xxx + 1
#else
            sprintf(message, "%5d%11d%11d%11d%s", msg, int1, int2 , int3 , StrValue);  // 5 + 11 + 11 + 11 + xxx + 1
#endif
        }

        push_back(message) ;
        free(message) ;
    }
    //-------------------------------------------------------------------------
    /// back : code + int + int + int + int + string
    void Add(const int msg, const int int1, const int int2 , const int int3 , const int int4 , const char * StrValue)
    {
        char * message ;

        if (value == NULL) {
            message = (char*)malloc(50) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, 49, "%5d%11d%11d%11d%11d", msg, int1, int2 , int3, int4);                // 5 + 11 + 11 + 11 + 11 + 1
#else
            sprintf(message, "%5d%11d%11d%11d%11d", msg, int1, int2 , int3, int4);                // 5 + 11 + 11 + 11 + 11 + 1
#endif
        } else {
            int messLen = 50 + strlen(StrValue) ;
            message = (char*)malloc(messLen) ;
#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (! UNDER_CE)  // visual studio 2005 : deprecated function
            sprintf_s(message, messLen, "%5d%11d%11d%11d%11d%s", msg, int1, int2 , int3 , int4, StrValue);  // 5 + 11 + 11 + 11 + 11 + xxx + 1
#else
            sprintf(message, "%5d%11d%11d%11d%11d%s", msg, int1, int2 , int3 , int4, StrValue);   // 5 + 11 + 11 + 11 + 11 + xxx + 1
#endif
        }

        push_back(message) ;
        free(message) ;
    }

} ;

typedef CommandDeque<string> CommandList ;

//====================================================================================
/// <summary>
/// NodeContext is used internaly for the Indent and UnIndent functions.
/// </summary>

class NodeContext
{
public :
    DWORD threadId ;
    string nodeId ;
} ;

//====================================================================================

/// <summary>
/// Specify a font detail for traces columns items and members.
/// </summary>

class FontDetail
{
public :
    int ColId  ;
    bool Bold  ;
    bool Italic ;
    int Color  ;    // -$7FFFFFFF-1..$7FFFFFFF;
    int Size   ;
    string FontName ;
} ;

//====================================================================================

/// <summary>
/// TMemberNode represent the informations displayed on the "Trace info panel" for a specific trace
/// </summary>

class TMemberNode
{
private :
    void _AddToStringList(CommandList * commands) ;
    deque <TMemberNode *> * m_Members ;          // sub members
    deque <FontDetail *> * m_FontDetails ;       // fonts
public :
    string col1 ;                                /// Column 1
    string col2 ;                                /// Column 2
    string col3 ;                                /// Column 3
    int    tag ;                                 /// User defined tag, NOT SEND to the viewer
    int    ViewerKind ;                          ///  Viewer kind. determine how the node will display members

    //-------------------------------------------------------------------------
    TMemberNode(const char    * strCol1 = NULL, const char    * strCol2 = NULL, const char    * strCol3 = NULL) ;
    TMemberNode(const wchar_t * strCol1       , const wchar_t * strCol2 = NULL, const wchar_t * strCol3 = NULL) ;

    ~TMemberNode() ;
    deque <TMemberNode *> * Members() ;
    TMemberNode * Add(TMemberNode * member) ;
    TMemberNode * Add(const char    * strCol1, const char    * strCol2 = NULL, const char    * strCol3 = NULL);
    TMemberNode * Add(const wchar_t * strCol1, const wchar_t * strCol2 = NULL, const wchar_t * strCol3 = NULL);
    TMemberNode * SetFontDetail(const int ColId, const bool Bold , const bool Italic = false , const int Color = -1 , const int Size = 0 , const char * FontName = NULL) ;

    void AddToStringList(CommandList * commands) ;                                              // convert to command
} ;


//====================================================================================

/// <summary>
/// WinWatch represent a windows tree where you put watches
/// </summary>

class WinWatch
{
public :
    string id ;                                                       // Winwatch id (empty for the main window)
    bool   enabled ;                                                  // enable or disable watches
    int    tag ;                                                      // User defined tag, NOT SEND to the viewer

    WinWatch(void) ;
    WinWatch(const char    * WinWatchID , const char    * WinWatchText) ;
    WinWatch(const wchar_t * WinWatchID , const wchar_t * WinWatchText) ;
    void DisplayWin(void) ;
    void ClearAll(void) ;
    void Close() ;                                                    // close the WinWatch
    void Send(const char    * WatchName , const char    * WatchValue) ;
    void Send(const wchar_t * WatchName , const wchar_t * WatchValue) ;
} ;

//====================================================================================

/// <summary>
/// Base class (abstract) for TraceToSend and TraceNodeEx
/// </summary>

class CACTUS_API_CORE TraceNodeBase
{
public :
    bool       enabled ;                      // enable or disable traces
    int        iconIndex ;                    // icon index
    string     id ;                           // Node id
    int        tag ;                          // User defined tag, NOT SEND to the viewer
    string     winTraceId ;                   // Owner
    TraceNodeEx * CreateChildEx(const char    *leftMsg = NULL, const char    *rightMsg = NULL) ;    // prepare minimal sub node without sending it
    TraceNodeEx * CreateChildEx(const wchar_t *leftMsg = NULL, const wchar_t *rightMsg = NULL) ;    // prepare minimal sub node without sending it
    virtual ~TraceNodeBase()
    {
        ;    // destructor
    }
} ;

//====================================================================================

/// <summary>
/// Base class (abstract) for TraceNode and Wintrace.
/// </summary>

class CACTUS_API_CORE TraceToSend : public TraceNodeBase
{
    friend TraceNodeEx ;
    friend WinTrace ;
    friend TraceNode ;
public:
    virtual TraceToSend::~TraceToSend()
    {
        DeleteCriticalSection(&criticalSection);
    }

    // The most usefull function in that library


    void SendFormat(const char* format, ...);
    void SendFormat(const wchar_t* format, ...);
    void Send(const char     *leftMsg,  const char    *rightMsg = NULL) ;
    void Send(const wchar_t  *wLeftMsg, const wchar_t *wRightMsg = NULL) ;

    void SendA(const char     *leftMsg,  const char    *rightMsg = NULL) ;
    void SendW(const wchar_t  *wLeftMsg, const wchar_t *wRightMsg = NULL) ;

    // send a node based on this node then change the indentation
    void Indent(const char    *leftMsg,  const char    *rightMsg  = NULL) ;
    void Indent(const wchar_t *leftMsg,  const wchar_t *rightMsg  = NULL) ;
    void Indent(const char    *leftMsg,  const char    *rightMsg , int BackGroundColor, bool IsEnter) ;
    void Indent(const wchar_t *wLeftMsg, const wchar_t *wRightMsg, int BackGroundColor, bool IsEnter) ;
    void EnterMethod(const char    *leftMsg , const char    *rightMsg  = NULL, int BackGroundColor = 0) ;
    void EnterMethod(const wchar_t *leftMsg , const wchar_t *rightMsg  = NULL, int BackGroundColor = 0) ;

    void UnIndent(const char    *leftMsg , const char    *rightMsg  , int BackGroundColor, bool isExit) ;
    void UnIndent(const wchar_t *leftMsg , const wchar_t *rightMsg  , int BackGroundColor, bool isExit) ;
    void UnIndent(const char    *leftMsg , const char    *rightMsg = NULL) ;
    void UnIndent(const wchar_t *leftMsg , const wchar_t *rightMsg = NULL) ;
    void UnIndent(void) ;
    void ExitMethod(const char    *leftMsg = NULL, const char    *rightMsg  = NULL, int BackGroundColor = 0);
    void ExitMethod(const wchar_t *leftMsg = NULL, const wchar_t *rightMsg  = NULL, int BackGroundColor = 0);

    void SendDump(const char    *leftMsg,  const char    *rightMsg,  const char    * title, const char * memory, const unsigned byteCount) ;
    void SendDump(const wchar_t *wLeftMsg, const wchar_t *wRightMsg, const wchar_t * title, const char * memory, const unsigned byteCount) ;
    void SendXml(const char    *leftMsg, const char * xml) ;
    void SendXml(const wchar_t *leftMsg, const wchar_t * xml) ;

protected :
    CommandList * prepareNewNode(const char * leftMsg, const char * newId) ;
    CommandList * prepareNewNode(const wchar_t *leftMsg, const char * newId) ;

    CRITICAL_SECTION criticalSection ;                                          // protect Indent and UnIndent functions
    deque <NodeContext *> * contextList ;                                       // context list
    deque <NodeContext *> * winTraceContext ;                                   // reference to winTrace Context (fContextList). Can be null
    NodeContext * GetLastContext() ;                                            // get the last context for the current thread
    string GetLastContextId() ;                                                 // get the last context id for the current thread
    void PushContext(NodeContext * context) ;                                   // push context
    void PushContextId(const char * contextId) ;                                // push a new context
    void deleteLastContext(void) ;                                              // delete the last context for the current thread
};

//====================================================================================
/// <summary>
/// TraceNode represent a node on the viewer.
/// </summary>
class CACTUS_API_CORE TraceNode : public TraceToSend
{
    friend WinTrace ;
    friend TraceToSend ;
protected :
    /// <summary>
    /// Internal constructor. Use TTrace or TraceNodeEx class as entry point
    /// create a Node with an unique ID (true)
    /// </summary>
    /// <param name="parentNode">The parent node where to place that trace.
    /// The IconIndex and the enabled properties are also recopied
    /// Can be null : the root tree become the parent node, enabled is true and the default icon is used
    /// </param>
    /// <param name="generateUniqueId">When true, a unique ID (a guid) is generated for the trace.
    /// </param>
    TraceNode(const TraceNode * parentNode = NULL , const bool generateUniqueId = true);
public :
    /// <summary>
    /// create a TraceNode copy of a TraceToSend
    /// </summary>
    /// <param name="Source">TraceNode to copy</param>
    TraceNode(const TraceToSend * source);
    /// <summary>
    /// create a TraceNode copy of a TraceNodeEx
    /// </summary>
    /// <param name="Source">TraceNodeEx to copy</param>
    TraceNode(const TraceNodeEx * source);

    ~TraceNode(void) ;   // destructor

    TraceNode * Resend(const char    *LeftMsg, const char    *RightMsg = NULL) ;
    TraceNode * Resend(const wchar_t *LeftMsg, const wchar_t *RightMsg = NULL) ;
    TraceNode * Append(const char    *LeftMsg, const char    *RightMsg = NULL) ;
    TraceNode * Append(const wchar_t *LeftMsg, const wchar_t *RightMsg = NULL) ;
    TraceNode * ResendIconIndex(const int index) ;
    TraceNode * SetFontDetail(const int colId, const bool bold, const bool italic = false, const int color = -1, const int size = 0, const char    * fontName = NULL) ;
    TraceNode * SetFontDetail(const int colId, const bool bold, const bool italic = false, const int color = -1, const int size = 0, const wchar_t * fontName = NULL) ;
    TraceNode * Show() ;                                                         // ensure the trace is visible in the viewer
    TraceNode * SetSelected() ;                                                  // set the node as selected
    TraceNode * Delete() ;                                                       // delete the node and all children
    TraceNode * DeleteChildren() ;                                               // delete the trace children, not the node itself
    TraceNode * SetBackgroundColor(const int color, const int colId = -1) ;

    TraceNode * SetBookmark(const bool Bookmarked) ;
    TraceNode * SetVisible(const bool Visible) ;
    TraceNode * GotoNextSibling() ;
    TraceNode * GotoPrevSibling() ;
    TraceNode * GotoFirstChild() ;
    TraceNode * GotoLastChild() ;
} ;

//====================================================================================
/// <summary>
/// TraceNodeEx allow you to construct trace node, adding 'members' before sending it.<p/>
/// Don't forget to free node.
/// </summary>

class CACTUS_API_CORE TraceNodeEx : public TraceNodeBase //Base
{
    friend TTrace ;                         // TTrace add command and process members before sending it.
    friend TraceNode ;                      // TraceNode add command
public:
    /// <summary>
    /// The Id of the parent node
    /// </summary>
    string parentNodeId ;

    /// <summary>
    /// left message
    /// </summary>
    string leftMsg;
    /// <summary>
    /// right message
    /// </summary>
    string rightMsg;

    /// <summary>
    /// time
    /// </summary>
    string time ;
    /// <summary>
    /// thread name
    /// </summary>
    string threadName ;

    TMemberNode * Members() ;   // members info

    // constructors

    TraceNodeEx(TraceNodeBase * parentNode ,        const bool generateUniqueId = true) ;      // constructor
    TraceNodeEx(TraceToSend   * parentNode = NULL , const bool generateUniqueId = true) ;      // constructor
    TraceNodeEx(TraceNodeEx   * parentNode,         const bool generateUniqueId = true) ;      // constructor
    virtual ~TraceNodeEx(void) ;                                                              // and destructor

    // AddXXX methods

    TraceNodeEx * AddDump(const char    * Title , const char * memory  , const unsigned index , const unsigned byteCount) ;   // add dump to the members info
    TraceNodeEx * AddDump(const wchar_t * Title , const char * memory  , const unsigned index , const unsigned byteCount) ;   // add dump to the members info
    TraceNodeEx * AddXml(const char * xml) ;
    TraceNodeEx * AddFontDetail(const int ColId,  const bool Bold ,  const bool Italic = false,  const int Color = -1 , const int Size = 0 ,  const char * FontName = NULL) ;
    TraceNodeEx * AddBackgroundColor(const int Color, const int ColId = -1) ;

    TraceNode * Send(void) ;

    /// When the TraceNodeEx is send, the traceNode property is assigned.
    /// traceNode represent a node on the viewer
    TraceNode * traceNode ;

protected:

    TMemberNode * m_Members ;               // node members
    deque <FontDetail *> * m_FontDetails ;       // fonts
} ;

//====================================================================================
/// <summary>
/// WinTrace represent a windows tree where you put traces
/// </summary>

class CACTUS_API_CORE WinTrace : public TraceToSend
{
    friend TTrace ;   // TTrace use direct access to debug, warning and error
private :
    TraceToSend * debug;
    TraceToSend * warning;
    TraceToSend * error;
    void CreateNodes() ;
public :
    WinTrace(void) ;
    WinTrace(const char    * WinTraceID , const char    * WinTraceTitle)  ;
    WinTrace(const wchar_t * WinTraceID , const wchar_t * WinTraceTitle)  ;
    ~WinTrace(void) ;

    /// Warning, Error and Debug are the 3 doors to send traces
    /// <returns>The Debug node</returns>
    TraceToSend    * Debug()
    {
        return debug   ;
    } ;

    /// Warning, Error and Debug are the 3 doors to send traces
    /// <returns>The warning node</returns>
    TraceToSend    * Warning()
    {
        return warning ;
    } ;

    /// Warning, Error and Debug are the 3 doors to send traces
    /// <returns>The Error node</returns>
    TraceToSend    * Error()
    {
        return error   ;
    } ;

    void SaveToTextfile(const char    * FileName) ;
    void SaveToTextfile(const wchar_t * FileName) ;
    void SaveToXml(const char    * FileName) ;
    void SaveToXml(const wchar_t * FileName) ;
    void SaveToXml(const char    * FileName, const char    * styleSheet) ;
    void SaveToXml(const wchar_t * FileName, const wchar_t * styleSheet) ;

    void LoadXml(const char    * FileName) ;
    void LoadXml(const wchar_t * FileName) ;
    void SetLogFile(const char    * FileName, int Mode, int maxLines) ;
    void SetLogFile(const wchar_t * FileName, int Mode, int maxLines) ;
    string GetLocalLogFile() ;
    void DisplayWin() ;
    void SetMultiColumn(const int MainColIndex = 0) ;
    void SetColumnsTitle(const char    * Titles) ;
    void SetColumnsTitle(const wchar_t * Titles) ;
    void SetColumnsWidth(const char    * Widths) ;
    void SetColumnsWidth(const wchar_t * Widths) ;
    void ClearAll() ;
    void Close() ;

    void GotoFirstNode() ;
    void GotoLastNode() ;
    void FindNext(const bool SearForward) ;
    void GotoBookmark(int Pos);
    void ClearBookmark();
    void ClearFilter() ;
    void AddFilter(int Column , int Compare , const char    * Text) ;
    void AddFilter(int Column , int Compare , const wchar_t * Text) ;
    void ApplyFilter(const bool ConditionAnd, const bool ShowMatch, const bool IncludeChildren) ;


    // Plugin API. not available on pocket PC
#ifndef _WIN32_WCE
    void CreateResource(const int ResId    , const int ResType ,
                        const int ResWidth , const char * ResText = NULL) ;
    void CreateResource(const int ResId    , const int ResType ,
                        const int ResWidth , const wchar_t * ResText = NULL) ;
    void DisableResource(const int ResId) ;
    void SetTextResource(const int ResId, const char    * ResText) ;
    void SetTextResource(const int ResId, const wchar_t * ResText) ;
    void LinkToPlugin(const char   * PluginName,  const int flags) ;
    void LinkToPlugin(const wchar_t * PluginName, const int flags) ;
#endif
} ;

//====================================================================================

/// <summary>
/// Represent a WinTrace, even if the WinTrace no more exist. This is a private class
/// </summary>
class InternalWinTrace
{
public :
    string ID ;
    bool   IsMultiColTree ;
    int    MainCol ;
    string TitleList ;
    string LogFileName ;
    string LastLocalLogFileName ;
    int    LogFileType ;
    int    MaxLines;// Max number of lines before starting a new file
    int    CurrentFileNumber;// Current file number, when MaxLines is set
    int    LinesWritten;// Number of lines written , when MaxLines is set
    InternalWinTrace(void) ;
} ;

//====================================================================================

/// <summary>
/// TTrace is the entry point for all traces.<p/>
/// TTrace give 3 'TraceNode' doors : Warning , Error and Debug.<p/>
/// Theses 3 doors are displayed with a special icon (all of them have the 'enabled' property set to true.<p/>
/// That class is fully static.
/// </summary>

class CACTUS_API_CORE TTrace
{
    friend TraceOptions ;
    friend WinTrace ;
    friend WinWatch ;
    friend TraceNodeEx ;
    friend TraceToSend ;
    friend TraceNode ;
private:
    static DWORD              m_ClockSequenceBase ;
    static WORD               m_DeviceId1 ;
    static WORD               m_DeviceId2 ;
    static WORD               m_DeviceId3 ;
    static WinTrace *         m_winTrace  ;
    static WinWatch *         m_winWatch ;
    static TraceOptions *     m_options ;
    static bool               m_IsSocketInitialized  ;
    static struct sockaddr_in m_serverSockAddr;    // Socket adress
    static UINT_PTR           m_socketHandle ;
    static CRITICAL_SECTION   criticalSection ;
    static InternalWinTrace * DefaultWinTrace ;
    static deque <InternalWinTrace *> * FormTraceList ;
    static deque <CommandList *> * setMessageList ;
    static deque <CommandList *> * getMessageList ;

    static HANDLE CloseEvent ;
    static HANDLE MessageReady ;
    static HANDLE ClosedEvent ;
    static HANDLE HandlesToWaitFor[2];
    static HANDLE hThread ;
    static bool isStopping ;
    static unsigned __stdcall MsgThread(void* pArguments) ;
    static void LoopMessages(void) ;

    TTrace(void) {} ;   // private constructor. No instance allowed
    static void SendToWinWatchClient(CommandList * Commands, const char * winWatchId);   // send the trace to the viewer
    static void SendToWinTraceClient(CommandList * Commands, const char * winTraceId);   // send the trace to the viewer
    static void SendToWinTraceClient(CommandList * Commands, const char * winTraceId, const char * dateTime, const char * threadName);   // send the trace to the viewer
    static void SendToClient(CommandList * Commands) ;                             // send the trace to the viewer
    static int ParseForInternal(CommandList * Commands) ;
    static InternalWinTrace * getInternalTraceForm(const char * TraceWinID , bool doCreate)  ;
public:
    static void CloseSocket() ;
    static char * CreateTraceID();
    static char * WideToMbs(const wchar_t * WideStr) ;
    static wchar_t * MbsToWide(const char * MbsStr) ;
    static string HtmlEncode(string source) ;

    /// Shortcut to WinTrace.Debug
    /// <returns>WinTrace.Debug</returns>
    static TraceToSend    * Debug()
    {
        return m_winTrace->debug ;
    };

    /// Shortcut to WinTrace.Warning
    /// <returns>WinTrace.Warning</returns>
    static TraceToSend    * Warning()
    {
        return m_winTrace->warning ;
    } ;

    /// Shortcut to WinTrace.Error
    /// <returns>WinTrace.Error</returns>
    static TraceToSend    * Error()
    {
        return m_winTrace->error ;
    } ;

    /// flush traces to the viewer. Default TimeOut is 5000 millisec
    static void Flush();
    /// flush traces to the viewer.
    /// <param name="FlushTimeOut">Number of millisec to wait</param>
    static void Flush(int FlushTimeOut);

    /// TTrace Options (socket, show functions, ...)
    /// <returns>TTrace Options</returns>
    static TraceOptions * Options()
    {
        return m_options ;
    } ;

    /// The windows where is stored the main tree (read only)
    /// <returns>the main wintrace</returns>
    static WinTrace     * WindowTrace()
    {
        return m_winTrace ;
    } ;

    /// The main watches window
    /// <returns>The main watches window</returns>
    static WinWatch     * Watches()
    {
        return m_winWatch ;
    } ;


    static void ClearAll()  ;
    static void CloseViewer() ;
    static void Show(bool IsVisible) ;
    static void Find(const wchar_t *Text , const bool Sensitive, const bool WholeWord , const bool Highlight, const bool SearchInAllPages) ;
    static void Find(const char    *Text , const bool Sensitive, const bool WholeWord , const bool Highlight, const bool SearchInAllPages) ;


    // TODO : Thread Sub System
    // TODO : static void Flush (FlushTimeOut : integer = 5000);
    // TODO : static void Start() ; // restart tracetool sub system if STOP was called

    static void Stop() ;  // Stop tracetool sub system. Must be called before exiting plugin

    static DWORD Init() ;

}  ;

//====================================================================================

/// <summary>
/// Determine how to send the trace : windows message or socket (for windows Ce for example)
/// </summary>

enum SendMode {
    WinMsg = 1,                             // Windows message
    Socket = 2,                             // Socket message
    None = 3                                // no message are send to viewer
} ;

//====================================================================================

/// <summary>
/// TTrace options
/// </summary>

class CACTUS_API_CORE TraceOptions
{
    friend TTrace ;                         // TTrace use the process file name
    friend TraceNodeEx ;                    // TraceNodeEx get TTrace options
protected :
    char * m_processFileName ;              // Process name
    char * socketHost;                      // socket host (name or ip)
    TraceOptions(void) ;                    // constructor
    ~TraceOptions(void) ;                   // and destructor
public :

    SendMode sendMode ;                     // WinMsg or Socket
    int      socketPort ;                   // socket port

    bool     SendProcessName ;                    // if true the process name is send
    bool     SendDate ;                           // if true the date is send
    bool     SendThreadId ;                       // if true the tread id is send
    const char * CheckProcessName() ;             // helper function : return the process name
    void     SetSocketHost(const char * Host) ;   // set the socket Host
    void     SetSocketHost(const wchar_t * Host);
} ;


//====================================================================================    
void DebugTrace(const TCHAR* format, ...);
