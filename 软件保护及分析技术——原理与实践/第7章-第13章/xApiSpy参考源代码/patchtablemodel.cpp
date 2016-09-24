#include "patchtablemodel.h"

PatchTableModel::PatchTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

PatchTableModel::~PatchTableModel()
{

}

QVariant PatchTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    int nrow = index.row();
    if (nrow < 0 || nrow >= m_patchs.size())
        return QVariant();
    const PatchItem& item = m_patchs.at(nrow);
    switch(index.column())
    {
    case 0:return QString("%1").arg(item.rva,8,16,QChar('0'));break;
    case 1:return QString("%1").arg(item.size);break;
    case 2:
    {
        switch(item.type)
        {
        case PatchItem_Reloc:return trUtf8("Reloc");
        }
        return QString("%1").arg(item.type);
    }break;
    case 3:
    {
        switch(item.state)
        {
        case PatchItem_State_Enable:return trUtf8("Enable");
        case PatchItem_State_Disabled:return trUtf8("Disabled");
        }

        return QString("%1").arg(item.state);break;
    }

    }

    return QVariant();
}

int PatchTableModel::rowCount(const QModelIndex &parent) const
{
    return m_patchs.count();
}

int PatchTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant PatchTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {

        switch(section)
        {
        case 0:return "Address";break;
        case 1:return "Size";break;
        case 2:return "Type";break;
        case 3:return "state";break;
        }
    }

    return QVariant();
}

void PatchTableModel::pushRecord(qintptr rva, int size, int type)
{
    PatchItem* item = 0;
    if (m_index.contains(rva))
    {
        item = m_index.value(rva);
        item->size = size;
        item->type = type;
        return;
    }
    beginInsertRows(QModelIndex(),m_patchs.count(),m_patchs.count());
    PatchItem it;
    it.rva = rva;
    it.size = size;
    it.type = type;
    it.flags = 0;
    it.state = PatchItem_State_Disabled;
    m_patchs.push_back(it);
    m_index[rva] = &m_patchs.back();
    endInsertRows();
}

void PatchTableModel::insertRecords(PatchItemVector &items)
{
    int nhas = 0;
    for(PatchItemVector::iterator itr =items.begin();itr != items.end();itr++)
    {
        if (m_index.contains(itr->rva))
        {
            PatchItem* item = m_index.value(itr->rva);
            item->size = itr->size;
            item->type = itr->type;
            item->state = itr->state;
            item->flags |= PATCHITEM_FLAG_DELETED;
        }else
            nhas++;
    }
    if (nhas < 1)
        return;
    int lc = m_patchs.size();
    beginInsertRows(QModelIndex(),lc,lc+nhas-1);
    for(PatchItemVector::iterator itr =items.begin();itr != items.end();itr++)
    {
        if (!(itr->flags & PATCHITEM_FLAG_DELETED))
        {
            m_patchs.push_back(*itr);
            m_index[itr->rva] = &m_patchs.back();
        }
    }
    endInsertRows();
}

PatchItem *PatchTableModel::item(int row)
{
    if (row < 0 || row >= m_patchs.size())
        return 0;
    return &m_patchs.value(row);
}

PatchItemList *PatchTableModel::items()
{
    return &m_patchs;
}
