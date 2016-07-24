/**************************************************************************************
 *                                                                                    *
 * This application contains code from OpenDivX and is released as a "Larger Work"    *
 * under that license. Consistant with that license, this application is released     *
 * under the GNU General Public License.                                              *
 *                                                                                    *
 * The OpenDivX license can be found at: http://www.projectmayo.com/opendivx/docs.php *
 * The GPL can be found at: http://www.gnu.org/copyleft/gpl.html                      *
 *                                                                                    *
 * Copyright (c) 2001 - Project Mayo                                                  *
 *                                                                                    *
 * Authors: Damien Chavarria <adrc at projectmayo.com>                                *
 *                                                                                    *
 **************************************************************************************/


#include "Skin.h"

/*
 * Class Functions
 *
 */

/*
 * Constructor : Loads the default skin
 *
 */


Skin::Skin(HINSTANCE hInstance, HWND hwnd)
{

	this->LoadDefault(hInstance, hwnd);
	
	if(this->skin == NULL )
	{//||this->controls == NULL) {
		MessageBox(NULL, "Cannot load default skin!", "Error", MB_OK);
	}

	/*
 	 * Default values for the rects
	 */

	this->menuButton.left   = 2;
	this->menuButton.right  = 14;
	this->menuButton.top    = 3;
	this->menuButton.bottom = 14;

	this->minimizeButton.left   = 266;
	this->minimizeButton.right  = 278;
	this->minimizeButton.top    = 3;
	this->minimizeButton.bottom = 14;

	this->closeButton.left   = 283;
	this->closeButton.right  = 295;
	this->closeButton.top    = 3;
	this->closeButton.bottom = 14;

	this->compactMode = 0;
	this->dMutex      = CreateMutex(NULL, FALSE, NULL);

	this->font = CreateFont(12, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
							CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
}

/*
 * Destructor
 *
 */

Skin::~Skin()
{
	DeleteObject((HGDIOBJ) this->skin); 

	DeleteObject((HGDIOBJ) this->bottom); 
	DeleteObject((HGDIOBJ) this->logo); 
}

/*
 * Returns TRUE if the point is
 * inside the given rect...
 *
 */

BOOL Skin::isInsideRect(int x, int y, RECT rect)
{
	return (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom);
}

/*
 * loads the Default Skin
 *
 */

int Skin::LoadDefault(HINSTANCE hInstance, HWND hwnd) {

	HDC   dc, sdc, ddc;
	DWORD i;

	this->skin     = LoadBitmap(hInstance, MAKEINTRESOURCE(ID_SKIN)); 
	this->logo     = LoadBitmap(hInstance, MAKEINTRESOURCE(ID_LOGO));

	dc = GetDC(hwnd);

	this->bottom = CreateCompatibleBitmap(dc, 256, 115);
	this->tmp    = CreateCompatibleBitmap(dc, 150, 28);

	sdc = CreateCompatibleDC(dc);
	SelectObject(sdc, this->skin);

	ddc = CreateCompatibleDC(dc);
	SelectObject(ddc, this->bottom);


	for(i=0; i < 256; i++) {

		BitBlt(ddc, i, 0, 1, 115, sdc, 200, 260, SRCCOPY);
	}

	ReleaseDC(hwnd, dc);
	DeleteDC(sdc);
	DeleteDC(ddc);

	this->skinColor = 0;

	return 1;
}




/*
 * Return Default Width
 *
 */

int Skin::GetDefaultWidth()
{
	return DEFAULT_SKIN_WIDTH;	
}

/*
 * Return Default Width
 *
 */


int Skin::GetDefaultHeight()
{
	return DEFAULT_SKIN_HEIGHT;	
}








/*
 * Calculates the video size
 * and position on the hwnd
 */

RECT *Skin::GetVideoRect()
{
	if(this->hwnd) {

		GetClientRect(this->hwnd, &this->videoRect);

		if(this->compactMode) {

			return &this->videoRect;
		}
		else {

			this->videoRect.left    = 7;
			this->videoRect.right  -= 15;
			this->videoRect.top     = 22;
			this->videoRect.bottom -= 115;

			return &this->videoRect;
		}
	}

	return &this->videoRect;
}

/*
 * Set/Unset the compact mode
 */

int Skin::SetCompact(int compact)
{
	this->compactMode = compact;

	return 0;
}


/*
 * Display : display the skin 
 *           into a window
 *
 */

int Skin::Display(HWND hwnd)
{
	HDC          hdc, bdc;
	PAINTSTRUCT  ps;
	int          sizeX, sizeY, i;
	RECT         clientRect;
	HBRUSH       brush, oldBrush;

	/*
 	 * Get some info about the
	 * window size and all.
	 */

	GetClientRect(hwnd, &clientRect);

	sizeX = clientRect.right - clientRect.left;
	sizeY = clientRect.bottom - clientRect.top;

	this->hwnd = hwnd;

	if(true) 
	{//not fullscreen

		hdc  = BeginPaint(hwnd, &ps);

		if(this->compactMode) 
		{
			bdc	 = CreateCompatibleDC(hdc);
			SelectObject(bdc, this->logo);
			if(false)//playback->IsOverlay())
			{
	//			brush = CreateSolidBrush(DD_OVERLAY_COLORREF); 
	//			oldBrush = (HBRUSH) SelectObject(hdc, brush);
	//			Rectangle(hdc, 0, 0, sizeX, sizeY); 
		
	//			SelectObject(hdc, oldBrush);
	//			DeleteObject((HGDIOBJ) brush);
			}else
			{
				
				if(true)//!playback->IsPlaying()) 
				{
					brush = CreateSolidBrush(this->skinColor); 
					oldBrush = (HBRUSH) SelectObject(hdc, brush);
					Rectangle(hdc, 0, 0, DEFAULT_SKIN_WIDTH + (sizeX - DEFAULT_SKIN_WIDTH), 
	    			  DEFAULT_SKIN_HEIGHT + (sizeY - DEFAULT_SKIN_HEIGHT)); 
					SelectObject(bdc, this->logo);
					if(sizeY > LOGO_HEIGHT)
						BitBlt(hdc, (sizeX - LOGO_WIDTH)/2, (sizeY - LOGO_HEIGHT)/2, LOGO_WIDTH, LOGO_HEIGHT, bdc, 0, 0, SRCCOPY);

					SelectObject(hdc, oldBrush);
					DeleteObject((HGDIOBJ) brush);
				}
			}

			DeleteDC(bdc);
		}else 
		{///not compactMode

			/*
			 * Paint the normal skin
			 */

			bdc  = CreateCompatibleDC(hdc);
			SelectObject(bdc, this->skin);

			if(hdc == NULL || bdc == NULL) 
			{

				return 0;
			}

			/*
			 * First blit the background
 			 */

			/*
			 * Title
			 */

			BitBlt(hdc, 0, 0, 21, 22, bdc, 0, 0, SRCCOPY);

			for(i = 0; i < (sizeX - DEFAULT_SKIN_WIDTH) / 2; i++) 
			{	

				BitBlt(hdc, 21 + i, 0, 1, 22, bdc, 21, 0, SRCCOPY);
			}

			BitBlt(hdc, 21 + (sizeX - DEFAULT_SKIN_WIDTH) / 2, 0, 240, 22, bdc, 21, 0, SRCCOPY);

			for(i = 0; i < (sizeX - DEFAULT_SKIN_WIDTH) / 2 + ((sizeX - DEFAULT_SKIN_WIDTH) % 2); i++) 
			{

				BitBlt(hdc, 261 + (sizeX - DEFAULT_SKIN_WIDTH) / 2 + i, 0, 1, 22, bdc, 261, 0, SRCCOPY);
			}

			BitBlt(hdc, 261 + (sizeX - DEFAULT_SKIN_WIDTH), 0, 39, 22, bdc, 261, 0, SRCCOPY);

			/*
			 * The play Area
			 *
			 */

			for(i=0; i < 238 + (sizeY - DEFAULT_SKIN_HEIGHT); i++) 
			{

				BitBlt(hdc, 0, 22 + i, 7, 1, bdc, 0, 22, SRCCOPY);
				BitBlt(hdc, 292 + (sizeX - DEFAULT_SKIN_WIDTH), 22 + i, 8, 1, bdc, 292, 22, SRCCOPY);
			}

			/*	
			 * The bottom
			 */

			BitBlt(hdc, 0, 260 + (sizeY - DEFAULT_SKIN_HEIGHT), 200, 115, bdc, 0, 260, SRCCOPY);
			BitBlt(hdc, 200 + (sizeX - DEFAULT_SKIN_WIDTH), 260 + (sizeY - DEFAULT_SKIN_HEIGHT), 100, 115, bdc, 200, 260, SRCCOPY);

			SelectObject(bdc, this->bottom);

			if(sizeX - DEFAULT_SKIN_WIDTH > 0) 
			{

				int pass = (sizeX - DEFAULT_SKIN_WIDTH) / 256;

				for(i=0; i < pass; i++) 
				{

					BitBlt(hdc, 200 + i*256, 260 + (sizeY - DEFAULT_SKIN_HEIGHT), 256, 115, bdc, 0, 0, SRCCOPY);
				}
		
				BitBlt(hdc, 200 + pass*256, 260 + (sizeY - DEFAULT_SKIN_HEIGHT), (sizeX - DEFAULT_SKIN_WIDTH) - pass*256, 115, bdc, 0, 0, SRCCOPY);
			}	

			/*
			 * Some info
			 */
			/*
			if(playback->HasVideo() && !this->compactMode && !playback->IsBuffering()) {

				HFONT oldFont;
				DWORD time, total;
				char  buffer[24];
				DWORD h, m, s, ht, mt, st;
				char *file;

				oldFont = (HFONT) SelectObject(hdc, (HGDIOBJ) this->font);

				SetBkMode(hdc, TRANSPARENT);
				
				if(strrchr(playback->GetFilename(), '\\') != NULL) {

					file = strrchr(playback->GetFilename(), '\\')+1;
				}
				else {

					file = playback->GetFilename();
				}
				
				TextOut(hdc, 20, 282 + (sizeY - DEFAULT_SKIN_HEIGHT), file, strlen(file));

				time = playback->GetActualTime();
				total = playback->GetTotalTime();

				h = time / 3600;
				m = (time - h*3600) / 60;
				s = (time - h*3600 - m*60);
				
				ht = total / 3600;
				mt = (total - ht*3600) / 60;
				st = (total - ht*3600 - mt*60);

				sprintf(buffer, "%.2d:%.2d:%.2d / %.2d:%.2d:%.2d", h, m, s, ht, mt, st);

				TextOut(hdc, 20, 296 + (sizeY - DEFAULT_SKIN_HEIGHT), buffer, strlen(buffer));

				SelectObject(hdc, (HGDIOBJ) oldFont);
			}
			*/
			/*
			 * Now the controls
			 *
			 */

			/*SelectObject(bdc, this->controls);	

			BitBlt(hdc, progressCursor.left, progressCursor.top, (progressCursor.right - progressCursor.left), (progressCursor.bottom - progressCursor.top), 
				   bdc, 2, 45, SRCCOPY);

			BitBlt(hdc, volumeCursor.left, volumeCursor.top, (volumeCursor.right - volumeCursor.left), (volumeCursor.bottom - volumeCursor.top), 
				   bdc, 32, 45, SRCCOPY);
			*/
			/*
			 * And the time info
			 */

			//if(!playback->desktopMode) {

			//	if(!playback->IsPlaying() && !playback->IsPaused()) 
			//	{	
			{

					brush = CreateSolidBrush(this->skinColor); 
	
					oldBrush = (HBRUSH) SelectObject(hdc, brush);
					Rectangle(hdc, 7, 22, 292 + (sizeX - DEFAULT_SKIN_WIDTH), 260 + (sizeY - DEFAULT_SKIN_HEIGHT)); 
		
					SelectObject(bdc, this->logo);

					if(sizeY > LOGO_HEIGHT + 135 + 22)
						BitBlt(hdc, (sizeX - LOGO_WIDTH)/2, 50 + (sizeY - DEFAULT_SKIN_HEIGHT)/2, LOGO_WIDTH, LOGO_HEIGHT, bdc, 0, 0, SRCCOPY);

					SelectObject(hdc, oldBrush);
					DeleteObject((HGDIOBJ) brush);
			//	}
			//	else 
			//	{

			//		if(playback->IsOverlay()) 
			//		{
		
		///				brush = CreateSolidBrush(DD_OVERLAY_COLORREF);	
		//				
		//				oldBrush = (HBRUSH) SelectObject(hdc, brush);
		//				Rectangle(hdc, 7, 22, 292 + (sizeX - DEFAULT_SKIN_WIDTH), 260 + (sizeY - DEFAULT_SKIN_HEIGHT)); 
		//
		//				SelectObject(hdc, oldBrush);
		///				DeleteObject((HGDIOBJ) brush);
			//		}
			//	}

				/*
				if(playback->IsPaused())
				{
					if(playback->IsOverlay()) 
					{
		
						brush = CreateSolidBrush(DD_OVERLAY_COLORREF);	
						
						oldBrush = (HBRUSH) SelectObject(hdc, brush);
						Rectangle(hdc, 7, 22, 292 + (sizeX - DEFAULT_SKIN_WIDTH), 260 + (sizeY - DEFAULT_SKIN_HEIGHT)); 
		
						SelectObject(hdc, oldBrush);
						DeleteObject((HGDIOBJ) brush);
					}else
					{
						brush = CreateSolidBrush(0); 
 			
						oldBrush = (HBRUSH) SelectObject(hdc, brush);
						Rectangle(hdc, 7, 22, 292 + (sizeX - DEFAULT_SKIN_WIDTH), 260 + (sizeY - DEFAULT_SKIN_HEIGHT)); 
	
						SelectObject(hdc, oldBrush);
						DeleteObject((HGDIOBJ) brush);
					}
				}
				*/
			}

			DeleteDC(bdc);
		}

		EndPaint(hwnd, &ps); 
	}
	/*
	else {//fullscreen

		if(playback->IsOverlay() && playback->HasVideo()) {
	
			RECT *fullRects;

			fullRects = playback->videoRenderer->GetFullscreenRects();

			if(fullRects != NULL) {

				GetClientRect(hwnd, &clientRect);

				hdc  = BeginPaint(hwnd, &ps);
	
				brush = CreateSolidBrush(0);	
				oldBrush = (HBRUSH) SelectObject(hdc, brush);
			
				Rectangle(hdc, fullRects[0].left, fullRects[0].top, fullRects[0].left + fullRects[0].right + 50, fullRects[0].top + fullRects[0].bottom); 
				Rectangle(hdc, fullRects[2].left, fullRects[2].top, fullRects[2].left + fullRects[2].right + 50, fullRects[2].top + fullRects[2].bottom + 50); 
			
				DeleteObject((HGDIOBJ) brush);
				
				brush = CreateSolidBrush(DD_OVERLAY_COLORREF_FULLSCREEN);	
							
				(HBRUSH) SelectObject(hdc, brush);
			
				Rectangle(hdc, fullRects[1].left, fullRects[1].top, fullRects[1].left + fullRects[1].right + 50, fullRects[1].top + fullRects[1].bottom); 
			
				SelectObject(hdc, oldBrush);
				DeleteObject((HGDIOBJ) brush);

				EndPaint(hwnd, &ps); 
			}
		}
		else {

			if(playback->HasVideo()) {

				hdc  = BeginPaint(hwnd, &ps);
	
				brush = CreateSolidBrush(0);	
				oldBrush = (HBRUSH) SelectObject(hdc, brush);
			
				Rectangle(hdc, 0, 0, sizeX, sizeY); 
				
				SelectObject(hdc, oldBrush);
				DeleteObject((HGDIOBJ) brush);

				EndPaint(hwnd, &ps); 
			}
		}
	}
*/
	return 1;
}


/*
 * Display the FPS
 *
 */



/*
 * Displays the buffering Bar
 *
 *
 */






/*
 * Update the rects according
 * to the new size of the window
 *
 */

int Skin::UpdateSize(HWND hwnd)
{

	int          sizeX, sizeY;
	RECT         clientRect;

	/*
 	 * Get some info about the
	 * window size and all.
	 */

	GetClientRect(hwnd, &clientRect);

	sizeX = clientRect.right - clientRect.left;
	sizeY = clientRect.bottom - clientRect.top;

	this->menuButton.left   = 2;
	this->menuButton.right  = 14;
	this->menuButton.top    = 3;
	this->menuButton.bottom = 14;

	this->minimizeButton.left   = 266 + (sizeX - DEFAULT_SKIN_WIDTH);
	this->minimizeButton.right  = 278 + (sizeX - DEFAULT_SKIN_WIDTH);
	this->minimizeButton.top    = 3;
	this->minimizeButton.bottom = 14;

	this->closeButton.left   = 283 + (sizeX - DEFAULT_SKIN_WIDTH);
	this->closeButton.right  = 295 + (sizeX - DEFAULT_SKIN_WIDTH);
	this->closeButton.top    = 3;
	this->closeButton.bottom = 14;


	this->resizeButton.left   = 282 + (sizeX - DEFAULT_SKIN_WIDTH);
	this->resizeButton.right  = 282 + 15 + (sizeX - DEFAULT_SKIN_WIDTH);
	this->resizeButton.top    = 357 + (sizeY - DEFAULT_SKIN_HEIGHT);
	this->resizeButton.bottom = 357 + 15 + (sizeY - DEFAULT_SKIN_HEIGHT);


	return 1;
}

/*
 * Updates special zones upon action
 *
 */

int Skin::SetActionStart(HWND hwnd, int action)
{
	/*
	HDC          hdc, bdc;
	
	hdc = GetDC(hwnd);
	bdc = CreateCompatibleDC(hdc);
 
	if(hdc == NULL || bdc == NULL) {

		return 0;
	}

	SelectObject(bdc, this->controls);

	switch(action) {



	default:
		break;
	}

	DeleteDC(bdc);
	ReleaseDC(hwnd, hdc);
	*/
	return 1;
}

int Skin::SetActionEnd(HWND hwnd, int action)
{
	HDC          hdc, bdc;
	
	hdc = GetDC(hwnd);
	bdc = CreateCompatibleDC(hdc);
 
	if(hdc == NULL || bdc == NULL) {

		return 0;
	}

	SelectObject(bdc, this->skin);

	switch(action) {



	default:
		break;
	}

	DeleteDC(bdc);
	ReleaseDC(hwnd, hdc);
	
	return 1;
}

/*
 * Returns the action corresponding
 * to the given mouse click inside 
 * the skin
 *
 */

int Skin::GetAction(int x, int y)
{
	if(isInsideRect(x, y, this->menuButton))
		return ACTION_MENU;

	if(isInsideRect(x, y, this->minimizeButton))
		return ACTION_MINIMIZE;

	if(isInsideRect(x, y, this->closeButton))
		return ACTION_CLOSE;

	if(isInsideRect(x, y, this->resizeButton))
		return ACTION_RESIZE;

	if(isInsideRect(x, y, *this->GetVideoRect()))
		return ACTION_PLAY_AREA;
	
	return ACTION_NONE;
}
