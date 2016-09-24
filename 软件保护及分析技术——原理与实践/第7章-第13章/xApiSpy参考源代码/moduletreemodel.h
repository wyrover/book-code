#ifndef MODULETREEMODEL_H
#define MODULETREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QPixmap>
#include "moduletreeitem.h"
class ModuleTreeItem;
class xApiSpy;
struct api_opt_filter;

class ModuleTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    friend class ModuleItem;
public:
    explicit ModuleTreeModel(xApiSpy* ApiSpy,QObject *parent = 0);
    ~ModuleTreeModel();

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

    ModuleItem* findModule(const QString& modname);
    ModuleItem* findModule(const qint64 modbase);

    ModuleItem *pushModule(const qint64 modbase);
    void appendItem(ModuleItem *item);
    void removeModule(const qint64 modbase);

    BOOL getApiFilterInfo(const QString& modname,const QString& apiname,int base,ModuleApi_Type type,api_opt_filter *opt);
    void clear();
private:
    ModuleTreeItem*     rootItem;
    QPixmap             m_icon_func;
    QPixmap             m_icon_data;
    QPixmap             m_icon_mod;
    QPixmap             m_icon_redir;
    QPixmap             m_icon_bp;
    xApiSpy*            m_apispy;
};

#endif // MODULETREEMODEL_H
