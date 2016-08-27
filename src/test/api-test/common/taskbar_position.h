#ifndef taskbar_position_h__
#define taskbar_position_h__

#include <Windows.h>


namespace cactus
{


enum emTaskbarPosition {
    TaskballPositionInvaild,
    TaskballPositionLeft,
    TaskballPositionRight,
    TaskballPositionTop,
    TaskballPositionBottom,
    TaskballPositionHide
};
/**
 * @brief 获取任务栏的位置
 * @return 成功返回任务栏的位置
 * @see SystemParametersInfo
 */
static emTaskbarPosition GetTaskbarPosition();


}

#endif // taskbar_position_h__
