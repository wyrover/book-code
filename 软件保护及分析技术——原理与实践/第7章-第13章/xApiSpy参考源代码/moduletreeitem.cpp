#include <QStringList>
#include "xapispy.h"
#include "moduletreeitem.h"
#include "moduletreemodel.h"
//! [0]
ModuleTreeItem::ModuleTreeItem(ModuleTreeItem *parent)
{
    parentitem = parent;
}
//! [0]

//! [1]
ModuleTreeItem::~ModuleTreeItem()
{
    clear();
}

void ModuleTreeItem::appendChild(ModuleTreeItem *child)
{
    childItems.append(child);
}

ModuleTreeItem *ModuleTreeItem::parent() const
{
    return parentitem;
}
//! [1]


//! [3]
ModuleTreeItem *ModuleTreeItem::child(int row)
{
    if (row >= childItems.count())
        return 0;
    return childItems.value(row);
}
//! [3]

//! [4]
int ModuleTreeItem::childCount() const
{
    return childItems.count();
}
//! [4]



//! [6]
QVariant ModuleTreeItem::data(int column)
{
    return QVariant();
}

int ModuleTreeItem::row() const
{
    if (parentitem)
        return parentitem->childItems.indexOf(const_cast<ModuleTreeItem*>(this));
    return -1;
}

bool ModuleTreeItem::removeChild(ModuleTreeItem *child)
{
    return childItems.removeOne(child);
}

void ModuleTreeItem::clear()
{
    foreach(ModuleTreeItem* item,childItems)
        delete item;
    childItems.clear();
}

Qt::ItemFlags ModuleTreeItem::flags()
{
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

TreeItemType ModuleTreeItem::itemtype() const
{
    return TreeItem_None;
}



ModuleItem::ModuleItem(ModuleTreeModel *model)
    :ModuleTreeItem(model->rootItem)
    ,m_model(model)
    ,m_isHooked(FALSE)
{

}

ModuleItem::~ModuleItem()
{

}

QString ModuleItem::modname() const
{
    return m_modname;
}

void ModuleItem::setModname(const QString &modname)
{
    m_modname = modname;
}
QString ModuleItem::modpath() const
{
    return m_modpath;
}

void ModuleItem::setModpath(const QString &modpath)
{
    m_modpath = modpath;
}
qint64 ModuleItem::modbase() const
{
    return m_modbase;
}

void ModuleItem::setModbase(const qint64 &modbase)
{
    m_modbase = modbase;
}

int ModuleItem::insertApi(ModuleApi_Type type, long RVA, long base, const QString &name, const char *redirName)
{
    ModuleApiTreeItem* api = findApi(RVA);
    if (api)
        return 0;
    api = new ModuleApiTreeItem(this);
    api->setRVA(RVA);
    api->setApiname(name);
    api->setApibase(base);
    api->setType(type);
    if (redirName)
        api->setRedirName(redirName);
    api->setModbase(m_modbase);
    api_opt_filter opt;
    m_model->getApiFilterInfo(modname(),name,base,type,&opt);
    api->setIsHooked(opt.bHook);
    api->setApiFlags(opt.flags);
    appendChild(api);
    return 1;
}

QVariant ModuleItem::data(int column)
{
    switch(column)
    {
    case 0:return QString("%1(%2)").arg(m_modname).arg(m_modbase,0,16);break;
    case 1:return m_modpath;break;
    }
    return QVariant();
}

Qt::ItemFlags ModuleItem::flags()
{
    return Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

ModuleApiTreeItem *ModuleItem::findApi(long RVA)
{
    foreach(ModuleTreeItem* item,childItems)
    {
        ModuleApiTreeItem* api = static_cast<ModuleApiTreeItem*>(item);
        if (api)
        {
            if (api->RVA() == RVA)
                return api;
        }
    }
    return 0;
}

TreeItemType ModuleItem::itemtype() const
{
    return TreeItem_Module;
}
BOOL ModuleItem::isHooked() const
{
    return m_isHooked;
}

void ModuleItem::setIsHooked(const BOOL &isHooked)
{
    m_isHooked = isHooked;
}




ModuleApiTreeItem::ModuleApiTreeItem(ModuleTreeItem *parent)
    :ModuleTreeItem(parent)
    ,m_refc(0)
    ,m_RVA(0)
{
    m_opt.bHook = TRUE;
    m_opt.flags = 0;
}

ModuleApiTreeItem::~ModuleApiTreeItem()
{

}
long ModuleApiTreeItem::RVA() const
{
    return m_RVA;
}

void ModuleApiTreeItem::setRVA(long RVA)
{
    m_RVA = RVA;
}

QVariant ModuleApiTreeItem::data(int column)
{
    switch(column)
    {
    case 0:
    {
        if (m_type == ModuleApi_REDIR)
            return QString("[%1]%2->%3").arg(m_apibase,4,16,QChar('0')).arg(m_apiname).arg(m_redirName);
        return QString("[%1]%2").arg(m_apibase,4,16,QChar('0')).arg(m_apiname);
    }break;
    case 1:
    {
        return QString("RVA:%1 \t|refc.:%2").arg(m_RVA,8,16,QChar('0')).arg(m_refc);
    }break;
    }

    return QVariant();
}

Qt::ItemFlags ModuleApiTreeItem::flags()
{
    return Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}
QString ModuleApiTreeItem::apiname() const
{
    return m_apiname;
}

void ModuleApiTreeItem::setApiname(const QString &apiname)
{
    m_apiname = apiname;
}

QString ModuleApiTreeItem::modname() const
{
    ModuleTreeItem* item = parent();
    if (item && item->itemtype() == TreeItem_Module)
    {
        ModuleItem* mditem = static_cast<ModuleItem*>(item);
        return mditem->modname();
    }
    return QString();
}
long ModuleApiTreeItem::apibase() const
{
    return m_apibase;
}

void ModuleApiTreeItem::setApibase(long apibase)
{
    m_apibase = apibase;
}

ULONG_PTR ModuleApiTreeItem::apiEntry() const
{
    return modbase()+m_RVA;
}
ModuleApi_Type ModuleApiTreeItem::type() const
{
    return m_type;
}

void ModuleApiTreeItem::setType(const ModuleApi_Type &type)
{
    m_type = type;
}
ULONG_PTR ModuleApiTreeItem::modbase() const
{
    return m_modbase;
}

void ModuleApiTreeItem::setModbase(const ULONG_PTR &modbase)
{
    m_modbase = modbase;
}
long ModuleApiTreeItem::refc() const
{
    return m_refc;
}

void ModuleApiTreeItem::setRefc(long refc)
{
    m_refc = refc;
}

TreeItemType ModuleApiTreeItem::itemtype() const
{
    return TreeItem_Api;
}
QString ModuleApiTreeItem::redirName() const
{
    return m_redirName;
}

void ModuleApiTreeItem::setRedirName(const QString &redirName)
{
    m_redirName = redirName;
}
BOOL ModuleApiTreeItem::isHooked() const
{
    return m_opt.bHook;
}

void ModuleApiTreeItem::setIsHooked(const BOOL &isHooked)
{
    m_opt.bHook = isHooked;
}
long ModuleApiTreeItem::apiFlags() const
{
    return m_opt.flags;
}

void ModuleApiTreeItem::setApiFlags(long apiFlags, bool on, bool replace)
{

    if (replace)
    {
        if (on)
            m_opt.flags = apiFlags;
        else
            m_opt.flags = 0;
    }else
    {
        if (on)
            m_opt.flags |= apiFlags;
        else
            m_opt.flags &= ~apiFlags;
    }

}
api_opt_filter* ModuleApiTreeItem::opt()
{
    return &m_opt;
}

void ModuleApiTreeItem::setOpt(const api_opt_filter &opt)
{
    m_opt = opt;
}










