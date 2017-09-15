//-----------------------------------------------------------------
// Person Sprite Object
// C++ Header - PersonSprite.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Sprite.h"

//-----------------------------------------------------------------
// PersonSprite Class
//-----------------------------------------------------------------
class PersonSprite : public Sprite
{
public:
    // Constructor(s)/Destructor
    PersonSprite(Bitmap* pBitmap, RECT& rcBounds,
                 BOUNDSACTION baBoundsAction = BA_STOP);
    virtual ~PersonSprite();

    // Helper Methods
    virtual void  UpdateFrame();

    // General Methods
    void Walk();
};
