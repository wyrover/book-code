#include "apitablemodel.h"

ApiTableModel::ApiTableModel(QObject *parent) :
    QAbstractTableModel(parent)
  ,m_uniqID(1)
  ,m_lastc(0)
  ,m_maxRecords(30000)
{
    m_timerid = QObject::startTimer(500);
}

ApiTableModel::~ApiTableModel()
{
    QObject::killTimer(m_timerid);
}

QVariant ApiTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    NStackAutoLocker sl((NSafeLocker*)&m_lock);
    sl.Lock();
    apicallsList* acs = (apicallsList*)&m_calls;
    apiCallInfo* info = &acs->at(index.row());
    if (!info)
        return QVariant();
    switch(index.column())
    {
    case 0:return QString("%1").arg(info->id);break;
    case 1:return QString("%1").arg(info->threadID);break;
    case 2:return QString("%1").arg(info->retaddr,8,16,QChar('0'));break;
    case 3:
    {
        ModuleItem* mditem = static_cast<ModuleItem*>(info->item->parent());
        if (mditem)
            return QString("%1").arg(mditem->modname());
    }break;
    case 4:return QString("%1").arg(info->item->apibase(),4,16,QChar('0'));break;
    case 5:return QString("%1").arg(info->item->apiname());break;

    }

    return QVariant();
}

int ApiTableModel::rowCount(const QModelIndex &parent) const
{
    NStackAutoLocker sl((NSafeLocker*)&m_lock);
    sl.Lock();
    return m_lastc;
}

int ApiTableModel::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant ApiTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {

        switch(section)
        {
        case 0:return "No.";break;
        case 1:return "TID";break;
        case 2:return "RetAddr";break;
        case 3:return "module";break;
        case 4:return "base";break;
        case 5:return "name";break;
        }
    }

    return QVariant();
}

void ApiTableModel::pushCallLog(ULONG_PTR addr, ULONG_PTR retaddr, DWORD threadID, ModuleApiTreeItem *item)
{

    apiCallInfo info;
    info.id = m_uniqID++;
    info.addr = addr;
    info.retaddr = retaddr;
    info.threadID = threadID;
    info.item = item;
    m_lock.Lock();
    m_calls.push_back(info);
    m_lock.Unlock();
}

void ApiTableModel::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_timerid)
    {
        m_lock.Lock();
        long nc = m_calls.size();
        int delta = (int)nc - (int)m_lastc;
        if (delta > 0)
        {
            beginInsertRows(QModelIndex(),m_lastc,m_lastc+delta-1);
            m_lastc = m_calls.size();
            endInsertRows();
        }
        if (m_lastc > m_maxRecords)
        {
            delta = (int)m_lastc - (int)m_maxRecords;
            beginRemoveRows(QModelIndex(),0,delta-1);
            m_lastc = m_maxRecords;
            for (int i=0;i<delta;i++)
                m_calls.erase(m_calls.begin());
#ifdef _DEBUG
            assert(m_lastc == m_calls.size());
#endif
            endRemoveRows();
        }
        m_lock.Unlock();
    }
}
long ApiTableModel::maxRecords() const
{
    return m_maxRecords;
}

void ApiTableModel::setMaxRecords(long maxRecords)
{
    m_maxRecords = maxRecords;
}

void ApiTableModel::clear()
{
    m_lock.Lock();
    if (m_lastc > 0 && m_calls.size() > 0)
    {
        beginRemoveRows(QModelIndex(),0,m_lastc-1);
        m_lastc = 0;
        m_calls.clear();
        m_uniqID= 1;
        endRemoveRows();
    }
    m_lock.Unlock();
}

long ApiTableModel::currentCount()
{
    return m_lastc;
}

DWORD ApiTableModel::uniqID() const
{
    return m_uniqID;
}

void ApiTableModel::setUniqID(const DWORD &uniqID)
{
    m_uniqID = uniqID;
}

