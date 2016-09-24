#ifndef XAPISPY_H
#define XAPISPY_H

#include <QMainWindow>

#include <QtWidgets>
#include <QSet>
#include <set>
#include "../../../nNetLib/nNetBase.h"
#include "../../../nCom/npacketbase.h"
#include "../../../nCom/nlinkedlist.h"

#include "moduletreemodel.h"
#include "apitablemodel.h"
#include "patchtablemodel.h"

#include "../xVMRuntime/xvmresource.h"
#include "resourcetreemodel.h"

#include "hexview/NHexView.h"

#include "../xVMRuntime/xvmiatcall.h"

#include "importtreemodel.h"

namespace Ui {
class xApiSpy;
}

struct memory_block
{
    ULONG_PTR   addr;
    ULONG_PTR   size;
    int         state;
    int         protect;
    QString     name;
    QByteArray  data;
    void clear()
    {
        data.clear();
        name.clear();
        state = 0;
        addr = 0;
        size = 0;
    }
};

struct iatcall_node : public iatcall_data{

};

typedef QMap<ULONG_PTR,iatcall_node> iatcall_map;





int BuildImportSection(ImportSymbols& impSyms,DWORD buildRVA,QByteArray* data,BOOL is32Bit);

//我们直接从NNetBase网络通信库基层，该通信库同时支持
//tcp,udp,ucp/*udp实现的可信连接*/,pipe
class xApiSpy : public QMainWindow,public NNetBase
{
    Q_OBJECT

    enum target_state{
        target_stoped,          //没有活动目标进程
        target_running,         //目标进程处于活动当中
        target_paused,          //目标进程已被我们的监视器暂停
        target_request_pause,    //请求暂停目标进程
        target_startup
    };

    enum snap_state{
        snap_snap1,
        snap_snap2,
        snap_processsnapshot
    };
    typedef QList<memory_block> memoryblocks;

    enum{
        action_setbreak = 0,
        action_delbreak,
        action_check,
        action_uncheck
    };
public:
    explicit xApiSpy(QWidget *parent = 0);
    ~xApiSpy();

    void setCurrentFile(const QString &fileName,const QString &args,const QString &workdir);
    void CreateTargetProcess(const QStringList& infos);
    void updateRecentFileActions();

    void setModuleBeHook(const QString& modname,BOOL beHook);
    void setModuleApiOpt(const QString& modname, const QString& apiname, int base, api_opt_filter *opt);

    BOOL getApiFilterInfo(const QString& modname,const QString& apiname,int base,ModuleApi_Type type,api_opt_filter* opt);


    target_state TargetState() const;
    void saveFilterOptsToFile(const QString& path);

    void sendActionPacket(NPacketBase* opt);
signals:
    void sgsetTargetState(int TargetState);
    void sgMainModuleInit(qint64 modBase,const QString& name,const QString& path);
public slots:
    void setTargetState(int TargetState);
protected:
    //实例化网络通信库远端连接建立通知虚函数
    virtual	BOOL	onInstOpen(nnet_inst inst);
    //实例化网络通信库远端连接关闭通知虚函数
    virtual void	onInstClose(nnet_inst inst);
    //实例化网络通信库远端连接异常通知虚函数
    virtual void	onInstError(nnet_inst inst,int err);
    //实例化网络通信库远端连接数据接收通知虚函数
    virtual void	onInstRead(nnet_inst inst, const char* pdata, size_t pszdata);

    virtual void timerEvent(QTimerEvent *e);
    virtual void closeEvent(QCloseEvent *e);
private slots:
    void on_actionCreatePS_triggered();
    void psOpenRecentFile();
    void updateUI();
    void on_actionContinue_triggered();
    void on_actionPause_triggered();
    void on_actionOptions_triggered();
    void on_actionAppOutput_triggered();
    void on_actionSave_triggered();
    void on_btnClearRecords_clicked();
    void on_spinMaxRecords_valueChanged(int arg1);
    void on_actionStop_triggered();
    void on_actionStep_triggered();
    void on_actionBreak_triggered();
    void on_btnEnterVM_clicked();
    void on_btnSelRF_clicked();
    void fsStartRsrcSpy();
    void on_resTree_clicked(const QModelIndex &index);
    void on_txtResBFType_currentIndexChanged(int index);
    void fsLineEditResChanged(QString txt);
    void on_btnResReBuild_clicked();
    void on_btnSaveRes_clicked();
    void on_btnLoadRes_clicked();
    void on_btnTakeSnap1_clicked();
    void on_btnTakeSnap2_clicked();
    void on_btnDetectReloc_clicked();
    void on_cmbActions_currentIndexChanged(int index);
    void on_btnInjectModule_clicked();
    void on_btnSelModule_clicked();
    void on_btnIMSnap1_clicked();
    void on_btnIMSnap2_clicked();
    void on_btnRebuildReloc_clicked();
    void on_btnSelPatchs_clicked();
    void on_action_About_triggered();
    void on_btnSelSnapFile_clicked();
    void on_btnSnapShot_clicked();
    void onModuleApiRightClick(QPoint pos,QModelIndexList ml);
    void onMainModuleInit(qint64 modBase, const QString& name, const QString& path);
    void on_btnLoadXImport_clicked();
    void on_btnFixImport_clicked();
private:
    //目标进程初始化事件
    void onRemoteInit(nnet_inst inst, NPacketBase *pk);
    //目标进程请求模块过滤
    void onModuleFilter(nnet_inst inst, NPacketBase *pk);
    //目标进程请求函数过滤
    void onApiFilter(nnet_inst inst, NPacketBase *pk);
    //收到目标进程模块载入信息
    void onModuleLoad(nnet_inst inst,NPacketBase *pk);
    //收到目标进程模块信息
    void onModuleApis(nnet_inst inst,NPacketBase *pk);
    //目标进程有函数调用
    void onApiCall(nnet_inst inst,NPacketBase *pk);
    //目标进程请求函数监控设定
    void onTestFlags(nnet_inst inst,NPacketBase *pk);

    void onVMExecFinished(nnet_inst inst,NPacketBase *pk);

    void onOEPCall(nnet_inst inst,NPacketBase *pk);
    void onResourceNode(nnet_inst inst,NPacketBase *pk);

    //收到内存块信息
    void onMemoryBlock(nnet_inst inst,NPacketBase *pk);
    //收到内存数据信息
    void onMemoryData(nnet_inst inst,NPacketBase *pk);
    //收到模块注入结果信息
    void onModuleInjectResult(nnet_inst inst,NPacketBase *pk);

    enum continue_type{
        continue_run = 0,
        continue_step,
        continue_break,
        continue_entervm,
        continue_action,
        continue_takesnapshot
    };

    bool continueTarget(continue_type type, NPacketBase *opt = 0);
    void pauseTarget();

    ULONG_PTR buildMemoryBlock(memoryblocks& bks, QByteArray* data);
    int  compareMemoryBlocks(ULONG_PTR rvaBase,ULONG_PTR addrLeft,QByteArray *dataLeft,ULONG_PTR addrRight,QByteArray *dataRight);


    void flushModuleToUI();

private:
    Ui::xApiSpy *ui;
    QString curFile;
    QAction *separatorAct;
    QAction *separatorAct2;

    enum { MaxRecentFiles = 15 };
    QAction *recentFileActs[MaxRecentFiles];

    ModuleTreeModel m_modelModules;
    ApiTableModel   m_apiModel;
    ModuleItem* m_cmod;
    NAvlList<ULONG_PTR,ModuleApiTreeItem*>  m_apisidx;
    int m_updateTimer;
    QLabel m_labCount;
    QLabel m_labStatus;
    target_state    m_TargetState;
    nnet_inst   m_lastinst;
    QString m_mmpath;
    typedef QMap<QString,module_opt_filter> ftmod_map;
    //过滤相关
    ftmod_map m_ft_modules;
    HANDLE  m_hPS;
    ULONG_PTR m_lastExecAddr;
    snap_state  m_snapstate;

    //for resource
    std::set<ResourceLeaf>  m_resNodes;
    ResourceTreeModel   m_resModel;
    int     m_curResID;
    QString m_curResStr;
    ULONG_PTR   m_rtEntry;
    //QSharedPointer<NByteStream> m_resBin;
    //for patch
    //使用两个列表来储存两个快照
    memoryblocks        m_snap1mem;
    memoryblocks        m_snap2mem;
    memory_block        m_membk;

    PatchTableModel     m_patchModel1;
    PatchTableModel     m_patchModel2;

    QMenu*  m_ApiMenu;

    iatcall_map     m_IATCalls;
    iat_modules     m_IATModules;
    ImportTreeModel m_importModel;
};



#endif // XAPISPY_H
