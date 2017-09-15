//-----------------------------------------------------------------
// ScrollingBackground Object
// C++ Source - ScrollingBackground.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "ScrollingBackground.h"

//-----------------------------------------------------------------
// BackgroundLayer Constructor(s)/Destructor
//-----------------------------------------------------------------
// Create a background layer from a bitmap file
BackgroundLayer::BackgroundLayer(HDC hDC, LPTSTR szFileName,
                                 int iSpeed, SCROLLDIR sdDirection)
    : Bitmap(hDC, szFileName), m_iSpeed(iSpeed), m_sdDirection(sdDirection)
{
    // Set the viewport to the entire layer image by default
    m_rcViewport.left = m_rcViewport.top = 0;
    m_rcViewport.right = m_iWidth;
    m_rcViewport.bottom = m_iHeight;
}

// Create a background layer from a bitmap resource
BackgroundLayer::BackgroundLayer(HDC hDC, UINT uiResID, HINSTANCE hInstance,
                                 int iSpeed, SCROLLDIR sdDirection)
    : Bitmap(hDC, uiResID, hInstance), m_iSpeed(iSpeed),
      m_sdDirection(sdDirection)
{
    // Set the viewport to the entire layer image by default
    m_rcViewport.left = m_rcViewport.top = 0;
    m_rcViewport.right = m_iWidth;
    m_rcViewport.bottom = m_iHeight;
}

void BackgroundLayer::Draw(HDC hDC, int x, int y, BOOL bTrans,
                           COLORREF crTransColor)
{
    // Draw only the part of the layer seen through the viewport
    if (m_rcViewport.top < 0 && m_rcViewport.left < 0) {
        // Draw the split viewport wrapping top to bottom AND left to right
        DrawPart(hDC, x, y,
                 m_iWidth + m_rcViewport.left, m_iHeight + m_rcViewport.top,
                 -m_rcViewport.left, -m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x - m_rcViewport.left, y,
                 0, m_iHeight + m_rcViewport.top,
                 m_rcViewport.right, -m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x, y - m_rcViewport.top,
                 m_iWidth + m_rcViewport.left, 0,
                 -m_rcViewport.left, m_rcViewport.bottom,
                 bTrans, crTransColor);
        DrawPart(hDC, x - m_rcViewport.left, y - m_rcViewport.top,
                 0, 0,
                 m_rcViewport.right, m_rcViewport.bottom,
                 bTrans, crTransColor);
    } else if (m_rcViewport.top < 0 && m_rcViewport.right > m_iWidth) {
        // Draw the split viewport wrapping top to bottom AND right to left
        DrawPart(hDC, x, y,
                 m_rcViewport.left, m_iHeight + m_rcViewport.top,
                 m_iWidth - m_rcViewport.left, -m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x + (m_iWidth - m_rcViewport.left), y,
                 0, m_iHeight + m_rcViewport.top,
                 m_rcViewport.right - m_iWidth, -m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x, y - m_rcViewport.top,
                 m_rcViewport.left, 0,
                 m_iWidth - m_rcViewport.left, m_rcViewport.bottom,
                 bTrans, crTransColor);
        DrawPart(hDC, x + (m_iWidth - m_rcViewport.left), y - m_rcViewport.top,
                 0, 0,
                 m_rcViewport.right - m_iWidth, m_rcViewport.bottom,
                 bTrans, crTransColor);
    } else if (m_rcViewport.bottom > m_iHeight && m_rcViewport.left < 0) {
        // Draw the split viewport wrapping bottom to top AND left to right
        DrawPart(hDC, x, y,
                 m_iWidth + m_rcViewport.left, m_rcViewport.top,
                 -m_rcViewport.left, m_iHeight - m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x - m_rcViewport.left, y,
                 0, m_rcViewport.top,
                 m_rcViewport.right, m_iHeight - m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x, y + (m_iHeight - m_rcViewport.top),
                 m_iWidth + m_rcViewport.left, 0,
                 -m_rcViewport.left, m_rcViewport.bottom - m_iHeight,
                 bTrans, crTransColor);
        DrawPart(hDC, x - m_rcViewport.left, y + (m_iHeight - m_rcViewport.top),
                 0, 0,
                 m_rcViewport.right, m_rcViewport.bottom - m_iHeight,
                 bTrans, crTransColor);
    } else if (m_rcViewport.bottom > m_iHeight && m_rcViewport.right > m_iWidth) {
        // Draw the split viewport wrapping bottom to top AND right to left
        DrawPart(hDC, x, y,
                 m_rcViewport.left, m_rcViewport.top,
                 m_iWidth - m_rcViewport.left, m_iHeight - m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x + (m_iWidth - m_rcViewport.left), y,
                 0, m_rcViewport.top,
                 m_rcViewport.right - m_iWidth, m_iHeight - m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x, y + (m_iHeight - m_rcViewport.top),
                 m_rcViewport.left, 0,
                 m_iWidth - m_rcViewport.left, m_rcViewport.bottom - m_iHeight,
                 bTrans, crTransColor);
        DrawPart(hDC, x + (m_iWidth - m_rcViewport.left), y + (m_iHeight - m_rcViewport.top),
                 0, 0,
                 m_rcViewport.right - m_iWidth, m_rcViewport.bottom - m_iHeight,
                 bTrans, crTransColor);
    } else if (m_rcViewport.top < 0) {
        // Draw the split viewport wrapping from top to bottom
        DrawPart(hDC, x, y,
                 m_rcViewport.left, m_iHeight + m_rcViewport.top,
                 m_rcViewport.right - m_rcViewport.left, -m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x, y - m_rcViewport.top,
                 m_rcViewport.left, 0,
                 m_rcViewport.right - m_rcViewport.left, m_rcViewport.bottom,
                 bTrans, crTransColor);
    } else if (m_rcViewport.right > m_iWidth) {
        // Draw the split viewport wrapping from right to left
        DrawPart(hDC, x, y,
                 m_rcViewport.left, m_rcViewport.top,
                 m_iWidth - m_rcViewport.left, m_rcViewport.bottom - m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x + (m_iWidth - m_rcViewport.left), y,
                 0, m_rcViewport.top,
                 m_rcViewport.right - m_iWidth, m_rcViewport.bottom - m_rcViewport.top,
                 bTrans, crTransColor);
    } else if (m_rcViewport.bottom > m_iHeight) {
        // Draw the split viewport wrapping from bottom to top
        DrawPart(hDC, x, y,
                 m_rcViewport.left, m_rcViewport.top,
                 m_rcViewport.right - m_rcViewport.left, m_iHeight - m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x, y + (m_iHeight - m_rcViewport.top),
                 m_rcViewport.left, 0,
                 m_rcViewport.right - m_rcViewport.left, m_rcViewport.bottom - m_iHeight,
                 bTrans, crTransColor);
    } else if (m_rcViewport.left < 0) {
        // Draw the split viewport wrapping from left to right
        DrawPart(hDC, x, y,
                 m_iWidth + m_rcViewport.left, m_rcViewport.top,
                 -m_rcViewport.left, m_rcViewport.bottom - m_rcViewport.top,
                 bTrans, crTransColor);
        DrawPart(hDC, x - m_rcViewport.left, y,
                 0, m_rcViewport.top,
                 m_rcViewport.right, m_rcViewport.bottom - m_rcViewport.top,
                 bTrans, crTransColor);
    } else
        // Draw the entire viewport at once
        DrawPart(hDC, x, y,
                 m_rcViewport.left, m_rcViewport.top,
                 m_rcViewport.right - m_rcViewport.left, m_rcViewport.bottom - m_rcViewport.top,
                 bTrans, crTransColor);
}

void BackgroundLayer::Update()
{
    switch (m_sdDirection) {
    case SD_UP:
        // Move the layer up (slide the viewport down)
        m_rcViewport.top += m_iSpeed;
        m_rcViewport.bottom += m_iSpeed;

        if (m_rcViewport.top > m_iHeight) {
            m_rcViewport.bottom = m_rcViewport.bottom - m_rcViewport.top;
            m_rcViewport.top = 0;
        }

        break;

    case SD_RIGHT:
        // Move the layer right (slide the viewport left)
        m_rcViewport.left -= m_iSpeed;
        m_rcViewport.right -= m_iSpeed;

        if (m_rcViewport.right < 0) {
            m_rcViewport.left = m_iWidth - (m_rcViewport.right - m_rcViewport.left);
            m_rcViewport.right = m_iWidth;
        }

        break;

    case SD_DOWN:
        // Move the layer down (slide the viewport up)
        m_rcViewport.top -= m_iSpeed;
        m_rcViewport.bottom -= m_iSpeed;

        if (m_rcViewport.bottom < 0) {
            m_rcViewport.top = m_iHeight - (m_rcViewport.bottom - m_rcViewport.top);
            m_rcViewport.bottom = m_iHeight;
        }

        break;

    case SD_LEFT:
        // Move the layer left (slide the viewport right)
        m_rcViewport.left += m_iSpeed;
        m_rcViewport.right += m_iSpeed;

        if (m_rcViewport.left > m_iWidth) {
            m_rcViewport.right = (m_rcViewport.right - m_rcViewport.left);
            m_rcViewport.left = 0;
        }

        break;
    }
}

//-----------------------------------------------------------------
// ScrollingBackground Constructor(s)/Destructor
//-----------------------------------------------------------------
ScrollingBackground::ScrollingBackground(int iWidth, int iHeight)
    : Background(iWidth, iHeight, 0), m_iNumLayers(0)
{
}

ScrollingBackground::~ScrollingBackground()
{
}

//-----------------------------------------------------------------
// ScrollingBackground General Methods
//-----------------------------------------------------------------
void ScrollingBackground::Update()
{
    // Update the layers
    for (int i = 0; i < m_iNumLayers; i++)
        m_pLayers[i]->Update();
}

void ScrollingBackground::Draw(HDC hDC, BOOL bTrans,
                               COLORREF crTransColor)
{
    // Draw the layers
    for (int i = 0; i < m_iNumLayers; i++)
        m_pLayers[i]->Draw(hDC, 0, 0, bTrans, crTransColor);
}

void ScrollingBackground::AddLayer(BackgroundLayer* pLayer)
{
    // Add the new layer (maximum of 10)
    if (m_iNumLayers < 10)
        m_pLayers[m_iNumLayers++] = pLayer;
}
