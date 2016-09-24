#ifndef APITABLEMODEL_H
#define APITABLEMODEL_H

#include <QAbstractTableModel>
#include "../../../nCom/nlinkedlist.h"
#include "../../../nCom/nsafelocker.h"
#include "../../../nCom/avltree.h"
#include <QTimerEvent>
class ModuleApiTreeItem;
struct apiCallInfo
{
    long                id;
    ULONG_PTR           addr;
    ULONG_PTR           retaddr;
    DWORD               threadID;
    ModuleApiTreeItem*  item;
};

#include "moduletreeitem.h"

//typedef NLinkedList<apiCallInfo> apicallsList;
typedef index_list<apiCallInfo> apicallsList;
class ApiTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ApiTableModel(QObject *parent = 0);
    virtual ~ApiTableModel();

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    //virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    int rowCount(const QModelIndex &parent) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;


    void pushCallLog(ULONG_PTR addr,ULONG_PTR retaddr,DWORD threadID,ModuleApiTreeItem* item);

    DWORD uniqID() const;
    void setUniqID(const DWORD &uniqID);

    long maxRecords() const;
    void setMaxRecords(long maxRecords);

    void clear();

    long currentCount();

protected:
    virtual void timerEvent(QTimerEvent *e);
signals:

public slots:
private:
    apicallsList    m_calls;

    DWORD   m_uniqID;
    NSafeLocker m_lock;
    int     m_timerid;
    long    m_lastc;
    long    m_maxRecords;
};

#endif // APITABLEMODEL_H
