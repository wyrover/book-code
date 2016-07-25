
//======================================================================
// Poker.h : main header file for the Poker application
//======================================================================

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // Main application symbols
#include "Filemap.h"
#include "Cards.h"

// POKER structure. This is mapped into shared memory for
// all players to access via mapped memory.
typedef struct tagPOKER
{
	WORD		wPlayers;		// Number of players
	WORD		wState;			// Current state of game
	DWORD		dwJackpot;		// Amount in the jackpot
	WORD		wRefCount;		// Reference count
	DWORD		dwHighHand;		// Used to determine high hand
	DWORD		dwDelay;		// Delay (to control speed)
	DWORD		dwDeck[52];		// Array of 52 cards (the deck)
} POKER;


// The different Poker states the game can be in.

#define	PST_WAIT		0		// Waiting for players
#define	PST_ANTE		1		// Waiting for ante
#define	PST_DEAL		2		// Dealing cards
#define	PST_OPEN		3		// Trying to open
#define	PST_BET			4		// Able to open... betting
#define	PST_SHOW		5		// Showing cards
#define	PST_CASH		6		// Winner cashes in.

// The different states players can be in.
#define JOINING			0		// Waiting to joining a game
#define PLAYING			1		// Playing the game
#define LEAVING			2		// Leaving the game
#define FINISHED		4		// Finished the game

#define MIN_PLAYERS		2		// Minimum number of players
#define MAX_PLAYERS		6		// Maximum number of players



//----------------------------------------------------------------------
// CDialogApp : Class definition for the entire application.
//----------------------------------------------------------------------
class CDialogApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG);		// Idle time processing
};


//----------------------------------------------------------------------
// CPokerWindow : The main window for the Poker application.
//----------------------------------------------------------------------
class CPokerWindow : public CDialog
{
DECLARE_MESSAGE_MAP()

public:
	CPokerWindow();					// Constructor.
	//{{AFX_DATA(CPokerWindow)
	enum { IDD = MAIN_DLG_WINDOW };
	//}}AFX_DATA

	BOOL	PlayPoker();			// Called during idle time

protected:
	//{{AFX_MSG(CPokerWindow)
	afx_msg void OnAbout();				// Creates and displays an About box.
	afx_msg void OnSystemAbout();		// System About box
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);// Used to paint the icon
	afx_msg void OnClose();
	//}}AFX_MSG

	BOOL OnInitDialog();				// Dialog initialization
	BOOL PreTranslateMessage(MSG*);		// Handle accelerator messages
	void OnOK(){}						// Called when 'Enter' is pressed
	void OnCancel(){OnClose();}			// If 'Cancel' is pressed try to close.
	void PostNcDestroy();				// Called after window is destroyed
	void DoDataExchange(CDataExchange*);// DDX/DDV support

	void JoinTheGame();					// Join the game
	void LeaveTheGame();				// Exit the game
	void UpdateMoney();					// Update the money section
	void IncrementControl(UINT);		// Increment a dialog control

protected:
	CFileMap	m_FileMap;				// File mapping object
	WORD		m_wPlayerState;			// Current state of the player
	LONG		m_Cash;					// The cash that we have
	CHand		m_Hand;					// The hand of cards we have

private:
	HACCEL		m_hAccel;				// Accelerator handle
};


