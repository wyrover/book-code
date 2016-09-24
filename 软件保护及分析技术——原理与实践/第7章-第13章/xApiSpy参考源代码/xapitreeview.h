#ifndef XAPITREEVIEW_H
#define XAPITREEVIEW_H

#include <QTreeView>

class XApiTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit XApiTreeView(QWidget *parent = 0);

protected:
    virtual     void mouseReleaseEvent(QMouseEvent *event);
signals:
    void onRightClick(QPoint pos,QModelIndexList ml);
public slots:

};

#endif // XAPITREEVIEW_H
