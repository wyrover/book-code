//-----------------------------------------------------------------
// Person Sprite Object
// C++ Source - PersonSprite.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "PersonSprite.h"

//-----------------------------------------------------------------
// PersonSprite Constructor(s)/Destructor
//-----------------------------------------------------------------
PersonSprite::PersonSprite(Bitmap* pBitmap, RECT& rcBounds,
                           BOUNDSACTION baBoundsAction) : Sprite(pBitmap, rcBounds,
                                       baBoundsAction)
{
}

PersonSprite::~PersonSprite()
{
}

//-----------------------------------------------------------------
// PersonSprite Virtual Methods
//-----------------------------------------------------------------
inline void PersonSprite::UpdateFrame()
{
}

//-----------------------------------------------------------------
// PersonSprite General Methods
//-----------------------------------------------------------------
void PersonSprite::Walk()
{
    // Toggle between the two frames to give the illusion of walking
    m_iCurFrame = 1 - m_iCurFrame;
}
