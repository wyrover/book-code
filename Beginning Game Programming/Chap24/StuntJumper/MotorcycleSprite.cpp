//-----------------------------------------------------------------
// Motorcycle Sprite Object
// C++ Source - MotorcycleSprite.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "MotorcycleSprite.h"

//-----------------------------------------------------------------
// External Global Variables
//-----------------------------------------------------------------
extern HINSTANCE  g_hInstance;
extern int        g_bGameOver;

//-----------------------------------------------------------------
// MotorcycleSprite Constructor(s)/Destructor
//-----------------------------------------------------------------
MotorcycleSprite::MotorcycleSprite(Bitmap* pBitmap, RECT& rcBounds,
                                   BOUNDSACTION baBoundsAction) : Sprite(pBitmap, rcBounds,
                                               baBoundsAction), m_iMINSPEED(1), m_iMAXSPEED(10), m_iHANGTIME(6)
{
    // Initialize the motorcycle sprite variables
    m_iCurFrame = 6;  // the frame where the motorcycle is level
    m_bJumping = FALSE;
    m_iJumpCounter = 0;
    m_bLandedSafely = TRUE;
}

MotorcycleSprite::~MotorcycleSprite()
{
}

//-----------------------------------------------------------------
// MotorcycleSprite Virtual Methods
//-----------------------------------------------------------------
void MotorcycleSprite::UpdateFrame()
{
    if (m_bJumping) {
        // Start jumping
        if (m_iJumpCounter-- >= 0) {
            // See if the motorcycle is on the way up
            if (m_iJumpCounter > (m_ptVelocity.x * m_iHANGTIME / 2)) {
                // Change the frame to show the motorcycle tilt upward
                m_iCurFrame = min(m_iCurFrame + 1, 12);

                // Change the vertical velocity to cause the motorcycle to ascend
                if (m_iJumpCounter % (m_iHANGTIME / 2) == 0)
                    m_ptVelocity.y++;
            }
            // See if the motorcycle is on the way down
            else if (m_iJumpCounter <= (m_ptVelocity.x * m_iHANGTIME / 2)) {
                // Change the frame to show the motorcycle tilt downward
                m_iCurFrame = max(m_iCurFrame - 1, 0);

                // Change the vertical velocity to cause the motorcycle to descend
                if (m_iJumpCounter % (m_iHANGTIME / 2) == 0)
                    m_ptVelocity.y++;
            }
        } else {
            // Stop the jump and level the motorcycle
            m_bJumping = FALSE;
            m_iCurFrame = 6;
            m_ptVelocity.y = 0;

            // See if the motorcycle overshot the landing ramp
            if (!m_bLandedSafely) {
                // Play the crash sound
                PlaySound((LPCSTR)IDW_CRASH, g_hInstance, SND_ASYNC |
                          SND_RESOURCE);
                // End the game
                m_ptVelocity.x = 0;
                g_bGameOver = TRUE;
            }
        }
    }
}

//-----------------------------------------------------------------
// MotorcycleSprite General Methods
//-----------------------------------------------------------------
void MotorcycleSprite::IncreaseSpeed()
{
    if (!m_bJumping)
        // Increase the horizontal speed of the motorcycle
        m_ptVelocity.x = min(m_ptVelocity.x + 1, m_iMAXSPEED);
}

void MotorcycleSprite::DecreaseSpeed()
{
    if (!m_bJumping)
        // Decrease the horizontal speed of the motorcycle
        m_ptVelocity.x = max(m_ptVelocity.x - 1, m_iMINSPEED);
}

void MotorcycleSprite::StartJumping()
{
    if (!m_bJumping) {
        // Start the motorcycle jumping
        m_iJumpCounter = m_ptVelocity.x * m_iHANGTIME;
        m_ptVelocity.y = -m_ptVelocity.x;
        m_bJumping = TRUE;
        m_bLandedSafely = FALSE;
    }
}

void MotorcycleSprite::LandedSafely()
{
    // Flag the motorcycle as having landed safely (hit the landing ramp)
    m_bLandedSafely = TRUE;
}
