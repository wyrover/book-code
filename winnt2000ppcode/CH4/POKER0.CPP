
/////////////////////////////////////////////////////////////////////////////
//
// Poker.cpp : Class behavior for the application and the main window.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Poker.h"
#include "Filemap.h"
#include "Cards.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// #define	PLAY_QUICK			// Define to play the game quickly

/////////////////////////////////////////////////////////////////////////////
// The one and only CDialogApp object
/////////////////////////////////////////////////////////////////////////////
CDialogApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CDialogApp initialization
/////////////////////////////////////////////////////////////////////////////
BOOL CDialogApp::InitInstance()
{

	srand(::GetTickCount());			// Seed random number generator
	SetDialogBkColor();        			// Set dialog and messagebox colors to gray

	m_pMainWnd = new CPokerWindow();	// Create the main application window
	ASSERT(m_pMainWnd);					// Check for failure here

	if (m_lpCmdLine[0] != '\0')
	{
		// Add command line processing here
	}

	return TRUE;
}

BOOL CDialogApp::OnIdle(LONG lCount)
{
BOOL bMorePoker;

	BOOL bMoreApp = CWinApp::OnIdle(lCount);
    
    if(m_pMainWnd)
    	bMorePoker = ((CPokerWindow*)m_pMainWnd)->PlayPoker();
	else
		bMorePoker = FALSE;

Sleep(0);	// Give up remainder of CPU time.
return (bMoreApp || bMorePoker);
}


BEGIN_MESSAGE_MAP(CPokerWindow, CDialog)
	//{{AFX_MSG_MAP(CPokerWindow)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_DIALOGS_ABOUT, OnSystemAbout)
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPokerWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);			// Call the base class first

	//{{AFX_DATA_MAP(CPokerWindow)
	//}}AFX_DATA_MAP
}

//-----------------------------------------------------------------------
// CPokerWindow constructor:
// Create the dialog box using the definition in the dialog template
//-----------------------------------------------------------------------
CPokerWindow::CPokerWindow()
{
	//{{AFX_DATA_INIT(CPokerWindow)
	//}}AFX_DATA_INIT

	Create(IDD);		// Create the Main Dialog Window (Dialog ID = IDD)

	// Load the accelerator table which we will be using
	m_hAccel=::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_POKER_ACCEL));
	ASSERT(m_hAccel);

	m_FileMap.MapMemory("PokerGame", sizeof(POKER));	// Create the poker game

	m_wPlayerState = JOINING;		// Current state of the player
	m_Cash = 100;					// Initial cash is $100
	SetWindowText("Waiting to join...");
}

//-----------------------------------------------------------------------
// Translate accelerator messages here.
//-----------------------------------------------------------------------
BOOL CPokerWindow::PreTranslateMessage(MSG *pMsg)
{
	if(!TranslateAccelerator(this->m_hWnd, m_hAccel, pMsg))
		return CDialog::PreTranslateMessage(pMsg);
	else
		return TRUE;
}

//-----------------------------------------------------------------------
// OnInitdialog() is called after the dialog has been created but before
// it is displayed. Perform initialization of the dialog here.
//-----------------------------------------------------------------------
BOOL CPokerWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

//-----------------------------------------------------------------------
// CWnd::PostNcDestroy() is the last member function which will be called.
// We will use this to shut down our application.
//-----------------------------------------------------------------------
void CPokerWindow::PostNcDestroy()
{
	PostQuitMessage(0);				// End the application
	delete this;					// Delete 'this' dialog box object
}

//
//------------------ Procedure to create and display the About Box -------------------
//
void CPokerWindow::OnAbout()
{
	CModalDialog aboutdlg(IDD_ABOUTBOX, this);
	aboutdlg.DoModal();
}


//------------------------------------------------------------------
// As a special treat we will create a standard 'system' about box.
// These are the common about dialogs that appear in the freebie
// windows applets (Notepad, PaintBrush, Clock, etc) that ship with
// Windows. There may be a better way to do this but this does work.
//------------------------------------------------------------------
void CPokerWindow::OnSystemAbout()
{
BOOL	bSuccess;									// Success or Failure flag
int (WINAPI *lpFunc)(HWND, LPSTR, LPSTR, HICON);	// Function prototype

	bSuccess = FALSE;								// Assume we don't make it
	HANDLE hLib=LoadLibrary("SHELL32");				// Load the DLL
	if((int)hLib > 32)								// See if it loaded properly
	{
		// Get the address of the ShellAbout() function
		lpFunc=(int (WINAPI *)(HWND,LPSTR,LPSTR,HICON))::GetProcAddress((HINSTANCE)hLib,"ShellAboutA");

		if(lpFunc)									// We have a pointer to the function
		{
			HICON hIcon=theApp.LoadIcon(IDR_POKER_ICON);	// Load the icon & call function
			(*lpFunc)(this->m_hWnd,"Poker Application#Poker","Written by Mark Szamrej", hIcon);
			::DestroyIcon(hIcon);					// Unload the icon
			bSuccess = TRUE;						// We made it!!
		}
	}

	if(!bSuccess)
		AfxMessageBox("ShellAbout() Failed!");
}


BOOL CPokerWindow::OnEraseBkgnd(CDC* pDC)
{
	if(IsIconic())									// Are we an icon
	{
		HICON hIcon=theApp.LoadIcon(IDR_POKER_ICON);	// Load the icon.
		ASSERT(hIcon);								// Make sure it loaded!

		SendMessage(WM_ICONERASEBKGND, (WPARAM)pDC->m_hDC, 0L);	// Paint the background
		pDC->DrawIcon(0,0, hIcon);					// Draw the icon.
		::DestroyIcon(hIcon);						// Destroy (release) the icon
		return TRUE;								// All done... return success.
	}

	return CDialog::OnEraseBkgnd(pDC);				// Not an icon. Let windows handle it.
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// This is where the actual game is played. This function is
// called during idle time to play the game. If a player is
// out of the game (state == FINISHED) the function will
// return FALSE indicating that no additional CPU time is
// needed. Otherwise the function will always return TRUE
// indicating that we want to be called again (more CPU time).
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CPokerWindow::PlayPoker()
{
static WORD	wLastState = 0xFFFF;			// Initialize to (-1)


	if(m_wPlayerState == FINISHED)		// If we are FINISHED then we
		return FALSE;					// don't need more CPU time.

POKER* pPoker = (POKER*) m_FileMap.Open();	// Open the game object
ASSERT(pPoker != NULL);

	// Switch depending on the state of the game
	switch(pPoker->wState)
	{
		// Executed when we are waiting for players to join the game
		case PST_WAIT:
			// If we are waiting to leave then leave the game now
			if(m_wPlayerState & LEAVING)
				LeaveTheGame();

			// If we are not a player yet then join the game
			if(m_wPlayerState==JOINING && pPoker->wPlayers < MAX_PLAYERS)
			{
				TRACE("Calling JoinTheGame()\n");
				JoinTheGame();
				wLastState = PST_WAIT;	// Save our last state as WAIT
			}

			// We were waiting for enough players to play. We have enough
			// players so we can begin the game now.
			if(pPoker->wPlayers >= MIN_PLAYERS)
			{
				TRACE("Done waiting... starting game\n");
				pPoker->wRefCount = pPoker->wPlayers;	// Ref count is all players
				pPoker->wState = PST_ANTE;				// Start the ante
				wLastState = PST_WAIT;					// Save our last state as WAIT
			}
			break;

		// Executed when it is time for an ante
		case PST_ANTE:

			// If we are not in the game then join now.
			// Joins are only allowed during 'WAIT' periods
			// and during ante's.
			if(m_wPlayerState==JOINING && pPoker->wPlayers < MAX_PLAYERS)
				JoinTheGame();

			// If we are playing and we have less than two people
			// then we need to wait for more players.
			if(m_wPlayerState & PLAYING && pPoker->wPlayers < MIN_PLAYERS)
			{
				pPoker->wState = PST_WAIT;				// Set state to waiting
				SetWindowText("Waiting for players");	// Set the window title
				wLastState = PST_WAIT;					// Save the last state as WAIT
				break;
			}

			if(wLastState != PST_ANTE && m_wPlayerState & PLAYING)
			{
				if(wLastState == PST_WAIT)				// If we were waiting then
					SetWindowText("Playing poker");		// Update the window title

				TRACE("Ante up!\n");
				m_Hand.ClearCards();		// Turn in the cards
				
#ifndef PLAY_QUICK					
				// Hide the controls which display our cards.
				for(UINT nItem = IDC_CARD1; nItem<=IDC_CARD7; nItem++)
				{
					CWnd* pDlgItem = GetDlgItem(nItem);
					ASSERT(pDlgItem);
					pDlgItem->ShowWindow(SW_HIDE);
				}
#endif

				wLastState = PST_ANTE;		// We already put in our ante

				m_Cash--;					// Ante a dollar
				pPoker->dwJackpot++;		// Place it into the jackpot

				if(! --pPoker->wRefCount)	// Are we the last??
				{
					TRACE("Ante done... Deal em\n");

					// We are about to deal the cards. Before we start
					// dealing we need to create the deck and shuffle it.
					DWORD* pDeck = pPoker->dwDeck;	// Beginning of the deck
					for(int suite=0; suite<4; suite++)
						for(int rank=2; rank<15; rank++)
						{
							DWORD dwTemp=MAKELONG(suite, rank);
							ASSERT(dwTemp != 0);
							*pDeck++ = dwTemp;
						}

					// Cards are created. Shuffle them. We will randomly
					// pick two cards from the deck and swap them. We will
					// do this multiple times in order to shuffle the deck.

					pDeck = pPoker->dwDeck;	// Beginning of the deck

					int iShuffles = 10000;	// Start with 10,000 shuffles
					iShuffles += rand()%2000;	// Add a random factor

					for(int iMany=0; iMany<10000; iMany++)	// 10000 iterations
					{
						int iFirst = rand() % 52;		// Limit to 52
						ASSERT(iFirst>=0 && iFirst<52);

						int iSecond = rand() % 52;		// Limit to 52
						ASSERT(iSecond>=0 && iSecond<52);
					
						// Swap the first and second card
						DWORD dwTemp = *(pDeck+iFirst);
						ASSERT(dwTemp != 0);
						*(pDeck+iFirst) = *(pDeck+iSecond);
						*(pDeck+iSecond) = dwTemp;

						if( !(iMany%500) && dwTemp == MAKELONG(3, 14))	// Ace of spades???
							srand(::GetTickCount());
					}

					pPoker->wRefCount = pPoker->wPlayers;	// Ref count is all players
					pPoker->wState = PST_DEAL;				// We can start dealing.
				}
			}
			break;

		//--------------------------------------------------------------
		// The cards are dealt one by one. This is probably the slowest
		// portion of the game. It can be improved in three ways:
		//
		//	#1 - Have each player take all 7 cards at once. The current
		// 		approach more closely approximates real life since only
		//		one card at a time is given to a player.
		//
		//	#2 - The manner in which a card is selected is to start
		//		at the beginning of the deck and to search forward
		//		until a valid card is found. This approach gets
		//		progressively slower as more cards are removed from
		//		the deck. A variable could have been stored within
		//		the POKER structure which specify the exact location
		//		of the next card. This is a minor CPU cycle eater in
		//		relation to item #3.
		//
		//	#3 - Defer updating the cards until all cards have been
		//		received. This would release the mapped POKER object
		//		sooner allowing other apps to run. Currently the
		//		controls are updated as each card is received. This is
		//		relatively slow in terms of CPU time. This situation
		//		is amplified by the fact that ALL other poker games
		//		will be suspended until the update is done. When
		//		those tasks get a chance to run they too will HOG
		//		the POKER object while they update their screens.
		//		Such is life!
		//--------------------------------------------------------------
		case PST_DEAL:
			if(wLastState != PST_DEAL && m_wPlayerState & PLAYING)
			{
				if(m_Hand.NeedMoreCards())
				{
					DWORD* pDeck = pPoker->dwDeck;	// Search from beginning
					while(*pDeck == 0)				// Until we find a card
						pDeck++;

					ASSERT(*pDeck != 0);			// Better be a card

					m_Hand.AddCard(CCard(*pDeck));	// Add this card
					*pDeck = 0;						// Set the card to nothing

#ifndef PLAY_QUICK
					CCard card = m_Hand.GetLastCard();
					CString string = card.CardToText();

					// Display the card we received. Here is where we
					// waste lots (relatively) of CPU time since all
					// of the other games must wait for us to paint
					// before they can continue executing.
					UINT nItem = IDC_CARD1+m_Hand.GetCardCount()-1;
					CWnd* pDlgItem = GetDlgItem(nItem);
					ASSERT(pDlgItem);
					pDlgItem->SetWindowText(string);
					pDlgItem->ShowWindow(SW_SHOW);
#endif
					break;
				}

				TRACE("Getting my card\n");
				wLastState = PST_DEAL;		// We were dealt to

				if(! --pPoker->wRefCount)	// Are we the last??
				{
					pPoker->dwHighHand = 0;	// Clear the high hand
					TRACE("Done dealing... Open\n");
					pPoker->wRefCount = pPoker->wPlayers;	// Ref count is all players
					pPoker->wState = PST_OPEN;				// Players must try to open
				}
			}
			break;

		case PST_OPEN:
			if(wLastState != PST_OPEN && m_wPlayerState & PLAYING)
			{
				TRACE("Trying to open\n");
				wLastState = PST_OPEN;					// We tried to open
				DWORD dwMyHand = m_Hand.GetHighHand();	// Get our high hand

				// We can open if our high hand is better than a pair. If we
				// can open and we have better cards than the current high
				// hand then set the current hand to our hand.
				if(HIWORD(dwMyHand) >= 2 &&  dwMyHand> pPoker->dwHighHand)
					pPoker->dwHighHand = dwMyHand;		// Set it as our hand

				if(! --pPoker->wRefCount)		// We are the last one
				{
				TRACE("Done with Open\n");
				pPoker->wRefCount = pPoker->wPlayers;	// Ref count is all players

					if(pPoker->dwHighHand == 0)
					{
						TRACE("Can't Open\n");
						pPoker->wState = PST_ANTE;		// Ante up again
					}
					else
					{
						TRACE("Can Open\n");
						pPoker->wState = PST_BET;		// Go ahead and bet
					}
				}
			}
			break;

		case PST_BET:
			if(wLastState != PST_BET && m_wPlayerState & PLAYING)
			{
				TRACE("Time to bet\n");
				wLastState = PST_BET;		// We placed our bet

				// The bet amount is dependant on what was opened with. That is...
				// the minimum openers was TWO_PAIR (2) therefor the minimum bet
				// will be at least $2. The highest hand is a Straight Flush which
				// has a value of 9. This makes the highest bet $9.00.
				DWORD dwBet = (DWORD) HIWORD(pPoker->dwHighHand);
				m_Cash-=dwBet;					// Subtract the bet from our money
				pPoker->dwJackpot+=dwBet;		// Place it into the jackpot

				if(! --pPoker->wRefCount)	// Are we the last??
				{
					TRACE("Last one to bet\n");
					pPoker->wRefCount = pPoker->wPlayers;	// Ref count is all players
					pPoker->wState = PST_SHOW;				// Show the cards
				}
			}
			break;

		case PST_SHOW:
			if(wLastState != PST_SHOW && m_wPlayerState & PLAYING)
			{
				TRACE("Showing my cards\n");
				wLastState = PST_SHOW;		// We showed our cards
				
				// Update the hands we've had.
				DWORD dwMyHighHand = m_Hand.GetHighHand();
				IncrementControl(HIWORD(dwMyHighHand)+IDC_HAND0);
				UpdateMoney();						// Update the money section

				// If the high hand is lower than ours then
				// set ours as being the high hand
				if(pPoker->dwHighHand < dwMyHighHand)
					pPoker->dwHighHand = dwMyHighHand;	// Set to our high hand

				if(! --pPoker->wRefCount)	// Are we the last??
				{
					TRACE("Cards are all shown\n");
					pPoker->wRefCount = pPoker->wPlayers;	// Ref count is all players
					pPoker->wState = PST_CASH;				// Cash in on the winnings
				}
			}
			break;

		case PST_CASH:
			if(wLastState != PST_CASH && m_wPlayerState & PLAYING)
			{
				TRACE("Cashing in\n");
				wLastState = PST_CASH;		// We cashed in

				// If we have the high hand then cash in on it!!
				if(pPoker->dwHighHand == m_Hand.GetHighHand())
				{
					m_Cash += pPoker->dwJackpot;		// Add in our jackpot
					pPoker->dwJackpot = 0;				// Zero the jackpot
					IncrementControl(IDC_NUMWIN);
				}

				if(m_wPlayerState & LEAVING || m_Cash <= 0)
					LeaveTheGame();

				if(! --pPoker->wRefCount)					// Are we the last??
				{
					TRACE("Last one to cash in\n");
					pPoker->dwHighHand = 0;					// Reset to no hand
					pPoker->wRefCount = pPoker->wPlayers;	// Ref count is all players
					pPoker->wState = PST_ANTE;				// Ante up for the next hand
				}
			}
			break;

		default:
			TRACE("Unknown last state\n");	// Should never happen
			ASSERT(FALSE);
			break;
	}
	
m_FileMap.Close();						// Close the game object
return TRUE;							// We want more CPU time
}

void CPokerWindow::OnClose()
{
	if(m_wPlayerState & PLAYING || m_wPlayerState & LEAVING)
	{
		TRACE("Setting flag to leave the game\n");
		SetWindowText("Leaving game...");	// Set the window title
		m_wPlayerState |= LEAVING;			// Set the state to leaving
	}
	else
		DestroyWindow();					// then just end the app.
}

void CPokerWindow::JoinTheGame()
{
TRACE("JoinTheGame()\n");

POKER* pPoker = (POKER*) m_FileMap.Open();	// Open the game object

	pPoker->wPlayers++;					// Inc. # of players
	pPoker->wRefCount++;				// Increment required ref count
	m_wPlayerState=PLAYING;				// We are now a player

	// Update the window title
	if(pPoker->wPlayers < MIN_PLAYERS)
		SetWindowText("Waiting for players");
	else
		SetWindowText("Playing poker");

m_FileMap.Close();						// Close the game object
}

void CPokerWindow::LeaveTheGame()
{
TRACE("LeavingTheGame()\n");

POKER* pPoker = (POKER*) m_FileMap.Open();	// Open the game object
	
	pPoker->wPlayers--;					// Dec. # of players
	m_wPlayerState = FINISHED;			// We are finished playing

	SetWindowText("Out of the game");	// Set the window title
	
m_FileMap.Close();	// Close the game object
}


void CPokerWindow::UpdateMoney()
{
char buff[128];
POKER* pPoker = (POKER*) m_FileMap.Open();

	wsprintf(buff, "$%d", pPoker->dwJackpot);
	GetDlgItem(IDC_JACKPOT)->SetWindowText(buff);

	wsprintf(buff, "$%d", m_Cash);
	GetDlgItem(IDC_TOTALWIN)->SetWindowText(buff);

m_FileMap.Close();	// Close the game object
}

void CPokerWindow::IncrementControl(UINT nID)
{
CString string;
char buff[32];

	CWnd* pDlgItem = GetDlgItem(nID);
	ASSERT(pDlgItem);
	pDlgItem->GetWindowText(string);

	int iCount = atoi(string);
	wsprintf(buff, "%d", ++iCount);
	pDlgItem->SetWindowText(buff);
}


