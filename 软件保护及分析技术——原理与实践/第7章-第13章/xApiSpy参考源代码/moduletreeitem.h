#ifndef MODULETREEITEM_H
#define MODULETREEITEM_H

#include <windows.h>
#include <QList>
#include <QVariant>


struct api_opt_filter
{
    BOOL    bHook;
    long    flags;
};

typedef QMap<QString,api_opt_filter>    ftapi_map;
struct module_opt_filter
{
    BOOL        bHook;
    ftapi_map   apis;
};

enum TreeItemType{
    TreeItem_None,
    TreeItem_Module,
    TreeItem_Api
};


class ModuleTreeItem
{
    friend class ModuleTreeModel;
    friend class ModuleItem;
    friend class xApiSpy;
public:
    explicit ModuleTreeItem(ModuleTreeItem* parent = 0);
    virtual ~ModuleTreeItem();

    void appendChild(ModuleTreeItem *child);

    ModuleTreeItem* parent() const;
    ModuleTreeItem *child(int row);
    int childCount() const;
    virtual QVariant data(int column);
    int row() const;
    bool removeChild(ModuleTreeItem *child);
    void clear();
    virtual Qt::ItemFlags flags();
    virtual TreeItemType itemtype() const;


private:
    QList<ModuleTreeItem*> childItems;
    ModuleTreeItem*        parentitem;
};

enum ModuleApi_Type{
    ModuleApi_CODE,
    ModuleApi_DATA,
    ModuleApi_REDIR
};

class ModuleApiTreeItem : public ModuleTreeItem
{

public:
    explicit ModuleApiTreeItem(ModuleTreeItem* parent = 0);
    virtual ~ModuleApiTreeItem();
    long RVA() const;
    void setRVA(long RVA);

    virtual QVariant data(int column);
    virtual Qt::ItemFlags flags();

    QString apiname() const;
    void setApiname(const QString &apiname);

    QString modname() const;

    long apibase() const;
    void setApibase(long apibase);

    ULONG_PTR apiEntry() const;

    ModuleApi_Type type() const;
    void setType(const ModuleApi_Type &type);

    ULONG_PTR modbase() const;
    void setModbase(const ULONG_PTR &modbase);

    long refc() const;
    void setRefc(long refc);

    virtual TreeItemType itemtype() const;

    QString redirName() const;
    void setRedirName(const QString &redirName);

    BOOL isHooked() const;
    void setIsHooked(const BOOL &isHooked);


    long apiFlags() const;
    void setApiFlags(long apiFlags, bool on = true, bool replace = false);

    api_opt_filter *opt();
    void setOpt(const api_opt_filter &opt);

private:
    long    m_RVA;
    QString m_apiname;
    QString m_redirName;
    long    m_apibase;
    ULONG_PTR   m_modbase;
    ModuleApi_Type  m_type;
    long    m_refc;

    api_opt_filter m_opt;

};


class ModuleTreeModel;
class ModuleItem : public ModuleTreeItem
{
    friend class xApiSpy;
public:
    explicit ModuleItem(ModuleTreeModel* model);
    virtual ~ModuleItem();
    QString modname() const;
    void setModname(const QString &modname);

    QString modpath() const;
    void setModpath(const QString &modpath);

    qint64 modbase() const;
    void setModbase(const qint64 &modbase);

    int insertApi(ModuleApi_Type type, long RVA, long base, const QString& name,const char* redirName);

    virtual QVariant data(int column);
    virtual Qt::ItemFlags flags();

    ModuleApiTreeItem* findApi(long RVA);

    virtual TreeItemType itemtype() const;

    BOOL isHooked() const;
    void setIsHooked(const BOOL &isHooked);

private:
    QString m_modname;
    QString m_modpath;
    qint64  m_modbase;
    BOOL    m_isHooked;
    ModuleTreeModel*    m_model;
};


#endif // MODULETREEITEM_H
