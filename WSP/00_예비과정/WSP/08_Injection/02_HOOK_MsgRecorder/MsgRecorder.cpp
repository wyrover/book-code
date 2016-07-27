// Message Recorder.

#define _WIN32_WINNT 0x0500
#define WINVER		 0x0500
#include <windows.h>
#include "resource.h"
#include <vector>
using namespace std;

vector<EVENTMSG> msg_buf;
HHOOK g_hRecordHook;
HHOOK g_hPlayHook;
HWND  g_hDlg;

LRESULT CALLBACK RecordHookProc(int nCode, WPARAM wParam, LPARAM lParam) 
 
{ 
    EVENTMSG        Event; 
    LPEVENTMSG      lpEvent; 

    if ( nCode >= 0 ) 
	{ 
        lpEvent = (LPEVENTMSG) lParam; 
         
        Event.message = lpEvent->message; 
        Event.paramL  = lpEvent->paramL; 
        Event.paramH  = lpEvent->paramH; 
        Event.time    = lpEvent->time; 
         
		msg_buf.push_back( Event );	
		
        return 0; 
    }  
    return (CallNextHookEx(g_hRecordHook, nCode, wParam, lParam)); 
} 

int    PlayCnt;
int    bufCnt;
DWORD  dwTimeAdjust;

LRESULT CALLBACK PlayHookProc(int nCode,WPARAM wParam,LPARAM lParam) 
{ 
    LPEVENTMSG  lpEvent; 
    long        lReturnValue; 
 
	if ( nCode >= 0 ) { 
        if (nCode == HC_SKIP ) 
		{
			OutputDebugString("HC_SKIP\n");
            if(PlayCnt < bufCnt - 1){ 
                PlayCnt++; 
 
            }else{ 
 
                SendMessage( g_hDlg, WM_COMMAND, IDC_STOP_PLAY, 0);
 
                return 0; 
            } 
        } 
        else if ( nCode == HC_GETNEXT) { 
			OutputDebugString("HC_GETNEXT\n");
            if(PlayCnt >= bufCnt - 1)goto end; 
             
            lpEvent = (LPEVENTMSG) lParam; 
             
            lpEvent->message = msg_buf[PlayCnt].message;
            lpEvent->paramL  = msg_buf[PlayCnt].paramL; 
            lpEvent->paramH  = msg_buf[PlayCnt].paramH; 
            lpEvent->time    = msg_buf[PlayCnt].time + dwTimeAdjust;  
             
            lReturnValue = lpEvent->time - GetTickCount();
		//	lReturnValue = msg_buf[PlayCnt].time  - msg_buf[0].time ;
            // 
            // No Long ( negative ) waits 
            //                               
            if ( lReturnValue < 0L ) { 
                lReturnValue  = 0L; 
                lpEvent->time = GetTickCount();         //현재시간으로 조정하여 시스템 메세지 큐에  
                //집어 넣는다. 
            } 
            return ( (DWORD) lReturnValue ); 
        } 
    } 
end: 
    return( CallNextHookEx(g_hPlayHook, nCode, wParam, lParam)); 
     
} 
//--------------------
// Buttons
//--------------------
HWND btnRecord, 
	 btnRecordStop, 
	 btnPlay, 
	 btnPlayStop;

void EnableButtons( BOOL b1, BOOL b2, BOOL b3, BOOL b4 )
{
	EnableWindow( btnRecord,	b1);
	EnableWindow( btnRecordStop,b2);  
	EnableWindow( btnPlay,		b3);
	EnableWindow( btnPlayStop,	b4);
}

INT_PTR WINAPI DlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	char temp[128];
	switch ( msg )
	{
	case WM_INITDIALOG:
		SetWindowPos( hDlg, HWND_TOPMOST, 0, 0, 0,0,SWP_NOMOVE|SWP_NOSIZE);
		g_hDlg			= hDlg;
		btnRecord		= GetDlgItem( hDlg, IDC_RECORD);
		btnRecordStop	= GetDlgItem( hDlg, IDC_STOP_RECORD);
		btnPlay			= GetDlgItem( hDlg, IDC_PLAY);
		btnPlayStop		= GetDlgItem( hDlg, IDC_STOP_PLAY);
		break;
	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		case IDC_RECORD:    
			while ( ! msg_buf.empty() ) msg_buf.pop_back();
			g_hRecordHook = SetWindowsHookEx( WH_JOURNALRECORD, RecordHookProc, GetModuleHandle(0), 0);
			EnableButtons( 0, 1, 0, 0);
			SetDlgItemText(hDlg, IDC_MESSAGE, "Recording..." );
			break;
		case IDC_STOP_RECORD:
			UnhookWindowsHookEx( g_hRecordHook );
			EnableButtons( 1, 0, 1, 0);
			wsprintf( temp, "%d Messages Recorded.", msg_buf.size() );
			SetDlgItemText(hDlg, IDC_MESSAGE, temp );
			break;

		case IDC_PLAY:                        
			bufCnt = msg_buf.size();
			PlayCnt = 0;    
			dwTimeAdjust = GetTickCount() - msg_buf[0].time; 

			g_hPlayHook = SetWindowsHookEx( WH_JOURNALPLAYBACK, PlayHookProc, GetModuleHandle(0), 0);
			EnableButtons( 0, 0, 0, 1);
			SetDlgItemText(hDlg, IDC_MESSAGE, "Playing..." );
			break;

		case IDC_STOP_PLAY:
			UnhookWindowsHookEx( g_hPlayHook );
			EnableButtons( 1, 0, 1, 0);
			wsprintf( temp, "%d Messages Played.", msg_buf.size() );
			SetDlgItemText(hDlg, IDC_MESSAGE, temp );
			break;
					
		case IDCANCEL:EndDialog( hDlg, IDCANCEL); break;
		}
		break;
		
	default:
		return FALSE;
	}
	return TRUE;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE , LPSTR lpCmdLine, int nShowCmd )
{
	UINT ret = DialogBox( hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)DlgProc);

	return 0;
}