#include "importtreemodel.h"

ImportTreeItem::ImportTreeItem(ImportTreeItem *parent)
    :checkState(Qt::Checked)
{
    parentitem = parent;
}
//! [0]

//! [1]
ImportTreeItem::~ImportTreeItem()
{
    clear();
}

void ImportTreeItem::appendChild(ImportTreeItem *child)
{
    childItems.append(child);
}

ImportTreeItem *ImportTreeItem::parent() const
{
    return parentitem;
}
//! [1]


//! [3]
ImportTreeItem *ImportTreeItem::child(int row)
{
    if (row >= childItems.count())
        return 0;
    return childItems.value(row);
}
//! [3]

//! [4]
int ImportTreeItem::childCount() const
{
    return childItems.count();
}
//! [4]



//! [6]
QVariant ImportTreeItem::data(int column)
{
    return QVariant();
}

int ImportTreeItem::row() const
{
    if (parentitem)
        return parentitem->childItems.indexOf(const_cast<ImportTreeItem*>(this));
    return -1;
}

bool ImportTreeItem::removeChild(ImportTreeItem *child)
{
    return childItems.removeOne(child);
}

void ImportTreeItem::clear()
{
    foreach(ImportTreeItem* item,childItems)
        delete item;
    childItems.clear();
}

Qt::ItemFlags ImportTreeItem::flags()
{
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

ImportItemType ImportTreeItem::itemtype() const
{
    return ImportItem_None;
}
int ImportTreeItem::getCheckState() const
{
    return checkState;
}

void ImportTreeItem::setCheckState(int value)
{
    checkState = value;
}




ImportModuleItem::ImportModuleItem(ImportTreeModel *model)
    :ImportTreeItem(model->rootItem)
    ,m_model(model)
{

}

ImportModuleItem::~ImportModuleItem()
{

}

QString ImportModuleItem::modname() const
{
    return m_modname;
}

void ImportModuleItem::setModname(const QString &modname)
{
    m_modname = modname;
}


int ImportModuleItem::insertApi(long hash, long RVA, const QString &name)
{
    ImportFuncItem* api = findApi(hash);
    if (api)
        return 0;
    api = new ImportFuncItem(this);
    api->setRVA(RVA);
    api->setApiname(name);
    if (name.startsWith("#"))
        api->setApibase(name.mid(1).toInt(0,16));
    appendChild(api);
    return 1;
}

QVariant ImportModuleItem::data(int column)
{
    switch(column)
    {
    case 0:return QString("%1").arg(m_modname);break;
    }
    return QVariant();
}

Qt::ItemFlags ImportModuleItem::flags()
{
    return Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

ImportFuncItem *ImportModuleItem::findApi(long RVA)
{
    foreach(ImportTreeItem* item,childItems)
    {
        ImportFuncItem* api = static_cast<ImportFuncItem*>(item);
        if (api)
        {
            if (api->RVA() == RVA)
                return api;
        }
    }
    return 0;
}

ImportItemType ImportModuleItem::itemtype() const
{
    return ImportItem_Module;
}
long ImportModuleItem::nameHash() const
{
    return m_nameHash;
}

void ImportModuleItem::setNameHash(long nameHash)
{
    m_nameHash = nameHash;
}




ImportFuncItem::ImportFuncItem(ImportTreeItem *parent)
    :ImportTreeItem(parent)
    ,m_RVA(0)
    ,m_apibase(0)
{
}

ImportFuncItem::~ImportFuncItem()
{

}
long ImportFuncItem::RVA() const
{
    return m_RVA;
}

void ImportFuncItem::setRVA(long RVA)
{
    m_RVA = RVA;
}

QVariant ImportFuncItem::data(int column)
{
    switch(column)
    {
    case 0:
    {
        return QString("[%1]%2").arg(m_apibase,4,16,QChar('0')).arg(m_apiname);
    }break;
    case 1:
    {
        return QString("RVA:%1").arg(m_RVA,8,16,QChar('0'));
    }break;
    }

    return QVariant();
}

Qt::ItemFlags ImportFuncItem::flags()
{
    return Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}
QString ImportFuncItem::apiname() const
{
    return m_apiname;
}

void ImportFuncItem::setApiname(const QString &apiname)
{
    m_apiname = apiname;
}

QString ImportFuncItem::modname() const
{
    ImportTreeItem* item = parent();
    if (item && item->itemtype() == ImportItem_Module)
    {
        ImportModuleItem* mditem = static_cast<ImportModuleItem*>(item);
        return mditem->modname();
    }
    return QString();
}
long ImportFuncItem::apibase() const
{
    return m_apibase;
}

void ImportFuncItem::setApibase(long apibase)
{
    m_apibase = apibase;
}

ImportItemType ImportFuncItem::itemtype() const
{
    return ImportItem_Api;
}

//! [0]
ImportTreeModel::ImportTreeModel(xApiSpy *ApiSpy, QObject *parent)
    : QAbstractItemModel(parent)
    ,m_apispy(ApiSpy)
{
    rootItem = new ImportTreeItem;
    m_icon_func.load(":/ico/images/func.png");
    m_icon_mod.load(":/ico/images/module.png");
}
//! [0]

//! [1]
ImportTreeModel::~ImportTreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int ImportTreeModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

ImportModuleItem *ImportTreeModel::findModule(long nameHash)
{
    foreach(ImportTreeItem* item,rootItem->childItems)
    {
        ImportModuleItem* module = static_cast<ImportModuleItem*>(item);
        if (module)
        {
            if (module->nameHash() == nameHash)
                return module;
        }
    }
    return 0;
}

ImportModuleItem *ImportTreeModel::pushModule(long nameHash, const QString &name)
{
    ImportModuleItem* item = findModule(nameHash);
    if (!item)
    {
        beginInsertRows(QModelIndex(),rootItem->childCount(),rootItem->childCount());
        item = new ImportModuleItem(this);
        item->setModname(name);
        item->setNameHash(nameHash);
        rootItem->appendChild(item);
        endInsertRows();
    }
    return item;
}

void ImportTreeModel::appendItem(ImportModuleItem *item)
{
    if (!item)
        return;
    beginInsertRows(QModelIndex(),rootItem->childCount(),rootItem->childCount());
    rootItem->appendChild(item);
    endInsertRows();
}


void ImportTreeModel::removeModule(long nameHash)
{
    ImportModuleItem* mod = findModule(nameHash);
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

int ImportTreeModel::saveToImportSymbols(ImportSymbols *impNodes, BOOL full)
{
    saveItems(impNodes,rootItem,full);
    return impNodes->size();
}

void ImportTreeModel::saveItems(ImportSymbols *impNodes, ImportTreeItem *item, BOOL full)
{
    foreach(ImportTreeItem* isub,item->childItems)
    {
        saveItems(impNodes,isub,full);
    }
    if (!full)
    {
        ImportTreeItem* itemlp = item;
        while(itemlp)
        {
            if (itemlp->getCheckState() != Qt::Checked)
                return;
            itemlp = itemlp->parent();
        }
    }

    if (item->itemtype() == ImportItem_Api)
    {
        ImportFuncItem* func = static_cast<ImportFuncItem*>(item);
        PESymbolImport sym;
        sym.FuncName = func->apiname();
        sym.Hint = 0;
        sym.IsOrdinal = (sym.FuncName.isEmpty() || sym.FuncName.startsWith("#"))?TRUE:FALSE;
        sym.Ordinal = func->apibase();
        sym.ModuleName = func->modname().trimmed().toLower();
        impNodes->insert(sym.ModuleName,sym);
    }
}


void ImportTreeModel::clear()
{
    if (rootItem->childCount() > 0)
    {
        beginRemoveRows(QModelIndex(),0,rootItem->childCount()-1);
        rootItem->clear();
        endRemoveRows();
    }
}


QVariant ImportTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    ImportTreeItem *item = (ImportTreeItem*)(index.internalPointer());
    switch(role)
    {
    case Qt::CheckStateRole:
    {
        if (index.column() == 0)
        {
            switch(item->itemtype())
            {
            case ImportItem_Module:
            {
                ImportModuleItem* mditem = static_cast<ImportModuleItem*>(item);
                return Qt::Checked;
            }break;
            case ImportItem_Api:
            {
                ImportFuncItem* apiitem = static_cast<ImportFuncItem*>(item);

                    return Qt::Checked;

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
            case ImportItem_Module:return m_icon_mod;
            case ImportItem_Api:
            {
                ImportFuncItem* apiitem = static_cast<ImportFuncItem*>(item);
                return m_icon_func;
            }break;
            }
        }
    }break;
    case Qt::DisplayRole:
    {
        if (!index.isValid())
        {
            return QVariant();
        }
        return item->data(index.column());
    }break;
    }
    return QVariant();
}

bool ImportTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (index.column() != 0)
        return false;
    ImportTreeItem *item = (ImportTreeItem*)(index.internalPointer());
    if (!item)
        return false;
    switch(role)
    {
    case Qt::CheckStateRole:
    {
        switch(item->itemtype())
        {
        case ImportItem_Module:
        {
            ImportModuleItem* moditem = static_cast<ImportModuleItem*>(item);
        }break;
        case ImportItem_Api:
        {
            ImportFuncItem* apitem = static_cast<ImportFuncItem*>(item);

        }break;
        }
        dataChanged(index,index);
        return true;
    }break;
    }

    return false;
}

Qt::ItemFlags ImportTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    ImportTreeItem *item = (ImportTreeItem*)(index.internalPointer());
    if (item)
        return item->flags();
    return QAbstractItemModel::flags(index);
}
//! [4]

//! [5]
QVariant ImportTreeModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {

        switch(section)
        {
        case 0:return "Apis";break;
        case 1:return "ThunkRVA";break;
        case 2:return "Summary";break;
        }
    }

    return QVariant();
}
//! [5]

//! [6]
QModelIndex ImportTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ImportTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ImportTreeItem*>(parent.internalPointer());

    ImportTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex ImportTreeModel::parent(const QModelIndex &index) const
{

    if (!index.isValid())
        return QModelIndex();

    ImportTreeItem *childItem = static_cast<ImportTreeItem*>(index.internalPointer());
    ImportTreeItem *parentItem = childItem->parent();

    if (parentItem == 0 || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


int ImportTreeModel::rowCount(const QModelIndex &parent) const
{
    ImportTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ImportTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}


