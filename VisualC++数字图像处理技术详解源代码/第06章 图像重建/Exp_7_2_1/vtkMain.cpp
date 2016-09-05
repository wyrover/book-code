//=======================================================
// VTK环境测试程序
//=======================================================

#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkProperty.h"
#include "Windows.h"       

//-------------------------------------------------------
// 主函数
//-------------------------------------------------------
int main()
{
    // 创建一个圆锥体源对象
    vtkConeSource *cone = vtkConeSource::New();
    // 设置圆锥体的高
    cone->SetHeight( 3.0 );
    // 设置圆锥体的半径
    cone->SetRadius( 1.0 );
    // 设置圆锥体的精细度
    cone->SetResolution( 10 );

    // 创建一个数据映射对象
    vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
    // 将圆锥体源对象关联到数据映射对象
    coneMapper->SetInputConnection( cone->GetOutputPort() );

    // 创建一个演员对象
    vtkActor *coneActor = vtkActor::New();
    // 将数据映射对象关联到演员对象
    coneActor->SetMapper( coneMapper );
   
    // 创建一个演示者对象
    vtkRenderer *aRenderer= vtkRenderer::New();
    // 将演员对象添加到演示者对象
    aRenderer->AddActor( coneActor );
    // 设置演示者对象的背景颜色
    aRenderer->SetBackground( 0.1, 0.2, 0.4 );

    // 创建一个绘制窗口对象
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    // 将演示者对象添加到绘制窗口中
    renWin->AddRenderer( aRenderer );
    // 设置绘制窗口大小
    renWin->SetSize( 300, 300 );

    // 从不同角度观察圆锥体
    for (int i = 0; i < 25; ++i)
    {
        // 绘制
        renWin->Render();
        // 暂停
        Sleep(500);
        // 改变视角
        aRenderer->GetActiveCamera()->Azimuth( i );
    }

    // 清除对象，释放空间
    cone->Delete();
    coneMapper->Delete();
    coneActor->Delete();
    aRenderer->Delete();
    renWin->Delete();

    return 0;
}
