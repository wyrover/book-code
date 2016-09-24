#include <Windows.h>
#include <tchar.h>

#include "xapispy.h"
#include "ui_xapispy.h"
#include "xprocessstartup.h"
#include "xapihexbuffer.h"

#include "../../xHooklib/xhook_peloader.h"
#include "../../xHooklib/xhook_pefuncs.h"

#include "../../../nCom/nautolargebuffer.h"

#include "../xVMRuntime/xvmruntime.h"
#include "../xVMRuntime/xapihookruntime.h"
#include "../xVMRuntime/xpedumper.h"

#include "aboutdialog.h"

#pragma pack(push,1)
struct reloc_node{
    int         type;
    DWORD       rva;
    ULONGLONG   targetRVA;
};

struct reloc_short
{
    unsigned short	m_value:12;
    unsigned short	m_type:4;

};

#pragma pack(pop)
typedef std::map<DWORD,reloc_node> reloclist;



#define reloc_once_alloc 0x1000

char* rebuidRelocSection(reloclist& relocs,unsigned long* Size)
{
    if (relocs.size() < 1)
        return 0;
    DWORD palign = 0x1000;
    char*	bufSec	=	(char*)malloc(reloc_once_alloc);
    memset((void*)bufSec,0,reloc_once_alloc);
    long	vlsize	=	reloc_once_alloc;
    long	pos		=	0;
    IMAGE_BASE_RELOCATION imBR;
    imBR.VirtualAddress = 0;
    imBR.SizeOfBlock = 0;
    for (reloclist::iterator pitr = relocs.begin();pitr != relocs.end();)
    {
        const reloc_node* reline = &pitr->second;
        if (imBR.VirtualAddress == 0)
            imBR.VirtualAddress = (reline->rva / palign) * palign;

        long	posblock = pos;
        if (pos+sizeof(IMAGE_BASE_RELOCATION) >= vlsize)
        {
            vlsize += reloc_once_alloc;
            bufSec = (char*)realloc((void*)bufSec,vlsize);
        }
        IMAGE_BASE_RELOCATION*	lpBR = (IMAGE_BASE_RELOCATION*)(bufSec+pos);
        *lpBR = imBR;
        pos+=sizeof(IMAGE_BASE_RELOCATION);
        while (pitr != relocs.end())
        {
            reline = &pitr->second;
            if (reline->rva < imBR.VirtualAddress)
            {
                pitr++;
                continue;
            }
            if (imBR.VirtualAddress <= reline->rva && imBR.VirtualAddress+palign > reline->rva)
            {
                if ((pos+sizeof(IMAGE_BASE_RELOCATION)+sizeof(reloc_short)) >= vlsize)
                {
                    vlsize += reloc_once_alloc;
                    bufSec = (char*)realloc((void*)bufSec,vlsize);
                }
                reloc_short*	value = (reloc_short*)(bufSec+pos);
                value->m_type = reline->type;
                value->m_value = reline->rva - imBR.VirtualAddress;
                pos += sizeof(reloc_short);
                pitr++;
            }else
            {
                imBR.VirtualAddress = (reline->rva / palign) * palign;
                break;
            }
        }
        if (pos%4 != 0)
        {
            int paddb = 4-(pos%4);
            memset(bufSec+pos,0,paddb);
            pos +=paddb;
        }
        lpBR = (IMAGE_BASE_RELOCATION*)(bufSec+posblock);
        lpBR->SizeOfBlock = pos - posblock;
    }
    *Size = pos;
    return bufSec;
}



struct  ResourceBuildLeaf;
typedef QMap<ResourceNodeName,ResourceBuildLeaf> ResBuildLeafs;

struct  ResourceBuildLeaf{
    ResourceNodeName    name;
    ResBuildLeafs       leafs;
    DWORD               codePage;
    QByteArray          data;
};
enum RESBUILDRELOC_TYPE{
    RESBUILDRELOC_NAME,
    RESBUILDRELOC_DATA
};
struct ResourceBuildReloc{
    RESBUILDRELOC_TYPE m_type;
    DWORD m_Offset;
};
typedef QList<ResourceBuildReloc>   BuildRelocList;



void ByteArrayAlignTo(QByteArray *pArray, int pAlign)
{
    if (pArray->size() == 0)
        return;
    if (pArray->size() % pAlign)
    {
        QByteArray pfil;
        pfil.fill(0,pAlign - (pArray->size() % pAlign));
        pArray->append(pfil);
    }
}


void WriteLeafToData(QByteArray *dtStub,QByteArray *dtNames,QByteArray *dtData,const ResourceBuildLeaf* rLeaf,BuildRelocList* relList)
{
    IMAGE_RESOURCE_DIRECTORY resDir = {0};
    for (ResBuildLeafs::const_iterator pitr = rLeaf->leafs.begin();pitr != rLeaf->leafs.end();pitr++)
    {
        if (pitr->name.m_NameIsString)
            resDir.NumberOfNamedEntries++;
        else
            resDir.NumberOfIdEntries++;
    }
    dtStub->append((const char*)&resDir,sizeof(IMAGE_RESOURCE_DIRECTORY));
    int entry = dtStub->size();
    int index = 0;
    ResourceBuildReloc reloc;
    dtStub->append(QByteArray().fill(0,sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY)*rLeaf->leafs.size()));
    for (ResBuildLeafs::const_iterator itrname = rLeaf->leafs.begin();itrname != rLeaf->leafs.end();itrname++)
    {
        IMAGE_RESOURCE_DIRECTORY_ENTRY* pePtr = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(dtStub->data() + entry) + index;
        if (itrname->name.m_NameIsString)
        {
            pePtr->NameIsString = itrname->name.m_NameIsString;
            pePtr->NameOffset = dtNames->size();
            WORD wsLen = itrname->name.m_sName.length();
            dtNames->append((const char*)&wsLen,sizeof(wsLen));
            dtNames->append((const char*)itrname->name.m_sName.c_str(),itrname->name.m_sName.size()*sizeof(wchar_t));
            //名称需要重定位
            reloc.m_type = RESBUILDRELOC_NAME;
            reloc.m_Offset = entry + (index * sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));
            relList->push_back(reloc);
        }else
            pePtr->Id = itrname->name.m_id;
        if (itrname->leafs.size() > 0 && itrname->data.size() < 1)
        {
            pePtr->DataIsDirectory = true;
            pePtr->OffsetToDirectory = dtStub->size();
            WriteLeafToData(dtStub,dtNames,dtData,&itrname.value(),relList);
        }
        else
        {
            pePtr->DataIsDirectory = false;
            pePtr->OffsetToData = dtStub->size();
            IMAGE_RESOURCE_DATA_ENTRY dtEntry;
            dtEntry.CodePage = itrname->codePage;
            dtEntry.Reserved = 0;
            ByteArrayAlignTo(dtData,sizeof(DWORD));
            dtEntry.Size = itrname->data.size();

            dtEntry.OffsetToData = dtData->size();
            reloc.m_type = RESBUILDRELOC_DATA;
            reloc.m_Offset = dtStub->size();
            relList->push_back(reloc);
            dtStub->append((const char*)&dtEntry,sizeof(IMAGE_RESOURCE_DATA_ENTRY));
            dtData->append(itrname->data);
        }
        index++;
    }
}



int BuildResourceSection(ResourceNodes* resNodes, DWORD RVA, QByteArray *data)
{
    if (resNodes->size() < 1)
        return -1;
    ResourceBuildLeaf  lpRoot;
    //根据资源数据整理出类似PE文件资源区段目录结构
    for (ResourceNodes::const_iterator itr1 = resNodes->begin();itr1 != resNodes->end();itr1++)
    {
        const ResourceLeaf& leaf = *itr1;
        ResourceBuildLeaf* bleaf = &lpRoot;
        int j=0;
        for (ResourcePath::const_iterator itr2 = leaf.m_ResPath.begin();itr2 != leaf.m_ResPath.end();itr2++,j++)
        {
            const ResourceNodeName& rName = *itr2;
            if ((j == (leaf.m_ResPath.size()-1)) && (leaf.m_data.size() > 0))
            {
                //是最后一层并且是数据资源
                ResourceBuildLeaf blOne;
                blOne.name = rName;
                blOne.codePage = leaf.m_CodePage;
                blOne.data.clear();
                blOne.data.append(&leaf.m_data[0],leaf.m_data.size());
                bleaf->leafs.insert(rName,blOne);
            }else if(!bleaf->leafs.contains(rName))
            {
                ResourceBuildLeaf oleaf;
                oleaf.name = rName;
                bleaf->leafs.insert(rName,oleaf);
            }
            bleaf = &bleaf->leafs[rName];
        }
    }
    //这里根据PE文件资源的样式，我们将资源区段分为3个部分组成
    //第一部分是资源目录数据
    //第二部分是资源名称数据
    //第三部分是资源数据
    QByteArray      dtStub;
    QByteArray      dtName;
    QByteArray      dtRes;
    //用于存放重定位信息
    BuildRelocList  relocList;
    data->clear();
    //将我们上面整理出来的资源数据转换成实际的PE文件资源目录格式，并分类存储到相应区域
    WriteLeafToData(&dtStub,&dtName,&dtRes,&lpRoot,&relocList);
    //对其各部分
    ByteArrayAlignTo(&dtStub,sizeof(DWORD));
    ByteArrayAlignTo(&dtName,sizeof(DWORD));
    //修正资源数据为正确值
    foreach(const ResourceBuildReloc& pReloc,relocList)
    {
        switch(pReloc.m_type)
        {
        case RESBUILDRELOC_NAME:
        {
            IMAGE_RESOURCE_DIRECTORY_ENTRY* pePtr = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(dtStub.data() + pReloc.m_Offset);
            pePtr->NameOffset += dtStub.size();
        }break;
        case RESBUILDRELOC_DATA:
        {
            IMAGE_RESOURCE_DATA_ENTRY* pePtr = (IMAGE_RESOURCE_DATA_ENTRY*)(dtStub.data() + pReloc.m_Offset);
            pePtr->OffsetToData += dtStub.size() + dtName.size() + RVA;
        }break;
        }
    }
    //合并打包各资源部分，重建完毕
    data->append(dtStub);
    data->append(dtName);
    data->append(dtRes);
    ByteArrayAlignTo(data,sizeof(DWORD));

    return 0;
}


#include "../xVMDebug/NativeSysCallID.h"
static syscIds gscIds;
void loadNativeSysApiID()
{
    NativeSysCallID sc;
    TCHAR path[MAX_PATH];
    GetSystemDirectory(path,sizeof(path) / sizeof(TCHAR)-1);
    _tcscat(path,_T("\\NTDLL.DLL"));
    sc.setTargetFile(path);
    gscIds.iNtQueryInformationProcess = sc.getNativeSysCallID("NtQueryInformationProcess");
    gscIds.iNtSetInformationThread = sc.getNativeSysCallID("NtSetInformationThread");
    gscIds.iNtQuerySystemInformation = sc.getNativeSysCallID("NtQuerySystemInformation");
    gscIds.iNtQueryObject = sc.getNativeSysCallID("NtQueryObject");
    gscIds.iNtClose = sc.getNativeSysCallID("NtClose");
    gscIds.iNtSetContextThread = sc.getNativeSysCallID("NtSetContextThread");
    sc.reset();
    gscIds.iFindowWindow = sc.getFindowWindowSysCallID();
}


xApiSpy::xApiSpy(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::xApiSpy)
  ,m_modelModules(this)
  ,m_importModel(this)
  ,m_cmod(0)
  ,m_resModel(this)
  ,m_rtEntry(0)
{
    loadNativeSysApiID();
    ui->setupUi(this);
    separatorAct = ui->fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = ui->fileMenu->addAction("");
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(psOpenRecentFile()));
    }
    separatorAct2 = ui->fileMenu->addSeparator();
    QAction* actExit = ui->fileMenu->addAction(QString::fromUtf8("Exit(&X)"));
    ui->fileMenu->addAction(actExit);
    updateRecentFileActions();

    ui->treeModule->setModel(&m_modelModules);

    ui->treeModule->setColumnWidth(0,180);

    
    ui->tabApiCall->setModel(&m_apiModel);

    ui->tabApiCall->setColumnWidth(0,50);
    ui->tabApiCall->setColumnWidth(1,40);

    ui->tabApiCall->setColumnWidth(2,70);
    ui->tabApiCall->setColumnWidth(3,120);
    ui->tabApiCall->setColumnWidth(4,40);

    QList<int> sizes;
    sizes << 200;
    sizes << 400;
    ui->sp2->setSizes(sizes);


    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_CURSOR),(int)RT_CURSOR);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_BITMAP),(int)RT_BITMAP);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_ICON),(int)RT_ICON);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_MENU),(int)RT_MENU);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_DIALOG),(int)RT_DIALOG);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_STRING),(int)RT_STRING);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_FONTDIR),(int)RT_FONTDIR);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_FONT),(int)RT_FONT);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_ACCELERATOR),(int)RT_ACCELERATOR);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_RCDATA),(int)RT_RCDATA);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_MESSAGETABLE),(int)RT_MESSAGETABLE);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_GROUP_CURSOR),(int)RT_GROUP_CURSOR);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_GROUP_ICON),(int)RT_GROUP_ICON);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_VERSION),(int)RT_VERSION);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_DLGINCLUDE),(int)RT_DLGINCLUDE);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_PLUGPLAY),(int)RT_PLUGPLAY);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_VXD),(int)RT_VXD);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_ANICURSOR),(int)RT_ANICURSOR);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_ANIICON),(int)RT_ANIICON);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_HTML),(int)RT_HTML);
    ui->txtResBFType->addItem(ResourceTreeModel::decodeResType((int)RT_MANIFEST),(int)RT_MANIFEST);

    connect(ui->txtResBFType->lineEdit(),SIGNAL(textEdited(QString)),this,SLOT(fsLineEditResChanged(QString)));

    connect(ui->btnStartRsrcSpy,SIGNAL(clicked()),this,SLOT(fsStartRsrcSpy()));

    connect(this,SIGNAL(sgsetTargetState(int)),this,SLOT(setTargetState(int)),Qt::BlockingQueuedConnection);
    connect(this,SIGNAL(sgMainModuleInit(qint64,QString,QString)),this,SLOT(onMainModuleInit(qint64,QString,QString)),Qt::BlockingQueuedConnection);

    ui->dockOutput->setWindowTitle(trUtf8("Application Output"));
    ui->dockOutput->setMinimumHeight(0);
    ui->txtOutput->resize(100,100);
    ui->dockOutput->resize(100,100);

    ui->tabSnap1->setColumnWidth(0,80);
    ui->tabSnap1->setColumnWidth(1,60);
    ui->tabSnap1->setColumnWidth(2,60);

    ui->tabSnap2->setColumnWidth(0,80);
    ui->tabSnap2->setColumnWidth(1,60);
    ui->tabSnap2->setColumnWidth(2,60);

    m_labCount.setText(trUtf8("Api Calls:0/0"));
    ui->statusBar->addPermanentWidget(&m_labCount);
    setTargetState(target_stoped);
    ui->statusBar->addWidget(&m_labStatus);

    //ui->wdtAction->setEnabled(false);

    ui->treeImport->setModel(&m_importModel);
    ui->treeImport->setColumnWidth(0,300);
    ui->treeImport->setColumnWidth(1,80);

    ui->dockOutput->setMaximumHeight(100);
    ui->dockOutput->updateGeometry();
    ui->dockOutput->setMaximumHeight(QWIDGETSIZE_MAX);

    ui->resTree->setModel(&m_resModel);

    ui->resHex->setRowWidth(10);

    sizes.clear();
    sizes << 180;
    sizes << 400;
    ui->spRes->setSizes(sizes);

    ui->tabPatch1->setModel(&m_patchModel1);
    ui->tabPatch1->setColumnWidth(0,80);
    ui->tabPatch1->setColumnWidth(1,40);
    ui->tabPatch1->setColumnWidth(2,70);
    ui->tabPatch1->setColumnWidth(3,60);
    ui->tabPatch2->setModel(&m_patchModel2);
    ui->tabPatch2->setColumnWidth(0,80);
    ui->tabPatch2->setColumnWidth(1,40);
    ui->tabPatch2->setColumnWidth(2,70);
    ui->tabPatch2->setColumnWidth(3,50);


    m_ApiMenu = new QMenu(this);
    QAction* act = m_ApiMenu->addAction("Pause On These API");
    act->setData(action_setbreak);

    act = m_ApiMenu->addAction("Clear Breakpoint");
    act->setData(action_delbreak);

    act = m_ApiMenu->addAction("Check");
    act->setData(action_check);
    act = m_ApiMenu->addAction("UnCheck");
    act->setData(action_uncheck);

    connect(ui->treeModule,SIGNAL(onRightClick(QPoint,QModelIndexList)),this,SLOT(onModuleApiRightClick(QPoint,QModelIndexList)));
    //初始化网络通信库
    nnStartup(1);
    //监听在命名管道XVMRUNTIME_COM_PIPENAME_FULL
    //XVMRUNTIME_COM_PIPENAME_FULL在通信协议定义头文件中定义
    ListenOnPipe(_T(XVMRUNTIME_COM_PIPENAME_FULL));

    m_updateTimer = QObject::startTimer(1000);
}


void xApiSpy::setCurrentFile(const QString &fileName, const QString &args, const QString &workdir)
{
    curFile = QString("%1|%2|%3").arg(QString::fromLatin1(fileName.toUtf8().toHex()))
            .arg(QString::fromLatin1(args.toUtf8().toHex()))
            .arg(QString::fromLatin1(workdir.toUtf8().toHex()));

    setWindowFilePath(curFile);

    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    files.removeAll(curFile);
    files.prepend(curFile);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);
    updateRecentFileActions();
}

void xApiSpy::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();


    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = files[i];
        QStringList infos = text.split("|");
        for (QStringList::iterator itr = infos.begin();itr != infos.end();itr++)
        {
            QString st = QString::fromUtf8(QByteArray::fromHex(itr->toLatin1()));
            *itr = st;
        }
        recentFileActs[i]->setText(QString("%1 %2 ->%3").arg(infos[0]).arg(infos[1]).arg(infos[2]));
        recentFileActs[i]->setData(infos);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
    separatorAct2->setVisible(numRecentFiles > 0);
}

void xApiSpy::setModuleBeHook(const QString &modname, BOOL beHook)
{
    m_ft_modules[modname.trimmed().toLower()].bHook = beHook;
}

void xApiSpy::setModuleApiOpt(const QString &modname, const QString &apiname, int base, api_opt_filter *opt)
{
    QString hashName = QString("[%1]").arg(base,4,16,QChar('0')).toUpper()+apiname;
    m_ft_modules[modname.trimmed().toLower()].apis[hashName] = *opt;
}

BOOL xApiSpy::onInstOpen(nnet_inst inst)
{
    fnSetInstOptions(inst,0,0,0,0,MSG_PACKET_SIZE_MAXIMUM,0,MSG_PACKET_SIZE_MAXIMUM);
    return TRUE;
}

void xApiSpy::onInstClose(nnet_inst inst)
{

}

void xApiSpy::onInstError(nnet_inst inst, int err)
{

}
//在网络库的数据接收函数当中我们分发具体的事件到相应的函数上
void xApiSpy::onInstRead(nnet_inst inst, const char *pdata, size_t pszdata)
{
    NPacketBase pk(pdata,pszdata);
    switch(pk.getMsgID())
    {
    case xvm_req_init: onRemoteInit(inst,&pk);break;
    case xvm_req_modulefilter:onModuleFilter(inst,&pk);break;
    case xvm_req_apifilter:onApiFilter(inst,&pk);break;
    case xvm_req_modapis:onModuleLoad(inst,&pk);break;
    case xvm_req_modapinodes:onModuleApis(inst,&pk);break;
    case xvm_req_apicall:onApiCall(inst,&pk);break;
    case xvm_req_testflags:onTestFlags(inst,&pk);break;
    case xvm_req_vmexecfinished:onVMExecFinished(inst,&pk);break;
    case xvm_req_resnode:onResourceNode(inst,&pk);break;
    case xvm_req_memblock:onMemoryBlock(inst,&pk);break;
    case xvm_req_memdata:onMemoryData(inst,&pk);break;
    case xvm_ack_injectmodule:onModuleInjectResult(inst,&pk);break;
    case xvm_req_oepcall:onOEPCall(inst,&pk);break;
    }
}

void xApiSpy::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_updateTimer)
    {
        updateUI();
    }
}

void xApiSpy::closeEvent(QCloseEvent *e)
{
    QByteArray st = saveState();
    QSettings settings;
    settings.setValue("winpos",st);
    return QMainWindow::closeEvent(e);
}

xApiSpy::~xApiSpy()
{
    killTimer(m_updateTimer);
    delete ui;
}

void xApiSpy::on_actionCreatePS_triggered()
{
    xProcessStartup xps;
    if (xps.exec() != QDialog::Accepted)
        return;
    QString path;
    QString args;
    QString workdir;
    xps.getInfo(path,args,workdir);
    setCurrentFile(path,args,workdir);
    QStringList infos;
    infos.append(path);
    infos.append(args);
    infos.append(workdir);
    CreateTargetProcess(infos);
}

void xApiSpy::psOpenRecentFile()
{
    QAction* act = qobject_cast<QAction*>(sender());
    QStringList infos = act->data().toStringList();
    setCurrentFile(infos[0],infos[1],infos[2]);
    CreateTargetProcess(infos);
}

void xApiSpy::updateUI()
{
    ui->treeModule->update();
    //ui->treeModule->scrollToBottom();
    m_labCount.setText(trUtf8("Api Calls:%1/%2").arg(m_apiModel.currentCount()).arg(m_apiModel.uniqID()-1));
    if (WaitForSingleObject(m_hPS,0) != WAIT_TIMEOUT)
    {
        m_hPS = 0;
        if (TargetState() != target_stoped)
            setTargetState(target_stoped);
    }
}

void xApiSpy::onRemoteInit(nnet_inst inst,NPacketBase *pk)
{
    //设定最后通信的实例，实际上在我们的函数监视器当中，始终只有一个实例
    m_lastinst = inst;
    //设定目标进程状态，由于我们的网络通信库并没有使用与GUI相同的线程来接收数据
    //因此我们在这里需要通过Qt的信号机制发送目标进程状态,以达到线程同步的目的
    DWORD psID = pk->getInt(xvm_vid_processID,0);
    if (m_hPS == 0)
        m_hPS = OpenProcess(MAXIMUM_ALLOWED,FALSE,psID);
    sgsetTargetState(target_startup);


    //取出各种参数并整理传递到onMainModuleInit函数
    const wchar_t* modname = pk->getWString(xvm_vid_modname);
    const wchar_t* modpath = pk->getWString(xvm_vid_modpath);
    ULONG_PTR modbase = pk->getLongLong(xvm_vid_modulebase);
    QString name = QString::fromWCharArray(modname).trimmed();
    QString path = QString::fromWCharArray(modpath).trimmed();
    m_rtEntry = (ULONG_PTR)pk->getLongLong(xvm_vid_rtentry);
    sgMainModuleInit(modbase,name,path);

    //onModuleLoad(inst,pk);
}

void xApiSpy::onModuleFilter(nnet_inst inst,NPacketBase *pk)
{
    const wchar_t* modname = pk->getWString(xvm_vid_modname);
    const wchar_t* modpath = pk->getWString(xvm_vid_modpath);
    int state = -1;

    QString mdname = QString::fromWCharArray(modname);
    //查找模块信息
    ModuleItem* mod = m_modelModules.findModule(mdname);
    if (mod)
    {
        //如果找到模块信息，那么根据我们的配置设定，决定是否HOOK该模块导出表
        state = mod->isHooked()?0:-1;
    }
    NPacketBase pkret(xvm_ack_modulefilter);
    pkret.putInt(xvm_vid_state,state);
    fpInstWrite(inst,pkret.data(),pkret.size());
}
//函数过滤事件处理
void xApiSpy::onApiFilter(nnet_inst inst, NPacketBase *pk)
{
    //默认情况下我们不监视函数，因此state设置为0,设置为>0表示监视
    int state = 0;
    NPacketBase pkret(xvm_ack_apifilter);
    //如果该函数已经被初步筛选为数据函数或者中转函数，那么我们不处理
    if (!pk->contains(xvm_vid_skiped))
    {
        //到这里表示函数是一个代码函数
        ULONG_PTR modbase = pk->getLongLong(xvm_vid_modulebase);
        ModuleItem* mod = m_modelModules.findModule(modbase);
        if (mod)
        {
            //如果找到模块信息，那么根据我们的配置设定，决定是否HOOK该函数
            long funcRVA = pk->getInt(xvm_vid_orgRVA,0);
            ModuleApiTreeItem* apitem = mod->findApi(funcRVA);
            if (apitem)
            {
                state = apitem->isHooked()?(XAPI_FLAG_HOOKEXPORT|XAPI_FLAG_USEVEXPORT):0;
                pkret.putInt(xvm_vid_flags,apitem->apiFlags());
            }
        }
    }
    //将过滤结果反馈给xVMRuntime模块

    pkret.putInt(xvm_vid_state,state);
    fpInstWrite(inst,pkret.data(),pkret.size());
}

void xApiSpy::onModuleLoad(nnet_inst inst, NPacketBase *pk)
{
    const wchar_t* modname = pk->getWString(xvm_vid_modname);
    const wchar_t* modpath = pk->getWString(xvm_vid_modpath);
    qint64 modbase = pk->getLongLong(xvm_vid_modulebase);
    ModuleItem* mod = m_modelModules.findModule(modbase);
    if (mod)
        return;
    if (m_cmod)
    {
        delete m_cmod;
        m_cmod = 0;
    }
    m_cmod = new ModuleItem(&m_modelModules);
    m_cmod->setModbase(modbase);
    QString mdname = QString::fromWCharArray(modname);
    m_cmod->setModname(mdname);
    QString mdpath = QString::fromWCharArray(modpath);
    m_cmod->setModpath(mdpath);
    ftmod_map::iterator itr = m_ft_modules.find(mdname.trimmed().toLower());
    if (itr != m_ft_modules.end())
    {
        m_cmod->setIsHooked(itr.value().bHook);
    }else
        m_cmod->setIsHooked(ui->chkDefMH->isChecked());
}

void xApiSpy::onModuleApis(nnet_inst inst, NPacketBase *pk)
{
    qint64 modbase = pk->getLongLong(xvm_vid_modulebase);
    if (!m_cmod)
        return;
    if (m_cmod->modbase() != modbase)
        return;

    const char* lpdt = pk->tofirst();
    while(lpdt)
    {
        if (pk->vid() == xvm_vid_modapinode)
        {
            NPacketBase pknode(lpdt,pk->szblock(),false);
            ModuleApi_Type tp = ModuleApi_CODE;
            const char* lpredirName = 0;
            if (pknode.contains(xvm_vid_dataapi))
                tp = ModuleApi_DATA;
            else if (pknode.contains(xvm_vid_redirectapi))
            {
                tp = ModuleApi_REDIR;
                lpredirName = pknode.getString(xvm_vid_redirname);
            }
            m_cmod->insertApi(tp,pknode.getInt(xvm_vid_orgRVA,0),pknode.getInt(xvm_vid_apibase,0),
                              pknode.getString(xvm_vid_apiname,0),lpredirName);
        }
        lpdt = pk->tonext();
    }

    if (pk->contains(xvm_vid_modapifinished))
    {
        foreach(ModuleTreeItem* item,m_cmod->childItems)
        {
            ModuleApiTreeItem* api = static_cast<ModuleApiTreeItem*>(item);
            if (api)
            {
                ULONG_PTR apiKey = api->modbase()+api->RVA();
                ModuleApiTreeItem** itemptr = m_apisidx.new_back(apiKey);
                *itemptr = api;
            }
        }

        m_modelModules.appendItem(m_cmod);
        m_cmod = 0;
    }

}

void xApiSpy::onApiCall(nnet_inst inst, NPacketBase *pk)
{
    ULONG_PTR hmod = pk->getLongLong(xvm_vid_modulebase);
    DWORD rva = pk->getInt(xvm_vid_orgRVA);
    if (hmod && rva)
    {
        ULONG_PTR retaddr = pk->getLongLong(xvm_vid_retaddr);
        DWORD refc = pk->getInt(xvm_vid_count);
        ULONG_PTR apiKey = hmod+rva;
        DWORD threadID = pk->getInt(xvm_vid_threadid);
        //从索引中查到函数信息
        ModuleApiTreeItem** itemptr = m_apisidx.find(apiKey);
        if (itemptr)
        {
            ModuleApiTreeItem* item = *itemptr;
            item->setRefc(refc);
            //记录函数调用
            m_apiModel.pushCallLog(apiKey,retaddr,threadID,item);
        }
    }
    //处理额外工作
    if (m_TargetState == target_request_pause || pk->contains(xvm_vid_break))
    {
        m_lastExecAddr = pk->getLongLong(xvm_vid_address);
        sgsetTargetState(target_paused);
    }else
    {
        NPacketBase pkret(xvm_ack_apicall);
        fpInstWrite(inst,pkret.data(),pkret.size());
    }
}

void xApiSpy::onTestFlags(nnet_inst inst, NPacketBase *pk)
{
    int state = 0;
    long flags = pk->getInt(xvm_vid_flags);
    if (flags & XAPI_FLAG_HOOKEXPORT)
        state = ui->chkHookExport->isChecked()?1:0;
    else if (flags & XAPI_FLAG_USEVEXPORT)
        state = ui->chkUseVExport->isChecked()?1:0;
    NPacketBase pkret(xvm_ack_testflags);
    pkret.putInt(xvm_vid_state,state);
    fpInstWrite(inst,pkret.data(),pkret.size());
}

void xApiSpy::onVMExecFinished(nnet_inst inst, NPacketBase *pk)
{
    m_lastExecAddr = pk->getLongLong(xvm_vid_address);
    sgsetTargetState(target_paused);
}

void xApiSpy::onOEPCall(nnet_inst inst, NPacketBase *pk)
{
    m_lastExecAddr = pk->getLongLong(xvm_vid_address);
    sgsetTargetState(target_paused);
}

void xApiSpy::onMainModuleInit(qint64 modBase, const QString &name, const QString &path)
{
    QString pathDB = path;
    pathDB.append(".xas");
    NAutoLargeBufferA nla;
    nla.load((const TCHAR*)pathDB.utf16());

    m_ft_modules.clear();
    m_apiModel.clear();
    ui->treeModule->reset();
    m_modelModules.clear();
    m_apisidx.clear();

    m_mmpath = pathDB;

    const char* lpdata = 0;
    int szdata = 0;
    lpdata = nla.data(szdata);
    if (!lpdata)
        return;

    NPacketBase pkopts(lpdata,szdata,false);
    const char* lpnode = pkopts.tofirst();
    while(lpnode)
    {
        switch(pkopts.vid())
        {
        case xvm_opt_vid_hookmodule:
        {
            NPacketBase pknode(pkopts.block(),pkopts.szblock(),false);
            QString sname = QString::fromWCharArray(pknode.getWString(xvm_vid_modname));
            if (!sname.isEmpty())
            {
                sname = sname.trimmed().toLower();
                module_opt_filter optf;
                optf.bHook = pknode.getInt(xvm_vid_behook);

                const char* lpapi = pknode.tofirst();
                while(lpapi)
                {
                    switch(pknode.vid())
                    {
                    case xvm_vid_apiopt:
                    {
                        NPacketBase pkapi(pknode.block(),pknode.szblock(),false);
                        QString sapiname = QString::fromWCharArray(pkapi.getWString(xvm_vid_apinamehash));
                        if (!sapiname.isEmpty())
                        {
                            api_opt_filter optapi;
                            optapi.bHook = pkapi.getInt(xvm_vid_behook);
                            optapi.flags = pkapi.getInt(xvm_vid_flags);
                            optf.apis[sapiname] = optapi;
                        }
                    }break;
                    }

                    lpapi = pknode.tonext();
                }
                m_ft_modules[sname] = optf;
            }

        }break;
        }
        lpnode = pkopts.tonext();
    }

}

void xApiSpy::onResourceNode(nnet_inst inst, NPacketBase *pk)
{
    ResourceLeaf leaf;
    leaf.m_CodePage = pk->getInt(xvm_vid_resCP);
    char* lpdata = 0;
    int szdata = 0;
    if (pk->getBinary(xvm_vid_resData,lpdata,szdata))
    {
        leaf.m_data.resize(szdata);
        memcpy(&leaf.m_data[0],lpdata,szdata);
    }
    char* lpnode = pk->tofirst();
    while(lpnode)
    {
        if (pk->vid() == xvm_vid_respath)
        {
            ResourceNodeName rn;
            NPacketBase pkpath(lpnode,pk->szblock(),false);
            if (pkpath.contains(xvm_vid_resid))
            {
                rn.m_NameIsString = false;
                rn.m_id = pkpath.getInt(xvm_vid_resid);
            }else if (pkpath.contains(xvm_vid_resname))
            {
                rn.m_NameIsString = true;
                rn.m_sName = pkpath.getWString(xvm_vid_resname);
            }
            rn.selected = pkpath.contains(xvm_vid_selected);
            leaf.m_ResPath.push_back(rn);
        }
        lpnode = pk->tonext();
    }
    m_resModel.insertResItem(leaf);
    //m_resNodes
}

void xApiSpy::onMemoryBlock(nnet_inst inst, NPacketBase *pk)
{
    m_membk.addr = pk->getLongLong(xvm_vid_mgstart);
    m_membk.size = pk->getLongLong(xvm_vid_mgend);
    m_membk.state = pk->getInt(xvm_vid_memstate);
    m_membk.protect = pk->getInt(xvm_vid_memprotect);
    m_membk.name = QString::fromLatin1(pk->getString(xvm_vid_name));

    switch(m_snapstate)
    {
    case snap_snap1:
    {
        int nwor = ui->tabSnap1->rowCount();
        ui->tabSnap1->insertRow(nwor);
        m_snap1mem.push_back(m_membk);
        ui->tabSnap1->setItem(nwor,0,new QTableWidgetItem(m_membk.name));
        ui->tabSnap1->setItem(nwor,1,new QTableWidgetItem(QString("%1").arg(m_membk.addr,8,16,QChar('0'))));
        ui->tabSnap1->setItem(nwor,2,new QTableWidgetItem(QString("%1").arg(m_membk.size,8,16,QChar('0'))));

    }break;
    case snap_snap2:
    {
        int nwor = ui->tabSnap2->rowCount();
        ui->tabSnap2->insertRow(nwor);
        m_snap2mem.push_back(m_membk);
        ui->tabSnap2->setItem(nwor,0,new QTableWidgetItem(m_membk.name));
        ui->tabSnap2->setItem(nwor,1,new QTableWidgetItem(QString("%1").arg(m_membk.addr,8,16,QChar('0'))));
        ui->tabSnap2->setItem(nwor,2,new QTableWidgetItem(QString("%1").arg(m_membk.size,8,16,QChar('0'))));
    }break;
    }
}

void xApiSpy::onMemoryData(nnet_inst inst, NPacketBase *pk)
{
    if (pk->contains(xvm_vid_begin))
        m_membk.clear();

    //ULONG_PTR addr = pk->getLongLong(xvm_vid_address);
    int size = pk->getInt(xvm_vid_size);
    char* lpdata = 0;
    int szdata = 0;
    if (pk->getBinary(xvm_vid_binary,lpdata,szdata) && szdata == size)
    {
        m_membk.data.append(lpdata,szdata);
    }
}

void xApiSpy::onModuleInjectResult(nnet_inst inst, NPacketBase *pk)
{
    qint64 modb = pk->getLongLong(xvm_vid_modulebase);
    ui->edtIMBase->setText(QString("%1").arg(modb,8,16,QChar('0')));
}

BOOL xApiSpy::getApiFilterInfo(const QString &modname, const QString &apiname, int base, ModuleApi_Type type, api_opt_filter *opt)
{
    opt->bHook = FALSE;
    opt->flags = 0;
    if (type != ModuleApi_CODE)
    {
        return FALSE;
    }
    ftmod_map::iterator itr = m_ft_modules.find(modname.trimmed().toLower());
    if (itr == m_ft_modules.end())
    {
        opt->bHook = ui->chkDefMH->isChecked();
        return TRUE;
    }
    QString hashName = QString("[%1]").arg(base,4,16,QChar('0')).toUpper()+apiname;
    ftapi_map::iterator apitr = itr.value().apis.find(hashName);
    if (apitr == itr.value().apis.end())
    {
        opt->bHook = ui->chkDefMH->isChecked();
        return TRUE;
    }
    *opt = apitr.value();
    return TRUE;
}

void xApiSpy::saveFilterOptsToFile(const QString &path)
{
    NPacketBase pkopts;
    for (ftmod_map::iterator itr = m_ft_modules.begin();itr != m_ft_modules.end();itr++)
    {
        module_opt_filter* mf = &itr.value();
        NPacketBase pknode;
        pknode.putWString(xvm_vid_modname,(const wchar_t*)itr.key().utf16());
        pknode.putInt(xvm_vid_behook,mf->bHook);

        for (ftapi_map::iterator apitr = mf->apis.begin();apitr != mf->apis.end();apitr++)
        {
            api_opt_filter* af = &apitr.value();
            NPacketBase pkapi;
            pkapi.putWString(xvm_vid_apinamehash,(const wchar_t*)apitr.key().utf16());
            pkapi.putInt(xvm_vid_behook,af->bHook);
            pkapi.putInt(xvm_vid_flags,af->flags);
            pknode.putBinary(xvm_vid_apiopt,pkapi.data(),pkapi.size());
        }


        pkopts.putBinary(xvm_opt_vid_hookmodule,pknode.data(),pknode.size());
    }

    QFile pf(path);
    pf.open(QFile::ReadWrite);
    pf.resize(0);
    pf.write(pkopts.data(),pkopts.size());
    pf.close();
}

void xApiSpy::sendActionPacket(NPacketBase *opt)
{
    if (!opt)
        return;
    if (TargetState() == target_running
            || (TargetState() != target_running && ui->radioRM->isChecked()))
    {
        LPVOID lpv = VirtualAllocEx(m_hPS,0,opt->size(),MEM_COMMIT,PAGE_READWRITE);
        if (lpv)
        {
            DWORD dwted = 0;
            if (WriteProcessMemory(m_hPS,lpv,opt->data(),opt->size(),&dwted) && dwted == opt->size())
                CreateRemoteThread(m_hPS,0,0,(LPTHREAD_START_ROUTINE)m_rtEntry,lpv,0,0);
        }
    }else
        continueTarget(continue_action,opt);
}

bool xApiSpy::continueTarget(continue_type type, NPacketBase* opt)
{
    target_state oldstate = TargetState();
    if (oldstate == target_running || oldstate == target_stoped)
        return false;
    NPacketBase pkret;
    switch(type)
    {
    case continue_run:
    {
        setTargetState(target_running);
        pkret.setMsgID(xvm_ack_continue);
    }break;//continue
    case continue_step:
    {
        setTargetState(target_request_pause);
        pkret.setMsgID(xvm_ack_continue);
    }break;//step
    case continue_action:
    {
        //setTargetState(target_request_pause);
        pkret.setMsgID(xvm_ack_doaction);
    }break;
    case continue_break://break
    {
        setTargetState(target_request_pause);
        if (HIBYTE(GetKeyState(VK_SHIFT)))
        {
            pkret.setMsgID(xvm_ack_loopbreak);
        }else
        {
            pkret.setMsgID(xvm_ack_break);
        }
    }break;
    case continue_entervm:
    {
        setTargetState(target_running);
        pkret.setMsgID(xvm_ack_entervm);
    }break;
    case continue_takesnapshot:
    {
        setTargetState(target_running);
        pkret.setMsgID(xvm_ack_takesnapshot);
    }break;
    default:
    {
        setTargetState(target_running);
        pkret.setMsgID(xvm_ack_continue);
    }break;
    }

    switch(oldstate)
    {
    case target_startup:
    {
        pkret.setempty(false);
        pkret.setMsgID(xvm_ack_init);
        NPacketBase pkopts;
        pkopts.putInt(xvm_vid_flags,XAPI_INIT_FLAG_DEFAULT);
        if (ui->chkVirKrl32->isChecked())
            pkret.putEmpty(xvm_vid_virtualkernel32);
        pkopts.putInt(xvm_vid_hookexport,ui->chkHookExport->isChecked()?1:0);
        pkopts.putInt(xvm_vid_usevexport,ui->chkUseVExport->isChecked()?1:0);
        pkopts.putInt(xvm_vid_usenullstub,ui->chkUseNullStub->isChecked()?1:0);
        pkret.putBinary(xvm_vid_moduleHook,pkopts.data(),pkopts.size());
        if (ui->chkXVMDebug->isChecked())
            pkret.putEmpty(xvm_vid_xvmdebug);
        if (ui->chkVMExecEnc->isChecked())
            pkret.putEmpty(xvm_vid_vmexecinit);
        if (ui->chkAntiAntiDebug->isChecked())
            pkret.putEmpty(xvm_vid_enhanceAntiAntiDBG);
        if (ui->chkDisSafeSEH->isChecked())
            pkret.putEmpty(xvm_vid_disableSafeSEH);
        if (opt)
        {
            setTargetState(target_paused);
            pkret.putBinary(xvm_vid_continueopt,opt->data(),opt->size());
        }
    }break;
    default:
    {
    }break;
    }
    if (opt)
        pkret.putBinary(xvm_vid_continueopt,opt->data(),opt->size());
    return fpInstWrite(m_lastinst,pkret.data(),pkret.size());
}

void xApiSpy::pauseTarget()
{
    sgsetTargetState(target_request_pause);
}

ULONG_PTR xApiSpy::buildMemoryBlock(xApiSpy::memoryblocks &bks,QByteArray *data)
{
    data->clear();
    if (bks.size() < 1)
        return 0;

    ULONG_PTR addrLow = bks.front().addr;
    ULONG_PTR addrHig = bks.front().addr+bks.front().size;
    foreach(const memory_block& mb,bks)
    {
        addrLow = qMin<ULONG_PTR>(addrLow,mb.addr);
        addrHig = qMax<ULONG_PTR>(addrHig,mb.addr+mb.size);
    }

    DWORD size = addrHig - addrLow;
    data->resize(size);
    char* lpdata = data->data();
    if (!lpdata)
        return 0;
    memset(lpdata,0,size);
    foreach(const memory_block& mb,bks)
        memcpy(lpdata+mb.addr-addrLow,mb.data.data(),min(mb.data.size(),mb.size));
    return addrLow;
}
//对比两个内存块数据
int xApiSpy::compareMemoryBlocks(ULONG_PTR rvaBase,ULONG_PTR addrLeft, QByteArray *dataLeft,
                                 ULONG_PTR addrRight, QByteArray *dataRight)
{
    DWORD szComp = min(dataLeft->size(),dataRight->size());
    //首先我们计算两个映像的基址差
    DWORD delta = ui->edtRelocDelta->text().toLongLong(0,16);
    if (delta == 0)
        delta =  addrRight - addrLeft;
    BYTE* lpbin1 = (BYTE*)dataLeft->data();
    BYTE* lpbin2 = (BYTE*)dataRight->data();

    PatchItemVector items;
    for (int j=0;j<szComp;j++)
    {
        if (lpbin1[j] != lpbin2[j])
        {
            //我们回溯3个字节的空间，因为重定位信息由时候并不会造成
            //地址数据低16位数据的变化
            for (int k=j-3;k<j;k++)
            {
                DWORD dw1 = *(DWORD*)(lpbin1+k);
                DWORD dw2 = *(DWORD*)(lpbin2+k);
                if (dw2 - dw1 == delta)
                {
                    DWORD rva = addrLeft+k - rvaBase;
                    PatchItem itm;
                    itm.rva = rva;
                    itm.size = sizeof(DWORD);
                    itm.type = PatchItem_Reloc;
                    itm.state = PatchItem_State_Enable;
                    itm.flags = 0;
                    items.push_back(itm);
                    j=k+sizeof(DWORD);
                    continue;
                }
            }
        }
    }
    //将数据保存到结果集当中
    m_patchModel1.insertRecords(items);
    return 0;
}

void xApiSpy::flushModuleToUI()
{
    for (iat_modules::iterator itr = m_IATModules.begin();itr != m_IATModules.end();itr++)
    {
        iat_module* im = &itr->second;
        ImportModuleItem* item = m_importModel.pushModule(im->nameHash,QString::fromStdWString(im->name));
        if (item)
        {
            for (iat_modfuncs::iterator itrfc = im->funcs.begin();itrfc != im->funcs.end();itrfc++)
            {
                iat_func* iatf =  &itrfc->second;
                item->insertApi(iatf->nameHash,iatf->IATRVA,QString::fromStdWString(iatf->name));
            }
        }
    }
}

xApiSpy::target_state xApiSpy::TargetState() const
{
    return m_TargetState;
}

void xApiSpy::setTargetState(int TargetState)
{
    m_TargetState = (target_state)TargetState;

    QString str = trUtf8("status:");
    switch(m_TargetState)
    {
    case target_running:
    {
        //ui->wdtAction->setEnabled(false);
        ui->actionContinue->setEnabled(false);
        ui->actionPause->setEnabled(true);
        ui->actionBreak->setEnabled(false);
        ui->actionStep->setEnabled(false);
        ui->actionStop->setEnabled(true);
        str.append(trUtf8("running"));
    }break;
    case target_startup:
    case target_paused:
    {
        ui->actionContinue->setEnabled(true);
        ui->actionPause->setEnabled(false);
        ui->actionBreak->setEnabled(true);
        ui->actionStep->setEnabled(true);
        ui->actionStop->setEnabled(true);
        //ui->wdtAction->setEnabled(true);
        ui->tabApiCall->scrollToBottom();
        str.append(trUtf8("paused"));
    }break;
    default:
    {
        ui->actionContinue->setEnabled(false);
        ui->actionPause->setEnabled(false);
        //ui->wdtAction->setEnabled(false);
        ui->actionStep->setEnabled(false);
        ui->actionBreak->setEnabled(false);
        ui->actionStop->setEnabled(false);
        str.append(trUtf8("stoped"));
    }break;
    }

    m_labStatus.setText(str);
}


void xApiSpy::CreateTargetProcess(const QStringList &infos)
{
    NAutoLargeBufferA nla;
    nla.load(_T("xVMRuntime32.dll"));

    int szdata = 0;
    const char* lpdata = nla.data(szdata);

    STARTUPINFO	stInfo;
    PROCESS_INFORMATION	psInfo;

    memset(&stInfo,0,sizeof(stInfo));
    stInfo.cb = sizeof(STARTUPINFO);
    memset(&psInfo,0,sizeof(psInfo));

    xvmrutime_args args = {0};
    //传递启用堆接管参数
    args.bEnableHeapHook = ui->chkPSSnapshot->isChecked();
    args.bBreakAtOEP = ui->chkBreakOnOEP->isChecked();
    args.bVirtualDebugMode = FALSE;
    args.bSandbox = ui->chkSandBox->isChecked();
    args.bAntiAntiDebug = ui->chkAntiAntiDebug->isChecked();
    args.bDisableSafeSEH = ui->chkDisSafeSEH->isChecked();
    args.bEnableClientPipe = TRUE;
    args.bEnableVirtualKernel32 = ui->chkVirKrl32->isChecked();
    args.sysc = gscIds;

    m_hPS = INVALID_HANDLE_VALUE;
    if (CreateProcessW((LPCWSTR)infos[0].data()
                       ,(LPWSTR)infos[1].data()
                       ,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,
                       (LPCWSTR)infos[2].data(),&stInfo,&psInfo))
    {
        m_hPS = StartInProcess(psInfo.hProcess,psInfo.hThread,
                               psInfo.dwThreadId,lpdata,szdata,(const char*)&args,sizeof(args),
                               XHOOK_PELOADER_FLAG_DIRECTEXEC
                               //接管LdrInitializeThunk
                               | XHOOK_PELOADER_FLAG_LDRINITTHUNKCALL
                               //接管LdrLoadDll
                               | XHOOK_PELOADER_FLAG_LDRLOADDLLCALL
                               //接管OEP
                               | XHOOK_PELOADER_FLAG_OEPCALL
                               //抢占模块内存
                               | XHOOK_PELOADER_FLAG_AFTERMAINMODULE
                               );
    }
    if (m_hPS == INVALID_HANDLE_VALUE)
    {
        return;
    }
    setWindowTitle(QString("xApiSpy(%1)").arg(infos[0]));

}

void xApiSpy::on_actionContinue_triggered()
{
    continueTarget(continue_run);
}

void xApiSpy::on_actionPause_triggered()
{
    pauseTarget();
}

void xApiSpy::on_actionOptions_triggered()
{
    ui->frmOptions->setVisible(!ui->frmOptions->isVisible());
}

void xApiSpy::on_actionAppOutput_triggered()
{
    ui->dockOutput->setVisible(!ui->dockOutput->isVisible());
}

void xApiSpy::on_actionSave_triggered()
{
    saveFilterOptsToFile(m_mmpath);
}

void xApiSpy::on_btnClearRecords_clicked()
{
    m_apiModel.clear();
}



void xApiSpy::on_spinMaxRecords_valueChanged(int arg1)
{
    m_apiModel.setMaxRecords(arg1);
}

void xApiSpy::on_actionStop_triggered()
{
    TerminateProcess(m_hPS,-1);
    setTargetState(target_stoped);
}

void xApiSpy::on_actionStep_triggered()
{
    continueTarget(continue_step);
}

void xApiSpy::on_actionBreak_triggered()
{
    continueTarget(continue_break);
}

void xApiSpy::on_btnEnterVM_clicked()
{
    NPacketBase opt;
    opt.putInt64(xvm_vid_address,m_lastExecAddr);
    opt.putWString(xvm_vid_path,(const wchar_t*)ui->edtRcFile->text().utf16());
    if (ui->chkStopAddr->isChecked())
        opt.putInt64(xvm_vid_endaddr,ui->edtEndAddr->text().toLongLong(0,16));
    if (ui->chkMaxRecords->isChecked())
        opt.putEmpty(xvm_vid_vmrecord);
    if (ui->chkVMPrint->isChecked())
        opt.putEmpty(xvm_vid_vmprint);
    if (ui->chkStopNR->isChecked())
        opt.putInt(xvm_vid_stoprecordcount,ui->spinNR->value());
    if (ui->chkBypassSys->isChecked())
        opt.putEmpty(xvm_vid_bypasssys);
    opt.putWString(xvm_vid_rcfile,(const wchar_t*)QDir::toNativeSeparators(ui->edtRcFile->text()).utf16());
    opt.putInt(xvm_vid_maxrc,ui->spinMaxRC->value());

    continueTarget(continue_entervm,&opt);
}

void xApiSpy::on_btnSelRF_clicked()
{
    QString sPath = QFileDialog::getSaveFileName(this);
    if (sPath.isEmpty())
        return;
    ui->edtRcFile->setText(QDir::toNativeSeparators(sPath));
}

void xApiSpy::fsStartRsrcSpy()
{
    NPacketBase opt(xvm_ack_spyrsrc);

    opt.putInt64(xvm_vid_modulebase,ui->spinRsrcModBase->text().toLongLong(0,16));
    if (ui->chkMemSection->isChecked())
        opt.putEmpty(xvm_vid_getresfrommemsec);
    if (ui->chkMoniterApi->isChecked())
        opt.putEmpty(xvm_vid_getresbyhookapi);
    if (ui->chkBruteforceRes->isChecked())
    {
        opt.putEmpty(xvm_vid_getresbybf);

        if (m_curResID !=  -1)
            opt.putInt(xvm_vid_bfrestypeid,m_curResID);
        else
            opt.putWString(xvm_vid_bfrestypestr,(const wchar_t*)m_curResStr.utf16());

        if (ui->chkStartMemRG->isChecked())
        {
            opt.putEmpty(xvm_vid_searchmemstr);
            opt.putInt64(xvm_vid_mgstart,ui->edtMGS->text().toLongLong(0,16));
            opt.putInt64(xvm_vid_mgend,ui->edtMGE->text().toLongLong(0,16));
        }
        QStringList strs = ui->txtResStrs->document()->toPlainText().split(QChar::CarriageReturn,QString::SkipEmptyParts);
        for (QStringList::iterator itr = strs.begin();itr != strs.end();itr++)
        {
            itr->trimmed();
            opt.putWString(xvm_vid_bfstr,(const wchar_t*)itr->utf16());
        }

        if (ui->chkAllIDType->isChecked())
            opt.putEmpty(xvm_vid_searchallidtype);


    }
    sendActionPacket(&opt);
}

void xApiSpy::on_resTree_clicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    ResTreeItem* item = static_cast<ResTreeItem*>(index.internalPointer());
    if (!item)
        return;
    if (item->leaf.m_data.size() < 1)
        return;
    QSharedPointer<xApiHexBuffer> buf(new xApiHexBuffer(&item->leaf.m_data[0],item->leaf.m_data.size()));
    ui->resHex->setData(buf);
}



void xApiSpy::on_txtResBFType_currentIndexChanged(int index)
{
    m_curResID = ui->txtResBFType->currentData().toInt();

}

void xApiSpy::fsLineEditResChanged(QString txt)
{
    m_curResID = -1;
    BOOL isID = TRUE;
    if (txt.startsWith("##"))
        isID = FALSE;
    else if (txt.compare("#") == 0)
        isID = FALSE;
    else if (!txt.startsWith("#"))
        isID = FALSE;
    if (isID)
    {
        txt.remove(0,1);
        m_curResID = txt.toInt();
    }else
        m_curResStr =txt;
}

void xApiSpy::on_btnResReBuild_clicked()
{
    QString sPath = QFileDialog::getOpenFileName(this);
    if (sPath.isEmpty())
        return;
    QFile fpe(sPath);
    if (!fpe.open(QFile::ReadWrite))
        return;
    uchar* lpdata = fpe.map(0,fpe.size());
    if (!lpdata)
        return;
    if (ui->chkBKSrc->isChecked())
    {
        QString bkfile = fpe.fileName().append(".secrebuild.bak");
        QFile::remove(bkfile);
        if (!fpe.copy(bkfile))
            return;
    }
    PIMAGE_SECTION_HEADER secptr = EnterImageFirstSection((const char*)lpdata);
    PIMAGE_NT_HEADERS imnh = EnterImageNtHeader((const char*)lpdata);
    int nsec = imnh->FileHeader.NumberOfSections;
    secptr += nsec-1;
    DWORD rvaLast = AdjustRva(secptr->VirtualAddress + secptr->Misc.VirtualSize,imnh->OptionalHeader.SectionAlignment);
    DWORD offsetLast = AdjustRaw(secptr->PointerToRawData+secptr->SizeOfRawData,imnh->OptionalHeader.FileAlignment);

    ResourceNodes res;
    QByteArray secdata;
    //将我们收集到的资源数据转换为ResourceNodes列表
    m_resModel.saveToResourceNodes(&res);
    //重建区段数据，rvaLast为区段的RVA
    BuildResourceSection(&res,rvaLast,&secdata);
    if (secdata.size() > 0)
    {
        //如果重建成功，向PE文件添加一个区段并修正区段数据
        DWORD szRaw = AdjustRaw(secdata.size(),imnh->OptionalHeader.FileAlignment);
        imnh->FileHeader.NumberOfSections++;
        secptr++;
        memset(secptr,0,sizeof(IMAGE_SECTION_HEADER));
        secptr->Characteristics = 0x40000040;
        strcpy((char*)secptr->Name,".rsrc");
        secptr->VirtualAddress = rvaLast;
        secptr->Misc.VirtualSize = AdjustRva(szRaw,imnh->OptionalHeader.SectionAlignment);
        secptr->SizeOfRawData = szRaw;
        secptr->PointerToRawData = offsetLast;
        //修正映像大小
        imnh->OptionalHeader.SizeOfImage = rvaLast + secptr->Misc.VirtualSize;
        //修正资源目录地址
        imnh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress = secptr->VirtualAddress;
        imnh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size = secptr->SizeOfRawData;
        fpe.unmap(lpdata);
        fpe.close();
        if (fpe.open(QFile::ReadWrite))
        {
            //写入实际区段数据
            fpe.resize(offsetLast);
            fpe.seek(offsetLast);
            fpe.write(secdata.data(),secdata.size());
            fpe.resize(offsetLast+szRaw);
            fpe.flush();
            fpe.close();
        }

    }

}



void xApiSpy::on_btnSaveRes_clicked()
{
    QString sPath = QFileDialog::getSaveFileName(this);
    if (sPath.isEmpty())
        return;

    ResourceNodes res;
    m_resModel.saveToResourceNodes(&res,TRUE);
    NPacketBase pksave;
    for(ResourceNodes::iterator itr1 = res.begin();itr1 != res.end();itr1++)
    {
        ResourceLeaf* leaf = &*itr1;
        NPacketBase pkleaf;
        //按顺序将资源路径封包
        for (ResourcePath::iterator itr = leaf->m_ResPath.begin();itr != leaf->m_ResPath.end();itr++)
        {
            NPacketBase pkpath;
            ResourceNodeName* rn = &*itr;
            if (!rn->m_NameIsString)
                pkpath.putInt(xvm_vid_resid,rn->m_id);
            else
                pkpath.putWString(xvm_vid_resname,rn->m_sName.c_str());
            if (rn->selected)
                pkpath.putEmpty(xvm_vid_selected);

            pkleaf.putBinary(xvm_vid_respath,pkpath.data(),pkpath.size());
        }
        //封包其它数据
        pkleaf.putInt(xvm_vid_resCP,leaf->m_CodePage);
        pkleaf.putBinary(xvm_vid_resData,&leaf->m_data[0],leaf->m_data.size());
        pksave.putBinary(xvm_vid_node,pkleaf.data(),pkleaf.size());
    }

    QFile fp(sPath);
    if (fp.open(QFile::ReadWrite))
    {
        fp.resize(0);
        fp.write(pksave.data(),pksave.size());
        fp.close();
    }

}

void xApiSpy::on_btnLoadRes_clicked()
{
    QString sPath = QFileDialog::getOpenFileName(this);
    if (sPath.isEmpty())
        return;
    QFile fpe(sPath);
    if (!fpe.open(QFile::ReadWrite))
        return;
    uchar* lpdata = fpe.map(0,fpe.size());
    if (!lpdata)
        return;
    NPacketBase pkload((const char*)lpdata,fpe.size(),false);
    char* lpnode = pkload.tofirst();
    while(lpnode)
    {
        if (pkload.vid() == xvm_vid_node)
        {
            NPacketBase pknode(pkload.block(),pkload.szblock(),false);
            onResourceNode(0,&pknode);
        }
        lpnode = pkload.tonext();
    }
}

void xApiSpy::on_btnTakeSnap1_clicked()
{
    m_snapstate  = snap_snap1;
    while(ui->tabSnap1->rowCount() > 0)
        ui->tabSnap1->removeRow(0);
    m_snap1mem.clear();
    NPacketBase pk(xvm_ack_takememsnapshot);
    pk.putInt64(xvm_vid_mgstart,ui->edtSnap1Addr1->text().toLongLong(0,16));
    pk.putInt64(xvm_vid_mgend,ui->edtSnap1Addr2->text().toLongLong(0,16));
    sendActionPacket(&pk);
}

void xApiSpy::on_btnTakeSnap2_clicked()
{
    m_snapstate  = snap_snap2;
    while(ui->tabSnap2->rowCount() > 0)
        ui->tabSnap2->removeRow(0);
    m_snap2mem.clear();
    NPacketBase pk(xvm_ack_takememsnapshot);
    pk.putInt64(xvm_vid_mgstart,ui->edtSnap2Addr1->text().toLongLong(0,16));
    pk.putInt64(xvm_vid_mgend,ui->edtSnap2Addr2->text().toLongLong(0,16));
    sendActionPacket(&pk);
}

void xApiSpy::on_btnDetectReloc_clicked()
{
    if (m_snap1mem.size() < 1 || m_snap2mem.size() < 1)
        return;
    QByteArray data1;
    QByteArray data2;
    ULONG_PTR rvaBase = ui->edtRelocBase->text().toLongLong(0,16);
    //如果我们没有指定对比的基址依据，那么我们自动使用第一份快照的第一个内存块地址
    if (rvaBase == 0)
        rvaBase = m_snap1mem.front().addr;
    //我们将收集到的内存快照数据分别映射到两块连续的内存空间当中
    //注意：我们在抓取内存数据的时候可以是不连续的内存块
    ULONG_PTR addr1 = buildMemoryBlock(m_snap1mem,&data1);
    ULONG_PTR addr2 = buildMemoryBlock(m_snap2mem,&data2);
    if (addr1 == 0 || addr2 == 0)
        return;
    //进入真正的内存数据对比环节
    compareMemoryBlocks(rvaBase,addr1,&data1,addr2,&data2);
}

void xApiSpy::on_cmbActions_currentIndexChanged(int index)
{
    ui->stkActions->setCurrentIndex(index);
}

void xApiSpy::on_btnInjectModule_clicked()
{
    NPacketBase pk(xvm_ack_injectmodule);
    pk.putWString(xvm_vid_path,(const wchar_t*)ui->edtModulePath->text().utf16());
    sendActionPacket(&pk);
}

void xApiSpy::on_btnSelModule_clicked()
{
    QString sPath = QFileDialog::getOpenFileName(this,trUtf8("Select Module"),QString(),trUtf8("Dll Files (*.dll);;All Files (*.*)"));
    if (sPath.isEmpty())
        return;
    ui->edtModulePath->setText(QDir::toNativeSeparators(sPath));
}

void xApiSpy::on_btnIMSnap1_clicked()
{
    ui->edtSnap1Addr1->setText(ui->edtIMBase->text());
    ui->tabOpts->setCurrentIndex(5);
    ui->btnTakeSnap1->click();
}

void xApiSpy::on_btnIMSnap2_clicked()
{
    ui->edtSnap2Addr1->setText(ui->edtIMBase->text());
    ui->tabOpts->setCurrentIndex(5);
    ui->btnTakeSnap2->click();
}

void xApiSpy::on_btnRebuildReloc_clicked()
{

    QString sPath = QFileDialog::getOpenFileName(this);
    if (sPath.isEmpty())
        return;
    QFile fpe(sPath);
    if (!fpe.open(QFile::ReadWrite))
        return;
    uchar* lpdata = fpe.map(0,fpe.size());
    if (!lpdata)
        return;
    if (ui->chkBKSrcReloc->isChecked())
    {
        QString bkfile = fpe.fileName().append(".relrebuild.bak");
        QFile::remove(bkfile);
        if (!fpe.copy(bkfile))
            return;
    }
    PIMAGE_SECTION_HEADER secptr = EnterImageFirstSection((const char*)lpdata);
    PIMAGE_NT_HEADERS imnh = EnterImageNtHeader((const char*)lpdata);
    int nsec = imnh->FileHeader.NumberOfSections;
    secptr += nsec-1;
    DWORD rvaLast = AdjustRva(secptr->VirtualAddress + secptr->Misc.VirtualSize,imnh->OptionalHeader.SectionAlignment);
    DWORD offsetLast = AdjustRaw(secptr->PointerToRawData+secptr->SizeOfRawData,imnh->OptionalHeader.FileAlignment);
    //我们整理收集到的重定位信息到我们重建函数使用的格式
    reloclist relocs;
    PatchItemList* items = m_patchModel2.items();
    foreach(const PatchItem& item,*items)
    {
        reloc_node node;
        node.rva = item.rva;
        node.targetRVA = 0;
        node.type = IMAGE_REL_BASED_HIGHLOW;
        relocs.insert(std::make_pair(node.rva,node));
    }

    unsigned long szreloc = 0;
    char* lpreloc = rebuidRelocSection(relocs,&szreloc);
    if (lpreloc)
    {
        //如果重建成功，向PE文件添加一个区段并修正区段数据
        DWORD szRaw = AdjustRaw(szreloc,imnh->OptionalHeader.FileAlignment);
        imnh->FileHeader.NumberOfSections++;
        secptr++;
        memset(secptr,0,sizeof(IMAGE_SECTION_HEADER));
        secptr->Characteristics = 0x42000040;
        strcpy((char*)secptr->Name,".reloc");
        secptr->VirtualAddress = rvaLast;
        secptr->Misc.VirtualSize = AdjustRva(szRaw,imnh->OptionalHeader.SectionAlignment);
        secptr->SizeOfRawData = szRaw;
        secptr->PointerToRawData = offsetLast;
        //修正映像大小
        imnh->OptionalHeader.SizeOfImage = rvaLast + secptr->Misc.VirtualSize;
        //修正资源目录地址
        imnh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = secptr->VirtualAddress;
        imnh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = secptr->SizeOfRawData;
        fpe.unmap(lpdata);
        fpe.close();
        if (fpe.open(QFile::ReadWrite))
        {
            //写入实际区段数据
            fpe.resize(offsetLast);
            fpe.seek(offsetLast);
            fpe.write(lpreloc,szreloc);
            fpe.resize(offsetLast+szRaw);
            fpe.flush();
            fpe.close();
        }
        free(lpreloc);
    }
}

void xApiSpy::on_btnSelPatchs_clicked()
{
    QModelIndexList ilst1 = ui->tabPatch1->selectionModel()->selectedRows();
    PatchItemVector items;
    foreach(const QModelIndex& idx,ilst1)
    {
        PatchItem* item = m_patchModel1.item(idx.row());
        if (item)
        {
            PatchItem it = *item;
            items.push_back(it);
        }
    }
    m_patchModel2.insertRecords(items);
}

void xApiSpy::on_action_About_triggered()
{
    AboutDialog ab(this);
    ab.exec();
}

void xApiSpy::on_btnSelSnapFile_clicked()
{
    QString sPath = QFileDialog::getSaveFileName(this);
    if (sPath.isEmpty())
        return;
    ui->edtSnapFile->setText(QDir::toNativeSeparators(sPath));
}

void xApiSpy::on_btnSnapShot_clicked()
{
    NPacketBase opt;
    opt.putWString(xvm_vid_path,(const wchar_t*)ui->edtSnapFile->text().utf16());
    if (ui->chkFullMem->isChecked())
        opt.putEmpty(xvm_vid_fullpremem);

    continueTarget(continue_takesnapshot,&opt);
}

void xApiSpy::onModuleApiRightClick(QPoint pos,QModelIndexList ml)
{
    QAction* act = m_ApiMenu->exec(pos);
    if (!act)
        return;

    QList<ModuleTreeItem*> items;
    foreach(const QModelIndex& idx,ml)
    {
        if (idx.column() == 0 && idx.internalPointer())
        {
            ModuleTreeItem* item = static_cast<ModuleTreeItem*>(idx.internalPointer());
            items.push_back(item);
        }
    }
    if (items.size() < 1)
        return;
    switch(act->data().toInt())
    {
    case action_setbreak:
    {
        int nac = 0;
        NPacketBase pk(xvm_ack_setapibp);
        foreach(ModuleTreeItem* item,items)
        {
            if (item->itemtype() == TreeItem_Api)
            {
                ModuleApiTreeItem* iapi = static_cast<ModuleApiTreeItem*>(item);
                iapi->setApiFlags(xvm_api_flag_breakpoint);
                setModuleApiOpt(iapi->modname(),iapi->apiname(),iapi->apibase(),iapi->opt());

                pk.putInt64(xvm_vid_orgRVA,iapi->apiEntry());
                nac++;
            }
        }
        if (nac > 0)
            sendActionPacket(&pk);
    }break;
    case action_delbreak:
    {
        int nac = 0;
        NPacketBase pk(xvm_ack_delapibp);
        foreach(ModuleTreeItem* item,items)
        {
            if (item->itemtype() == TreeItem_Api)
            {
                ModuleApiTreeItem* iapi = static_cast<ModuleApiTreeItem*>(item);
                iapi->setApiFlags(xvm_api_flag_breakpoint,false);
                setModuleApiOpt(iapi->modname(),iapi->apiname(),iapi->apibase(),iapi->opt());
                pk.putInt64(xvm_vid_orgRVA,iapi->apiEntry());
                nac++;
            }
        }
        if (nac > 0)
            sendActionPacket(&pk);
    }break;
    case action_check:
    {
        foreach(ModuleTreeItem* item,items)
        {
            if (item->itemtype() == TreeItem_Api)
            {
                ModuleApiTreeItem* iapi = static_cast<ModuleApiTreeItem*>(item);
                iapi->setIsHooked(TRUE);
                setModuleApiOpt(iapi->modname(),iapi->apiname(),iapi->apibase(),iapi->opt());
            }else if (item->itemtype() == TreeItem_Module)
            {
                ModuleItem* mitem = static_cast<ModuleItem*>(item);
                mitem->setIsHooked(TRUE);
                setModuleBeHook(mitem->modname(),TRUE);
            }
        }
    }break;
    case action_uncheck:
    {
        foreach(ModuleTreeItem* item,items)
        {
            if (item->itemtype() == TreeItem_Api)
            {
                ModuleApiTreeItem* iapi = static_cast<ModuleApiTreeItem*>(item);
                iapi->setIsHooked(FALSE);
                setModuleApiOpt(iapi->modname(),iapi->apiname(),iapi->apibase(),iapi->opt());
            }else if (item->itemtype() == TreeItem_Module)
            {
                ModuleItem* mitem = static_cast<ModuleItem*>(item);
                mitem->setIsHooked(FALSE);
                setModuleBeHook(mitem->modname(),FALSE);
            }
        }
    }break;
    }

    foreach(const QModelIndex& idx,ml)
    {
        ui->treeModule->update(idx);
    }
}



void xApiSpy::on_btnLoadXImport_clicked()
{
    QString sPath = QFileDialog::getOpenFileName(this,trUtf8("Select XImport File"),QString(),trUtf8("Ximport Files (*.ximport);;All Files (*.*)"));
    if (sPath.isEmpty())
        return;
    NAutoLargeBufferA   nla;
    nla.load((const wchar_t*)sPath.utf16());
    const char* lpdata = 0;
    int szdata = 0;
    lpdata = nla.data(szdata);
    if (!lpdata || szdata < 1)
        return;
    m_IATModules.clear();
    m_IATCalls.clear();
    NPacketBase pk(lpdata,szdata,false);
    char* lpnode = pk.tofirst();
    while(lpnode)
    {
        switch(pk.vid())
        {
        case iatcall_vid_module:
        {
            NPacketBase pkmods(pk.block(),pk.szblock(),false);
            char*lpmod = pkmods.tofirst();
            while(lpmod)
            {
                NPacketBase pkmodone(lpmod,pkmods.szblock(),false);

                iat_module imod;
                imod.nameHash = pkmodone.getInt(iatcall_vid_hash);
                imod.name = pkmodone.getWString(iatcall_vid_name);
                iat_modules::iterator itrmod = m_IATModules.insert(std::make_pair(imod.nameHash,imod)).first;
                iat_module* imodptr = &itrmod->second;
                char* lpnode = pkmodone.tofirst();
                while(lpnode)
                {
                    switch(pkmodone.vid())
                    {
                    case iatcall_vid_node:
                    {
                        NPacketBase pknode(lpnode,pkmodone.szblock(),false);
                        iat_func func;
                        func.nameHash = pknode.getInt(iatcall_vid_hash);
                        func.name = pknode.getWString(iatcall_vid_name);
                        func.IATRVA = pknode.getInt(iatcall_vid_iatrva);
                        iat_modfuncs::iterator ifc = imodptr->funcs.insert(std::make_pair(func.nameHash,func)).first;
                        ifc->second.module =imodptr;
                    }break;
                    }
                    lpnode = pkmodone.tonext();
                }

                lpmod = pkmods.tonext();
            }
        }break;
        }
        lpnode = pk.tonext();
    }

    lpnode = pk.tofirst();
    while(lpnode)
    {
        switch(pk.vid())
        {
        case iatcall_vid_record:
        {
            NPacketBase pkcall(lpnode,pk.szblock(),false);
            iatcall_node info;
            memset(&info,0,sizeof(info));


            info.cmdaddr = pkcall.getLongLong(iatcall_vid_cmdaddr);
            info.callto = pkcall.getLongLong(iatcall_vid_callto);
            info.ophead = pkcall.getByte(iatcall_vid_ophead);
            char* lpval = 0;
            int szval = 0;
            if (pkcall.getBinary(iatcall_vid_opcode,lpval,szval))
            {
                info.szop = min(szval,sizeof(info.opcode));
                memcpy(info.opcode,lpval,info.szop);
            }
            info.calltype = (IATCall_Type)pkcall.getInt(iatcall_vid_calltype);
            info.memoft = pkcall.getInt(iatcall_vid_memoft);
            info.retaddr = pkcall.getLongLong(iatcall_vid_retaddr);
            info.hasHead = pkcall.getInt(iatcall_vid_hashead);
            info.rtflags = pkcall.getInt(iatcall_vid_rtflags);
            info.modhash = pkcall.getInt(iatcall_vid_modhash);
            info.funchash = pkcall.getInt(iatcall_vid_apihash);
            m_IATCalls[info.cmdaddr] = info;

        }break;
        }
        lpnode = pk.tonext();
    }
    flushModuleToUI();
}

void xApiSpy::on_btnFixImport_clicked()
{
    QString sPath = QFileDialog::getOpenFileName(this);
    if (sPath.isEmpty())
        return;
    QFile fpe(sPath);
    if (!fpe.open(QFile::ReadWrite))
        return;
    uchar* lpdata = fpe.map(0,fpe.size());
    if (!lpdata)
        return;
    if (ui->chkBKSrcIAT->isChecked())
    {
        QString bkfile = fpe.fileName().append(".IAT.bak");
        QFile::remove(bkfile);
        if (!fpe.copy(bkfile))
            return;
    }
    PIMAGE_SECTION_HEADER secptr = EnterImageFirstSection((const char*)lpdata);
    PIMAGE_NT_HEADERS imnh = EnterImageNtHeader((const char*)lpdata);
    int nsec = imnh->FileHeader.NumberOfSections;
    secptr += nsec-1;
    DWORD rvaLast = AdjustRva(secptr->VirtualAddress + secptr->Misc.VirtualSize,imnh->OptionalHeader.SectionAlignment);
    DWORD offsetLast = AdjustRaw(secptr->PointerToRawData+secptr->SizeOfRawData,imnh->OptionalHeader.FileAlignment);
    //我们整理收集所需需要重建的导入表数据
    ImportSymbols imps;
    m_importModel.saveToImportSymbols(&imps,TRUE);
    QByteArray secData;
    BuildImportSection(imps,rvaLast,&secData,TRUE);
    if (secData.size() > 0)
    {
        //如果重建成功，向PE文件添加一个区段并修正区段数据
        DWORD szRaw = AdjustRaw(secData.size(),imnh->OptionalHeader.FileAlignment);
        imnh->FileHeader.NumberOfSections++;
        secptr++;
        memset(secptr,0,sizeof(IMAGE_SECTION_HEADER));
        secptr->Characteristics = 0xC0000040;
        strcpy((char*)secptr->Name,".Import");
        secptr->VirtualAddress = rvaLast;
        secptr->Misc.VirtualSize = AdjustRva(szRaw,imnh->OptionalHeader.SectionAlignment);
        secptr->SizeOfRawData = szRaw;
        secptr->PointerToRawData = offsetLast;
        //修正映像大小
        imnh->OptionalHeader.SizeOfImage = rvaLast + secptr->Misc.VirtualSize;
        //修正资源目录地址
        imnh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = secptr->VirtualAddress;
        imnh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = secptr->SizeOfRawData;
        fpe.unmap(lpdata);
        fpe.close();
        if (fpe.open(QFile::ReadWrite))
        {
            //写入实际区段数据
            fpe.resize(offsetLast);
            fpe.seek(offsetLast);
            fpe.write(secData);
            fpe.resize(offsetLast+szRaw);
            fpe.flush();
            fpe.close();
        }
    }
}



//我们定义4个参数，其中包括我们预计将导入表存放的RVA，最后一个参数表示重建平台32/64位
int BuildImportSection(ImportSymbols& impSyms,DWORD buildRVA,QByteArray* data,BOOL is32Bit)
{
    //为了达到标准，我们将导入表信息分为4个部分
    QByteArray      dtStub; //导入描述表部分
    QByteArray      dtData; //导入表数据部分
    QByteArray      dtOriThunk; //OriginalFirstThunk 部分
    QByteArray      dtFstThunk; //FirstThunk 部分
    ImportBuildTP   vRels;
    QString sDllName;
    IMAGE_IMPORT_DESCRIPTOR impDesr;
    ImpBuildTPVal TPVal;
    //遍历所有导入信息
    for (ImportSymbols::const_iterator itr = impSyms.begin();itr != impSyms.end();)
    {
        if (sDllName != itr.key())
        {
            //如果到达一个新DLL模块，我们使用了multimap特性来实现自然排序
            sDllName = itr.key();
            //初始化一个有效的导入描述
            impDesr.TimeDateStamp = 0;
            impDesr.ForwarderChain = 0;
            impDesr.Characteristics = 0;
            impDesr.OriginalFirstThunk = 0;
            impDesr.FirstThunk = 0;
            impDesr.Name = dtData.size();
            dtData.append(sDllName);
            dtData.append("\0",1);
            //将需要最后计算地址的地方记录下来
            TPVal.offset = dtStub.size() + offsetof(IMAGE_IMPORT_DESCRIPTOR,Name);
            vRels.insert(RELOC_IMP_DATA,TPVal);
            impDesr.OriginalFirstThunk = dtOriThunk.size();
            TPVal.offset = dtStub.size() + offsetof(IMAGE_IMPORT_DESCRIPTOR,OriginalFirstThunk);
            vRels.insert(RELOC_IMP_ORITHUNKPTR,TPVal);
            impDesr.FirstThunk = dtFstThunk.size();
            TPVal.offset = dtStub.size() + offsetof(IMAGE_IMPORT_DESCRIPTOR,FirstThunk);
            vRels.insert(RELOC_IMP_FSTTHUNKPTR,TPVal);
            dtStub.append((const char*)&impDesr,sizeof(impDesr));
            while(itr != impSyms.end() && sDllName == itr.key())
            {
                //具体写入函数数据
                if (is32Bit)
                {
                    IMAGE_THUNK_DATA32 Thunk32;
                    if (itr.value().IsOrdinal)
                    {
                        Thunk32.u1.Ordinal = itr.value().Ordinal | IMAGE_ORDINAL_FLAG32;
                        dtOriThunk.append((const char*)&Thunk32,sizeof(Thunk32));
                        dtFstThunk.append((const char*)&Thunk32,sizeof(Thunk32));
                    }
                    else
                    {
                        Thunk32.u1.AddressOfData = dtData.size();
                        dtData.append((const char*)&itr.value().Hint,sizeof(WORD));
                        dtData.append(itr.value().FuncName);
                        dtData.append("\0",1);

                        TPVal.offset = dtOriThunk.size();
                        vRels.insert(RELOC_IMP_ORITHUNK32,TPVal);
                        dtOriThunk.append((const char*)&Thunk32,sizeof(Thunk32));

                        TPVal.offset = dtFstThunk.size();
                        vRels.insert(RELOC_IMP_FSTTHUNK32,TPVal);
                        dtFstThunk.append((const char*)&Thunk32,sizeof(Thunk32));

                    }
                }else
                {
                    IMAGE_THUNK_DATA64 Thunk64;
                    if (itr.value().IsOrdinal)
                    {
                        Thunk64.u1.Ordinal = itr.value().Ordinal | IMAGE_ORDINAL_FLAG64;
                        dtOriThunk.append((const char*)&Thunk64,sizeof(Thunk64));
                        dtFstThunk.append((const char*)&Thunk64,sizeof(Thunk64));
                    }
                    else
                    {
                        Thunk64.u1.AddressOfData = dtData.size();
                        dtData.append((const char*)&itr.value().Hint,sizeof(WORD));
                        dtData.append(itr.value().FuncName);
                        dtData.append("\0",1);

                        TPVal.offset = dtOriThunk.size();
                        vRels.insert(RELOC_IMP_ORITHUNK64,TPVal);
                        dtOriThunk.append((const char*)&Thunk64,sizeof(Thunk64));

                        TPVal.offset = dtFstThunk.size();
                        vRels.insert(RELOC_IMP_FSTTHUNK64,TPVal);
                        dtFstThunk.append((const char*)&Thunk64,sizeof(Thunk64));

                    }
                }
                itr ++;
                if (sDllName != itr.key())
                {
                    if (is32Bit)
                    {
                        IMAGE_THUNK_DATA32 pThunk32 = {0};
                        dtOriThunk.append((const char*)&pThunk32,sizeof(pThunk32));
                        dtFstThunk.append((const char*)&pThunk32,sizeof(pThunk32));

                    }else
                    {
                        IMAGE_THUNK_DATA64 pThunk64 = {0};
                        dtOriThunk.append((const char*)&pThunk64,sizeof(pThunk64));
                        dtFstThunk.append((const char*)&pThunk64,sizeof(pThunk64));
                    }
                }
            }
        }
    }

    memset(&impDesr,0,sizeof(impDesr));
    dtStub.append((const char*)&impDesr,sizeof(impDesr));

    //根据记录计算修正正确的地址
    for(ImportBuildTP::const_iterator itr = vRels.begin();itr != vRels.end();itr++)
    {
        switch(itr.key())
        {
        case RELOC_IMP_DATA:
        {
            *(DWORD*)(dtStub.data() + itr.value().offset) += buildRVA + dtStub.size()+dtOriThunk.size()+dtFstThunk.size();
        }break;
        case RELOC_IMP_ORITHUNK32:
        {
            IMAGE_THUNK_DATA32* ThunkPtr = (IMAGE_THUNK_DATA32*)(dtOriThunk.data() + itr.value().offset);
            ThunkPtr->u1.AddressOfData += buildRVA + dtStub.size()+dtOriThunk.size()+dtFstThunk.size();
        }break;
        case RELOC_IMP_FSTTHUNK32:
        {
            IMAGE_THUNK_DATA32* pThunkPtr = (IMAGE_THUNK_DATA32*)(dtFstThunk.data() + itr.value().offset);
            pThunkPtr->u1.AddressOfData += buildRVA + dtStub.size()+dtOriThunk.size()+dtFstThunk.size();
        }break;
        case RELOC_IMP_ORITHUNK64:
        {
            IMAGE_THUNK_DATA64* pThunkPtr = (IMAGE_THUNK_DATA64*)(dtOriThunk.data() + itr.value().offset);
            pThunkPtr->u1.AddressOfData += buildRVA + dtStub.size()+dtOriThunk.size()+dtFstThunk.size();
        }break;
        case RELOC_IMP_FSTTHUNK64:
        {
            IMAGE_THUNK_DATA64* pThunkPtr = (IMAGE_THUNK_DATA64*)(dtFstThunk.data() + itr.value().offset);
            pThunkPtr->u1.AddressOfData += buildRVA + dtStub.size()+dtOriThunk.size()+dtFstThunk.size();
        }break;
        case RELOC_IMP_ORITHUNKPTR:
        {
            *(DWORD*)(dtStub.data() + itr.value().offset) += buildRVA + dtStub.size();
        }break;
        case RELOC_IMP_FSTTHUNKPTR:
        {
            *(DWORD*)(dtStub.data() + itr.value().offset) += buildRVA + dtStub.size()+dtOriThunk.size();
        }break;
        }
    }
    //合并打包数据
    data->append(dtStub);
    data->append(dtOriThunk);
    data->append(dtFstThunk);
    data->append(dtData);
    //按双字对其一下
    if ((data->size() % sizeof(DWORD)) != 0)
    {
        DWORD pc = 0;
        data->append((const char*)&pc,sizeof(DWORD)-(data->size() % sizeof(DWORD)));
    }
    return data->size();
}
