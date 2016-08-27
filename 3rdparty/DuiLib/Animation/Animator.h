/*!
* \file Animator.h
* \brief 动画类库
*
* 动画类库声明
*
* \author wangyang
* \date 2015/02/15
* \version 1.0
*/

/*
Example:

void do_animator()
{
    TThreadMethod update_callback = boost::bind(&CMainFrm::animator_update, this);
    TThreadMethod complate_callback = boost::bind(&CMainFrm::animator_complate, this);
    animator_.reset(new DuiLib::Animator());
    animator_->set_update_callback(update_callback);
    animator_->set_complete_callback(complate_callback);
    animator_->set_elapsed(1000);
    animator_->set_start_value(pCon->GetPos().top);
    animator_->set_end_value(100);
    animator_->set_tweener(tween::BACK, tween::EASE_IN_OUT);
    animator_->start();
}


void CMainFrm::animator_update()
{
    RECT rect;
    CContainerUI* pCon = reinterpret_cast<CContainerUI*>(m_PaintManager.FindControl(_T("MainFrameOpBtnView")));     rect = pCon->GetPos();
    rect.top = animator_->get_value();
    pCon->SetPos(rect);
}

void CMainFrm::animator_complate()
{
    CContainerUI* pCon = reinterpret_cast<CContainerUI*>(m_PaintManager.FindControl(_T("MainFrameOpBtnView")));
    m_PaintManager.SendNotify(pCon, DUI_MSGTYPE_CHANGEMAINPAGE);
}
*/

#ifndef __ANIMATOR_H__
#define __ANIMATOR_H__
#include <cactus/cactus_thread_helper.h>
#include "DuiLib/Animation/CppTweener.h"
#include <cactus/cactus_pattern.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace cactus;
using namespace tween;


namespace DuiLib
{
/*! @brief tween动画接口
 *
 * tween动画接口
 */
class IAnimator
{
public:
    virtual ~IAnimator() { }
    /*! @brief 动画开始
     *
     * @return 返回空
     */
    virtual void start() = 0;
    /*! @brief 动画暂停
     *
     * @return 返回空
     */
    virtual void pause() = 0;
    /*! @brief 动画取消
     *
     * @return 返回空
     */
    virtual void cancel() = 0;
    /*! @brief 动画终止
     *
     * @return 返回空
     */
    virtual void end() = 0;
    /*! @brief 是否正在运行
     *
     * @return 返回true表示成功，否则为失败
     */
    virtual bool is_running() = 0;
    /*! @brief 设置tween方法
     *
     * @param[in] quart 参数说明
     * @param[in] easy 参数说明
     * @return 返回空
     */
    virtual void set_tweener(int quart, int easy) = 0;
    /*! @brief 设置动画循环次数
     *
     * @param[in] count 动画循环次数
     * @return 返回空
     */
    virtual void set_repeat_count(int value = -1) = 0;
    /*! @brief 设置动画反向
     *
     * @param[in] value 动画是否反向
     * @return 返回空
     */
    virtual void set_reverse(bool value) = 0;
    /*! @brief 设置动画用时
     *
     * @param[in] value 用时 毫秒
     * @return 返回空
     */
    virtual void set_interval(float value) = 0;
    /*! @brief 设置初始值
     *
     * @param[in] value 初始值
     * @return 返回空
     */
    virtual void set_start_value(double value) = 0;
    /*! @brief 设置终结值
     *
     * @param[in] value 终结值
     * @return 返回空
     */
    virtual void set_end_value(double value) = 0;
    /*! @brief 获取当前运行值
     *
     * @return 返回当前运行值
     */
    virtual double get_value() = 0;
    /*! @brief 设置动画运行回调方法
     *
     * @param[in] update_callback 更新回调方法
     * @return 返回空
     */
    virtual void set_update_callback(TThreadMethod update_callback) = 0;
    /*! @brief 设置动画完成回调方法
     *
     * @param[in] complete_callback 动画完成回调方法
     * @return 返回空
     */
    virtual void set_complete_callback(TThreadMethod complete_callback) = 0;

    virtual void update(long currentMillis) = 0;
    virtual void set_name(LPCTSTR name) = 0;
};

class UILIB_API Animator : public IAnimator, TweenerListener
{
public:
    Animator();
    virtual ~Animator();
public:
    void start();
    void cancel();
    void end();
    void pause();
    void resume();
    bool is_paused();
    bool is_running();
    //bool is_complated();
    bool is_started();
    void set_tweener(int quart, int easy);
    void set_repeat_count(int value = -1);
    void set_reverse(bool value);
    void set_interval(float speed);
    void set_start_value(double value);
    void set_end_value(double value);
    void set_name(LPCTSTR name);
    double get_value();
    void set_update_callback(TThreadMethod update_callback);
    void set_complete_callback(TThreadMethod complete_callback);
    void update(long currentMillis);
public:
    void onStart(const TweenerParam& param);
    void onStep(const TweenerParam& param);
    void onComplete(const TweenerParam& param);
private:
    //void _run();
protected:
    bool paused_;
    bool running_;
    //bool complated_;
private:
    boost::shared_ptr<boost::thread>    thrd_;
    boost::recursive_mutex                        mutex_;
    //boost::condition_variable           condition_;
    TThreadMethod                       update_callback_;
    TThreadMethod                       complate_callback_;
    tween::Tweener                      tweener_;
    float                               speed_;
    double                              start_pos_;
    double                              lost_pos_;
    double                              new_pos_;
    int                                 quart_;
    int                                 easy_;
    int                                 repeat_;
    int                                 reverse_;
    std::wstring                        name_;
};

//typedef boost::shared_ptr<IAnimator> IAnimatorPtr;
typedef IAnimator* IAnimatorPtr;

class AnimatorManagerImpl
{
public:
    AnimatorManagerImpl();
    ~AnimatorManagerImpl();
public:
    void add_animator(IAnimatorPtr animator);
    void remove_animator(IAnimatorPtr animator);
    bool exist(IAnimatorPtr animator);
private:
    void _run();
private:
    std::vector<IAnimatorPtr>           animator_list_;
    boost::shared_ptr<boost::thread>    thrd_;
    boost::recursive_mutex                        mutex_;
    boost::mutex                        con_mutex_;
    boost::condition_variable           condition_;
};

typedef cactus::Singleton<AnimatorManagerImpl> AnimatorManager;

} // namespace DuiLib



#endif // __ANIMATOR_H__
