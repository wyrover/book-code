#ifndef IMPORTTREEMODEL_H
#define IMPORTTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QPixmap>
#include <windows.h>


enum ImportItemType{
    ImportItem_None,
    ImportItem_Module,
    ImportItem_Api
};


enum ImpBuildRelTP{ //定义段内重定位类型
    RELOC_IMP_DATA,
    RELOC_IMP_ORITHUNK32,
    RELOC_IMP_FSTTHUNK32,
    RELOC_IMP_ORITHUNK64,
    RELOC_IMP_FSTTHUNK64,
    RELOC_IMP_ORITHUNKPTR,
    RELOC_IMP_FSTTHUNKPTR

};
struct ImpBuildTPVal{   //重定位具体偏移
    DWORD   offset;
};

struct PESymbolImport{  //导入表符号描述，我们用自己的格式来描述一个导入信息
    bool                    IsOrdinal;
    DWORD                   Ordinal;
    WORD                    Hint;
    QString                 ModuleName;
    QString                 FuncName;
};

//在重建过程当中，我们用MultiMap来归类
typedef QMultiMap<QString,PESymbolImport>               ImportSymbols;
typedef QMultiMap<ImpBuildRelTP,ImpBuildTPVal>          ImportBuildTP;

class ImportTreeItem
{
    friend class ImportTreeModel;
    friend class ImportModuleItem;
    friend class xApiSpy;
public:
    explicit ImportTreeItem(ImportTreeItem* parent = 0);
    virtual ~ImportTreeItem();

    void appendChild(ImportTreeItem *child);

    ImportTreeItem* parent() const;
    ImportTreeItem *child(int row);
    int childCount() const;
    virtual QVariant data(int column);
    int row() const;
    bool removeChild(ImportTreeItem *child);
    void clear();
    virtual Qt::ItemFlags flags();
    virtual ImportItemType itemtype() const;
    int getCheckState() const;
    void setCheckState(int value);

private:
    QList<ImportTreeItem*> childItems;
    ImportTreeItem*        parentitem;
    int checkState;
};

class ImportFuncItem : public ImportTreeItem
{

public:
    explicit ImportFuncItem(ImportTreeItem* parent = 0);
    virtual ~ImportFuncItem();
    long RVA() const;
    void setRVA(long RVA);

    virtual QVariant data(int column);
    virtual Qt::ItemFlags flags();

    QString apiname() const;
    void setApiname(const QString &apiname);

    QString modname() const;

    long apibase() const;
    void setApibase(long apibase);

    virtual ImportItemType itemtype() const;


private:
    long    m_RVA;
    QString m_apiname;
    long    m_apibase;
    long    m_nameHash;
};


class ImportTreeModel;
class ImportModuleItem : public ImportTreeItem
{
    friend class xApiSpy;
public:
    explicit ImportModuleItem(ImportTreeModel* model);
    virtual ~ImportModuleItem();
    QString modname() const;
    void setModname(const QString &modname);
    int insertApi(long hash, long RVA, const QString& name);

    virtual QVariant data(int column);
    virtual Qt::ItemFlags flags();

    ImportFuncItem* findApi(long RVA);

    virtual ImportItemType itemtype() const;

    long nameHash() const;
    void setNameHash(long nameHash);

private:
    QString     m_modname;
    long        m_nameHash;
    ImportTreeModel*    m_model;
};


class xApiSpy;
class ImportTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    friend class ImportModuleItem;
public:
    explicit ImportTreeModel(xApiSpy* ApiSpy,QObject *parent = 0);
    ~ImportTreeModel();

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

    ImportModuleItem*   findModule(long nameHash);
    ImportModuleItem*   pushModule(long nameHash,const QString& name);
    void appendItem(ImportModuleItem *item);
   // void removeModule(const qint64 modbase);
    void clear();
    void removeModule(long nameHash);
    int saveToImportSymbols(ImportSymbols* impNodes,BOOL full = FALSE);
    void saveItems(ImportSymbols* impNodes,ImportTreeItem* item,BOOL full = FALSE);
private:
    ImportTreeItem*     rootItem;
    QPixmap             m_icon_func;
    QPixmap             m_icon_data;
    QPixmap             m_icon_mod;
    xApiSpy*            m_apispy;
};

#endif // IMPORTTREEMODEL_H
