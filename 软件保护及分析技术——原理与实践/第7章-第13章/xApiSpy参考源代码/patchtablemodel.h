#ifndef PATCHTABLEMODEL_H
#define PATCHTABLEMODEL_H

#include <QAbstractTableModel>

enum PatchItem_Type{
    PatchItem_Reloc
};

enum PatchItem_State{
    PatchItem_State_Disabled,
    PatchItem_State_Enable
};

struct PatchItem{
    qintptr rva;
    int     size;
    int     type;
    int     state;
    long    flags;
};

typedef QVector<PatchItem>  PatchItemVector;
typedef QList<PatchItem>    PatchItemList;
#define PATCHITEM_FLAG_DELETED  0x80000000
class PatchTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PatchTableModel(QObject *parent = 0);
    ~PatchTableModel();
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    //virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    int rowCount(const QModelIndex &parent) const;
    int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void pushRecord(qintptr rva,int size,int type);
    void insertRecords(PatchItemVector& items);

    PatchItem* item(int row);
    PatchItemList* items();
signals:

public slots:
private:
    PatchItemList              m_patchs;
    QMap<qintptr,PatchItem*>    m_index;
};

#endif // PATCHTABLEMODEL_H
