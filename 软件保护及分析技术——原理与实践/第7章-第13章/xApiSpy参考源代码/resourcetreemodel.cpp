#include "resourcetreemodel.h"

ResTreeItem::ResTreeItem(ResTreeItem *parent)
{
    parentitem = parent;
}
//! [0]

//! [1]
ResTreeItem::~ResTreeItem()
{
    clear();
}

void ResTreeItem::appendChild(ResTreeItem *child)
{
    childItems.append(child);
}

ResTreeItem *ResTreeItem::parent() const
{
    return parentitem;
}
//! [1]


//! [3]
ResTreeItem *ResTreeItem::child(int row)
{
    if (row >= childItems.count())
        return 0;
    return childItems.value(row);
}
//! [3]

//! [4]
int ResTreeItem::childCount() const
{
    return childItems.count();
}
//! [4]



//! [6]
QVariant ResTreeItem::data(int column)
{
    return QVariant();
}

int ResTreeItem::row() const
{
    if (parentitem)
        return parentitem->childItems.indexOf(const_cast<ResTreeItem*>(this));
    return -1;
}

bool ResTreeItem::removeChild(ResTreeItem *child)
{
    return childItems.removeOne(child);
}

void ResTreeItem::clear()
{
    foreach(ResTreeItem* item,childItems)
        delete item;
    childItems.clear();
}

Qt::ItemFlags ResTreeItem::flags()
{
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

ResItemType ResTreeItem::itemtype() const
{
    return ResItem_None;
}


ResTreeItem *ResTreeItem::child(ResourceNodeName &name)
{
    foreach(ResTreeItem* item,childItems)
    {
        if (item->name == name)
            return item;
    }
    return 0;
}




ResourceTreeModel::ResourceTreeModel(xApiSpy* ApiSpy,QObject *parent) :
    QAbstractItemModel(parent),
    m_apispy(ApiSpy)
  ,m_ico_folder(":/ico/rsrc/images/rsrc/folder.ico")
  ,m_ico_node(":/ico/rsrc/images/rsrc/node.ico")
  ,m_ico_data(":/ico/rsrc/images/rsrc/data.ico")
{
    rootItem = new ResTreeItem;
    rootItem->name.selected = true;
}

ResourceTreeModel::~ResourceTreeModel()
{
    delete rootItem;
}

QVariant ResourceTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    ResTreeItem *item = (ResTreeItem*)(index.internalPointer());
    if (!item)
        return QVariant();
    switch(role)
    {
    case Qt::CheckStateRole:
    {
        return item->name.selected?Qt::Checked:Qt::Unchecked;
    }break;
    case Qt::DecorationRole:
    {
        if (item->parent() == rootItem)
        {
            //decode known type
            return m_ico_folder;
        }else if (item->parent() && item->parent()->parent() == rootItem)
            return m_ico_node;
        else
            return m_ico_data;
    }break;
    case Qt::DisplayRole:
    {
        QString sname;
        if (!item->name.m_NameIsString)
        {

            if (item->parent() == rootItem)
            {
                //decode known type
                sname = decodeResType(item->name.m_id);
                if (!sname.isEmpty())
                    return sname;
            }
            return QString("ID:%1").arg(item->name.m_id);
        }
        else
        {
            sname = QString("\"%1\"").arg(QString::fromWCharArray((const wchar_t*)&item->name.m_sName[0],item->name.m_sName.size()/sizeof(wchar_t)));
        }
        return sname;
    }break;
    }
    return QVariant();

}

bool ResourceTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (index.column() != 0)
        return false;
    ResTreeItem *item = (ResTreeItem*)(index.internalPointer());
    if (!item)
        return false;
    switch(role)
    {
    case Qt::CheckStateRole:
    {
        item->name.selected = value.toBool();
        dataChanged(index,index);
        return true;
    }break;
    }

    return false;
}

Qt::ItemFlags ResourceTreeModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QVariant ResourceTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {

        switch(section)
        {
        case 0:return "Resource Tree";break;
        }
    }

    return QVariant();
}

QModelIndex ResourceTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ResTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ResTreeItem*>(parent.internalPointer());

    ResTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex ResourceTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    ResTreeItem *childItem = static_cast<ResTreeItem*>(index.internalPointer());
    ResTreeItem *parentItem = childItem->parent();

    if (parentItem == 0 || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ResourceTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;
    if (!parent.isValid())
        return rootItem->childCount();

    ResTreeItem* parentItem = static_cast<ResTreeItem*>(parent.internalPointer());
    if (!parentItem)
        return 0;
    return parentItem->childCount();
}

int ResourceTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex ResourceTreeModel::itemIndex(ResTreeItem *item)
{
    if (!item)
        return QModelIndex();
    return createIndex(item->row(), 0, item);
}

void ResourceTreeModel::insertResItem(ResourceLeaf &leaf)
{
    if (leaf.m_ResPath.size() < 1)
        return;
    ResTreeItem* item = rootItem;
    ResourcePath::iterator itr = leaf.m_ResPath.begin();
    ResourceNodeName* rn;
    for (;itr != leaf.m_ResPath.end();itr++)
    {
        rn = &*itr;
        ResTreeItem *child = item->child(*rn);
        if (!child)
        {
            beginInsertRows(itemIndex(item->parent()),item->childCount(),item->childCount());
            child = new ResTreeItem(item);
            child->name = *rn;
            item->appendChild(child);
            endInsertRows();
        }
        item = child;
    }

    //path same but body not same
    if (item->leaf.m_data.size() < leaf.m_data.size())
        item->leaf = leaf;
}

QString ResourceTreeModel::decodeResType(int type)
{
    switch(type)
    {
    case RT_CURSOR:return trUtf8("Cursor");break;
    case RT_BITMAP:return trUtf8("Bitmap");break;
    case RT_ICON:return trUtf8("Icon");break;
    case RT_MENU:return trUtf8("Menu");break;
    case RT_DIALOG:return trUtf8("Dialog");break;
    case RT_STRING:return trUtf8("String");break;
    case RT_FONTDIR:return trUtf8("FontDir");break;
    case RT_FONT:return trUtf8("Font");break;
    case RT_ACCELERATOR:return trUtf8("Accelerator");break;
    case RT_RCDATA:return trUtf8("RCDATA");break;
    case RT_MESSAGETABLE:return trUtf8("MessageTable");break;
    case RT_GROUP_CURSOR:return trUtf8("CursorGroup");break;
    case RT_GROUP_ICON:return trUtf8("IconGroup");break;
    case RT_VERSION:return trUtf8("Version");break;
    case RT_DLGINCLUDE:return trUtf8("DLGINCLUDE");break;
    case RT_PLUGPLAY:return trUtf8("PlugPlay");break;
    case RT_VXD:return trUtf8("VXD");break;
    case RT_ANICURSOR:return trUtf8("AniCursor");break;
    case RT_ANIICON:return trUtf8("AniIcon");break;
    case RT_HTML:return trUtf8("HTML");break;
    case RT_MANIFEST:return trUtf8("Manifest");break;
    }
    return QString();
}

int ResourceTreeModel::saveToResourceNodes(ResourceNodes *resNodes, BOOL full)
{
    ResourcePath path;
    saveLeafs(resNodes,rootItem,path,full);
    return resNodes->size();
}

void ResourceTreeModel::saveLeafs(ResourceNodes *resNodes, ResTreeItem *item, ResourcePath &paths, BOOL full)
{

    foreach(ResTreeItem* isub,item->childItems)
    {
        paths.push_back(isub->name);
        saveLeafs(resNodes,isub,paths,full);
        paths.pop_back();
    }

    if (!full)
    {
        ResTreeItem* itemlp = item;
        while(itemlp)
        {
            if (!itemlp->name.selected)
                return;
            itemlp = itemlp->parent();
        }
    }

    if (item->leaf.m_data.size() > 0)
    {
        item->leaf.m_ResPath = paths;
        resNodes->push_back(item->leaf);
    }
}
