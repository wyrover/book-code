
#include <windows.h>
#include <process.h>    /* _beginthread, _endthread */
#include <stddef.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

void Bounce( void *ch );
void CheckKey( void *dummy );

/* GetRandom returns a random integer between min and max. */
#define GetRandom( min, max ) ((rand() % (int)(((max) + 1) - (min))) + (min))

BOOL repeat = TRUE;     /* Global repeat flag and video variable */
HANDLE hStdOut;         /* Handle for console window */
CONSOLE_SCREEN_BUFFER_INFO csbi;    /* Console information structure */

void main()
{
    CHAR    ch = 'A';

    hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );

    /* Get display screen's text row and column information. */
   GetConsoleScreenBufferInfo( hStdOut, &csbi );

    /* Launch CheckKey thread to check for terminating keystroke.此线程用来控制线程和程序的终止 */
    _beginthread( &CheckKey, 0, NULL );

    /* Loop until CheckKey terminates program. */
    while( repeat )
    {
        /* On first loops, launch character threads. */
        _beginthread( Bounce, 0, (void *) (ch++)  );
		//不断启动新的线程

        /* Wait one second between loops. */
        Sleep( 1000L );
    }
}

/* CheckKey - Thread to wait for a keystroke, then clear repeat flag. */
void CheckKey( void *dummy )
{
    _getch();
    repeat = 0;    /* _endthread implied */

}

/* Bounce - Thread to create and and control a colored letter that moves
 * around on the screen.
 *
 * Params: ch - the letter to be moved
 */
void Bounce( void *ch )
{
    /* Generate letter and color attribute from thread argument. */
    char    blankcell = 0x20;
    char    blockcell = (char) ch;
    BOOL    first = TRUE;
   COORD   oldcoord, newcoord;
   DWORD   result;


    
   /* Seed the random-number generator with current time so that
	  the numbers will be different every time we run.
    */
	srand( (unsigned)time( NULL ) );

  
	/*  get initial location. */
    newcoord.X = GetRandom( 0, csbi.dwSize.X - 1 );
    newcoord.Y = GetRandom( 0, csbi.dwSize.Y - 1 );
    while( repeat )
    {
        /* Pause between loops. */
		Sleep( 100L );
        /* Blank out our old position on the screen, and draw new letter. */
        if( first )
            first = FALSE;
        else
         WriteConsoleOutputCharacter( hStdOut, &blankcell, 1, oldcoord, &result );
         WriteConsoleOutputCharacter( hStdOut, &blockcell, 1, newcoord, &result );

        /* Increment the coordinate for next placement of the block. */
        oldcoord.X = newcoord.X;
        oldcoord.Y = newcoord.Y;
        newcoord.X += GetRandom( -1, 1 );
        newcoord.Y += GetRandom( -1, 1 );

        /* Correct placement (and beep) if about to go off the screen. */
        if( newcoord.X < 0 )
            newcoord.X = 1;
        else if( newcoord.X == csbi.dwSize.X )
            newcoord.X = csbi.dwSize.X - 2;
        else if( newcoord.Y < 0 )
            newcoord.Y = 1;
        else if( newcoord.Y == csbi.dwSize.Y )
            newcoord.Y = csbi.dwSize.Y - 2;

        /* If not at a screen border, continue, otherwise beep. */
        else
            continue;
		//产生声音
        Beep( ((char) ch - 'A') * 100, 175 );

		
    }
    /* _endthread given to terminate */
    _endthread();
}
