#include "taskbar_position.h"

namespace cactus
{

cactus::emTaskbarPosition GetTaskbarPosition()
{
    RECT rcWA;
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWA, 0);

    if (rcWA.left > 0) {
        return TaskballPositionLeft;
    } else if (rcWA.top > 0) {
        return TaskballPositionTop;
    } else {
        int iScrWidth = ::GetSystemMetrics(SM_CXSCREEN);
        int iScrHeight = ::GetSystemMetrics(SM_CYSCREEN);

        if (rcWA.bottom - rcWA.top < iScrHeight) {
            return TaskballPositionBottom;
        } else if (rcWA.right - rcWA.left < iScrWidth) {
            return TaskballPositionRight;
        } else {
            return TaskballPositionHide;
        }
    }
}

}