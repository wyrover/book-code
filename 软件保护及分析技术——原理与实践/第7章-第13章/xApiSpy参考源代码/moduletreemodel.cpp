#include "moduletreeitem.h"
#include "moduletreemodel.h"
#include "xapispy.h"
#include "../xVMRuntime/xvmruntime.h"
#include <QStringList>

//! [0]
ModuleTreeModel::ModuleTreeModel(xApiSpy *ApiSpy, QObject *parent)
    : QAbstractItemModel(parent)
    ,m_apispy(ApiSpy)
    ,m_icon_bp(":/ico/images/Breakpoint.ico")
{
    rootItem = new ModuleTreeItem;
    m_icon_func.load(":/ico/images/func.png");
    m_icon_data.load(":/ico/images/dataptr.png");
    m_icon_mod.load(":/ico/images/module.png");
    m_icon_redir.load(":/ico/images/redirect.png");

}
//! [0]

//! [1]
ModuleTreeModel::~ModuleTreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int ModuleTreeModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}


ModuleItem *ModuleTreeModel::findModule(const QString &modname)
{
    QString scomp = modname.trimmed();
    foreach(ModuleTreeItem* item,rootItem->childItems)
    {
        ModuleItem* module = static_cast<ModuleItem*>(item);
        if (module)
        {
            if (module->modname().compare(scomp,Qt::CaseInsensitive) == 0)
                return module;
        }
    }
    return 0;
}

ModuleItem *ModuleTreeModel::findModule(const qint64 modbase)
{
    foreach(ModuleTreeItem* item,rootItem->childItems)
    {
        ModuleItem* module = static_cast<ModuleItem*>(item);
        if (module)
        {
            if (module->modbase() == modbase)
                return module;
        }
    }
    return 0;
}

ModuleItem* ModuleTreeModel::pushModule(const qint64 modbase)
{
    ModuleItem* item = findModule(modbase);
    if (!item)
    {
        beginInsertRows(QModelIndex(),rootItem->childCount(),rootItem->childCount());
        item = new ModuleItem(this);
        item->setModbase(modbase);
        rootItem->appendChild(item);
        endInsertRows();
    }
    return item;
}

void ModuleTreeModel::appendItem(ModuleItem *item)
{
    if (!item)
        return;
    beginInsertRows(QModelIndex(),rootItem->childCount(),rootItem->childCount());
    rootItem->appendChild(item);
    endInsertRows();
}


void ModuleTreeModel::removeModule(const qint64 modbase)
{
    ModuleItem* mod = findModule(modbase);
    if (mod)
    {
        int row = rootItem->childItems.indexOf(mod);
        beginRemoveRows(QModelIndex(),row,row);
        if (rootItem->removeChild(mod))
        {
            delete mod;
        }
        endRemoveRows();
    }
}

BOOL ModuleTreeModel::getApiFilterInfo(const QString &modname, const QString &apiname, int base, ModuleApi_Type type,api_opt_filter* opt)
{
    return m_apispy->getApiFilterInfo(modname,apiname,base,type,opt);
}

void ModuleTreeModel::clear()
{
    if (rootItem->childCount() > 0)
    {
        beginRemoveRows(QModelIndex(),0,rootItem->childCount()-1);
        rootItem->clear();
        endRemoveRows();
    }
}


QVariant ModuleTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    ModuleTreeItem *item = (ModuleTreeItem*)(index.internalPointer());
    switch(role)
    {
    case Qt::CheckStateRole:
    {
        if (index.column() == 0)
        {
            switch(item->itemtype())
            {
            case TreeItem_Module:
            {
                ModuleItem* mditem = static_cast<ModuleItem*>(item);
                return mditem->isHooked()?Qt::Checked:Qt::Unchecked;
            }break;
            case TreeItem_Api:
            {
                ModuleApiTreeItem* apiitem = static_cast<ModuleApiTreeItem*>(item);
                switch(apiitem->type())
                {
                case ModuleApi_CODE:
                case ModuleApi_DATA:
                    return apiitem->isHooked()?Qt::Checked:Qt::Unchecked;
                case ModuleApi_REDIR:return Qt::Unchecked;
                }

            }break;
            }
        }
    }break;
    case Qt::DecorationRole:
    {
        if (index.column() == 0)
        {
            switch(item->itemtype())
            {
            case TreeItem_Module:return m_icon_mod;
            case TreeItem_Api:
            {
                ModuleApiTreeItem* apiitem = static_cast<ModuleApiTreeItem*>(item);
                if (apiitem->apiFlags() & xvm_api_flag_breakpoint)
                    return m_icon_bp;
                switch(apiitem->type())
                {
                case ModuleApi_CODE:return m_icon_func;
                case ModuleApi_DATA:return m_icon_data;
                case ModuleApi_REDIR:return m_icon_redir;
                }

            }break;
            }
        }
    }break;
    case Qt::DisplayRole:
    {
        if (!index.isValid())
        {
            //top level
            // index.column()
            return QVariant();
        }
        return item->data(index.column());
    }break;
    }
    return QVariant();
}

bool ModuleTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (index.column() != 0)
        return false;
    ModuleTreeItem *item = (ModuleTreeItem*)(index.internalPointer());
    if (!item)
        return false;
    switch(role)
    {
    case Qt::CheckStateRole:
    {
        switch(item->itemtype())
        {
        case TreeItem_Module:
        {
            ModuleItem* moditem = static_cast<ModuleItem*>(item);
            moditem->setIsHooked(value.toBool());
            m_apispy->setModuleBeHook(moditem->modname(),value.toBool());
            //m_apispy->find
        }break;
        case TreeItem_Api:
        {
            ModuleApiTreeItem* apitem = static_cast<ModuleApiTreeItem*>(item);
            if (apitem->type() == ModuleApi_CODE)
            {
                apitem->setIsHooked(value.toBool());
                m_apispy->setModuleApiOpt(apitem->modname(),apitem->apiname(),apitem->apibase(),apitem->opt());
            }
        }break;
        }
        dataChanged(index,index);
        return true;
    }break;
    }

    return false;
}

Qt::ItemFlags ModuleTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    ModuleTreeItem *item = (ModuleTreeItem*)(index.internalPointer());
    if (item)
        return item->flags();
    return QAbstractItemModel::flags(index);
}
//! [4]

//! [5]
QVariant ModuleTreeModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {

        switch(section)
        {
        case 0:return "Apis";break;
        case 1:return "Summary";break;
        }
    }

    return QVariant();
}
//! [5]

//! [6]
QModelIndex ModuleTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ModuleTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ModuleTreeItem*>(parent.internalPointer());

    ModuleTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex ModuleTreeModel::parent(const QModelIndex &index) const
{

    if (!index.isValid())
        return QModelIndex();

    ModuleTreeItem *childItem = static_cast<ModuleTreeItem*>(index.internalPointer());
    ModuleTreeItem *parentItem = childItem->parent();

    if (parentItem == 0 || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


int ModuleTreeModel::rowCount(const QModelIndex &parent) const
{
    ModuleTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ModuleTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}


