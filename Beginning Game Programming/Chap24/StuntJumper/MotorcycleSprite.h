//-----------------------------------------------------------------
// Motorcycle Sprite Object
// C++ Header - MotorcycleSprite.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Resource.h"
#include "Sprite.h"

//-----------------------------------------------------------------
// MotorcycleSprite Class
//-----------------------------------------------------------------
class MotorcycleSprite : public Sprite
{
protected:
    // Member Variables
    const int m_iMINSPEED, m_iMAXSPEED;
    const int m_iHANGTIME;
    BOOL      m_bJumping;
    int       m_iJumpCounter;
    BOOL      m_bLandedSafely;

public:
    // Constructor(s)/Destructor
    MotorcycleSprite(Bitmap* pBitmap, RECT& rcBounds,
                     BOUNDSACTION baBoundsAction = BA_STOP);
    virtual ~MotorcycleSprite();

    // Helper Methods
    virtual void  UpdateFrame();

    // General Methods
    void IncreaseSpeed();
    void DecreaseSpeed();
    void StartJumping();
    void LandedSafely();
    BOOL HasLandedSafely()
    {
        return m_bLandedSafely;
    };
};
