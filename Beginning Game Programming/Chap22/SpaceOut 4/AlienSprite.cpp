//-----------------------------------------------------------------
// Alien Sprite Object
// C++ Source - AlienSprite.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "AlienSprite.h"

//-----------------------------------------------------------------
// External Global Variables
//-----------------------------------------------------------------
extern Bitmap* g_pBlobboBitmap;
extern Bitmap* g_pBMissileBitmap;
extern Bitmap* g_pJellyBitmap;
extern Bitmap* g_pJMissileBitmap;
extern Bitmap* g_pTimmyBitmap;
extern Bitmap* g_pTMissileBitmap;
extern int     g_iDifficulty;

//-----------------------------------------------------------------
// AlienSprite Constructor(s)/Destructor
//-----------------------------------------------------------------
AlienSprite::AlienSprite(Bitmap* pBitmap, RECT& rcBounds,
                         BOUNDSACTION baBoundsAction) : Sprite(pBitmap, rcBounds,
                                     baBoundsAction)
{
}

AlienSprite::~AlienSprite()
{
}

//-----------------------------------------------------------------
// AlienSprite General Methods
//-----------------------------------------------------------------
SPRITEACTION AlienSprite::Update()
{
    // Call the base sprite Update() method
    SPRITEACTION saSpriteAction;
    saSpriteAction = Sprite::Update();

    // See if the alien should fire a missile
    if ((rand() % (g_iDifficulty / 2)) == 0)
        saSpriteAction |= SA_ADDSPRITE;

    return saSpriteAction;
}

Sprite* AlienSprite::AddSprite()
{
    // Create a new missile sprite
    RECT    rcBounds = { 0, 0, 640, 410 };
    RECT    rcPos = GetPosition();
    Sprite* pSprite = NULL;

    if (GetBitmap() == g_pBlobboBitmap) {
        // Blobbo missile
        pSprite = new Sprite(g_pBMissileBitmap, rcBounds, BA_DIE);
        pSprite->SetVelocity(0, 7);
    } else if (GetBitmap() == g_pJellyBitmap) {
        // Jelly missile
        pSprite = new Sprite(g_pJMissileBitmap, rcBounds, BA_DIE);
        pSprite->SetVelocity(0, 5);
    } else {
        // Timmy missile
        pSprite = new Sprite(g_pTMissileBitmap, rcBounds, BA_DIE);
        pSprite->SetVelocity(0, 3);
    }

    // Set the missile sprite's position and return it
    pSprite->SetPosition(rcPos.left + (GetWidth() / 2), rcPos.bottom);
    return pSprite;
}
