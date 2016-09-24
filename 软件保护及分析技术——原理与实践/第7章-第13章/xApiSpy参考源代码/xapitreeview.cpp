#include <QMouseEvent>
#include "xapitreeview.h"

XApiTreeView::XApiTreeView(QWidget *parent) :
    QTreeView(parent)
{
}

void XApiTreeView::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeView::mouseReleaseEvent(event);
    if (!(event->button() & Qt::RightButton))
        return;
    QItemSelectionModel* sm = selectionModel();
    if (!sm)
        return;
    QModelIndexList mil = sm->selectedIndexes();
    if (mil.size() < 1)
        return;
    onRightClick(event->globalPos(),mil);
}
