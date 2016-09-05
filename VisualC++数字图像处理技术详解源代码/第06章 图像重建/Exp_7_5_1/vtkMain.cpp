//=======================================================
// 基于MC算法的CT图像重建
//=======================================================

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolume16Reader.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkOutlineFilter.h"
#include "vtkCamera.h"
#include "vtkPolyDataMapper.h"
#include "vtkStripper.h"
#include "vtkPolyDataNormals.h"
#include "vtkMarchingCubes.h"

//-------------------------------------------------------
// 主函数
//-------------------------------------------------------
int main (int argc, char **argv)
{

    // 参数个数检查
    if(argc < 2)
    {
        cout << "用法： " << argv[0] << " DATADIR/headsq/quarter" << endl;
        return 1;
    }

    // ---------读取数据---------

    // 创建2D数据读者对象
    vtkVolume16Reader *v16 = vtkVolume16Reader::New();
    // 设置数据的各维大小
    v16->SetDataDimensions(64,64);
    // 设置数据的字节排列方式
    v16->SetDataByteOrderToLittleEndian();
    // 设置文件前辍，它将结合下一参数，以“FilePrefix.%d”方式读取随后文件
    v16->SetFilePrefix (argv[1]);
    // 设置文件读取范围
    v16->SetImageRange(1, 93);
    // 设置CT数据的切片间距和间隔
    v16->SetDataSpacing (3.2, 3.2, 1.5);  
    
    // ---------重建皮肤---------

    // 创建一个Marching Cubes算法的对象
    vtkMarchingCubes *skinExtractor = vtkMarchingCubes::New();
    // 获得所读取的CT数据
    skinExtractor->SetInputConnection(v16->GetOutputPort());
    // 提取出CT值为500的皮肤数据
    skinExtractor->SetValue(0, 500);
    // 重新计算法向量
    vtkPolyDataNormals *skinNormals = vtkPolyDataNormals::New();
    skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
    skinNormals->SetFeatureAngle(60.0);
    // 创建三角带对象
    vtkStripper *skinStripper = vtkStripper::New();
    // 将生成的三角片连接成三角带
    skinStripper->SetInputConnection(skinNormals->GetOutputPort());
    // 创建一个数据映射对象
    vtkPolyDataMapper *skinMapper = vtkPolyDataMapper::New();
    // 将三角带映射为几何数据
    skinMapper->SetInputConnection(skinStripper->GetOutputPort()); 
    skinMapper->ScalarVisibilityOff();
    // 创建一个代表皮肤的演员对象
    vtkActor *skin = vtkActor::New();
    // 获得皮肤几何数据的属性
    skin->SetMapper(skinMapper);
    // 设置皮肤颜色的属性
    skin->GetProperty()->SetDiffuseColor(1, .49, .25);
    // 设置反射率
    skin->GetProperty()->SetSpecular(.3);
    // 设置反射光强度 
    skin->GetProperty()->SetSpecularPower(20);
    // 设置不透明度
    skin->GetProperty()->SetOpacity(1.0);

    // ---------重建骨骼---------

    // 创建一个Marching Cubes算法的对象
    vtkMarchingCubes *boneExtractor = vtkMarchingCubes::New();
    // 获得所读取的CT数据
    boneExtractor->SetInputConnection(v16->GetOutputPort());
    // 提取出CT值为1150的骨骼数据
    boneExtractor->SetValue(0, 1150);
    // 重新计算法向量
    vtkPolyDataNormals *boneNormals = vtkPolyDataNormals::New();
    boneNormals->SetInputConnection(boneExtractor->GetOutputPort());
    boneNormals->SetFeatureAngle(60.0);
    // 创建三角带对象
    vtkStripper *boneStripper = vtkStripper::New();
    // 将生成的三角片连接成三角带
    boneStripper->SetInputConnection(boneNormals->GetOutputPort());
    // 创建一个数据映射对象
    vtkPolyDataMapper *boneMapper = vtkPolyDataMapper::New();
    // 将三角带映射为几何数据
    boneMapper->SetInputConnection(boneStripper->GetOutputPort());
    boneMapper->ScalarVisibilityOff();
    // 创建一个代表骨骼的演员对象
    vtkActor *bone = vtkActor::New();
    // 获得骨骼几何数据的属性
    bone->SetMapper(boneMapper);
    // 设置骨骼颜色的属性
    bone->GetProperty()->SetDiffuseColor(1, 1, .9412);

    // ---------建立轮廓---------

    // 在生成图像的四周建立轮廓，以方便与用户交互
    vtkOutlineFilter *outlineData = vtkOutlineFilter::New();
    outlineData->SetInputConnection(v16->GetOutputPort());
    vtkPolyDataMapper *mapOutline = vtkPolyDataMapper::New();
    mapOutline->SetInputConnection(outlineData->GetOutputPort());
    vtkActor *outline = vtkActor::New();
    outline->SetMapper(mapOutline);
    outline->GetProperty()->SetColor(0,0,0);

    // ---------建立相机---------

    // 创建一个相机对象
    vtkCamera *aCamera = vtkCamera::New();
    // 设置相机的视角
    aCamera->SetViewUp (0, 0, -1);
    // 设置相机的位置
    aCamera->SetPosition (0, 1, 0);
    // 设置相机的焦点
    aCamera->SetFocalPoint (0, 0, 0);
    aCamera->ComputeViewPlaneNormal();

    // ---------准备演示---------
    
    // 创建演示者对象
    vtkRenderer *aRenderer = vtkRenderer::New();
    // 创建演示窗口对象
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    // 将演示者添加到演示窗口
    renWin->AddRenderer(aRenderer);
    // 创建演示窗口交互对象，使得用户可以用鼠标、键盘和窗口交互
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
    // 将演示窗口关联到演示窗口交互对象
    iren->SetRenderWindow(renWin);

    // 将轮廓添加到演示者对象
    aRenderer->AddActor(outline);
    // 将皮肤添加到演示者对象
    aRenderer->AddActor(skin);
    // 将骨骼添加到演示者对象
    aRenderer->AddActor(bone);
    // 设置演示者所使用的相机
    aRenderer->SetActiveCamera(aCamera);
    // 使相机生效
    aRenderer->ResetCamera();
    aCamera->Dolly(1.5);  
    // 设置演示者的背景颜色
    aRenderer->SetBackground(1,1,1);
    // 调整相机截取平面
    aRenderer->ResetCameraClippingRange();

    // 设置演示窗口大小
    renWin->SetSize(640, 480);

    // ---------开始演示---------

    // 演示窗口交互对象初始化
    iren->Initialize();
    // 开始演示
    iren->Start(); 

    // ---------演示结束---------

    // 删除对象，释放空间
    v16->Delete();
    skinExtractor->Delete();
    skinNormals->Delete();
    skinStripper->Delete();
    skinMapper->Delete();
    skin->Delete();
    boneExtractor->Delete();
    boneNormals->Delete();
    boneStripper->Delete();
    boneMapper->Delete();
    bone->Delete();
    outlineData->Delete();
    mapOutline->Delete();
    outline->Delete();
    aCamera->Delete();
    aRenderer->Delete();
    renWin->Delete();
    iren->Delete();

    return 0;
}
