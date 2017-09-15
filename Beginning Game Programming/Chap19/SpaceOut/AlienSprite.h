//-----------------------------------------------------------------
// Alien Sprite Object
// C++ Header - AlienSprite.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Sprite.h"

//-----------------------------------------------------------------
// AlienSprite Class
//-----------------------------------------------------------------
class AlienSprite : public Sprite
{
public:
    // Constructor(s)/Destructor
    AlienSprite(Bitmap* pBitmap, RECT& rcBounds,
                BOUNDSACTION baBoundsAction = BA_STOP);
    virtual ~AlienSprite();

    // General Methods
    virtual SPRITEACTION  Update();
    virtual Sprite*       AddSprite();
};
