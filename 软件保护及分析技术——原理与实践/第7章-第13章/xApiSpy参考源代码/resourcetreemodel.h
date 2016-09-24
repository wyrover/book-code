#ifndef RESOURCETREEMODEL_H
#define RESOURCETREEMODEL_H

#include <QAbstractItemModel>
#include "../xVMRuntime/xvmresource.h"
#include <QPixmap>
enum ResItemType{
    ResItem_None,
    ResItem_DIR,
    ResItem_DATA
};


class ResTreeItem
{
    friend class xApiSpy;
    friend class ResourceTreeModel;
public:
    explicit ResTreeItem(ResTreeItem* parent = 0);
    virtual ~ResTreeItem();

    void appendChild(ResTreeItem *child);

    ResTreeItem* parent() const;
    ResTreeItem *child(int row);
    int childCount() const;
    virtual QVariant data(int column);
    int row() const;
    bool removeChild(ResTreeItem *child);
    void clear();
    virtual Qt::ItemFlags flags();
    virtual ResItemType itemtype() const;

    ResTreeItem* child(ResourceNodeName& name);


private:
    QList<ResTreeItem*> childItems;
    ResTreeItem*        parentitem;
    ResourceNodeName    name;
    ResourceLeaf        leaf;
};

class xApiSpy;
class ResourceTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ResourceTreeModel(xApiSpy* ApiSpy,QObject *parent = 0);
    ~ResourceTreeModel();


    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex itemIndex(ResTreeItem *item);

    void insertResItem(ResourceLeaf& leaf);

    static QString decodeResType(int type);

    int saveToResourceNodes(ResourceNodes* resNodes,BOOL full = FALSE);
    void saveLeafs(ResourceNodes* resNodes,ResTreeItem* item,ResourcePath& paths,BOOL full = FALSE);
signals:

public slots:

private:
    xApiSpy*            m_apispy;
    ResTreeItem*        rootItem;

    QPixmap             m_ico_folder;
    QPixmap             m_ico_node;
    QPixmap             m_ico_data;

};

#endif // RESOURCETREEMODEL_H
