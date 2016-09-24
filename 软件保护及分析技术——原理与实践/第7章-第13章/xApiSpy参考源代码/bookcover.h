#ifndef BOOKCOVER_H
#define BOOKCOVER_H

#include <QDialog>

namespace Ui {
class BookCover;
}

class BookCover : public QDialog
{
    Q_OBJECT

public:
    explicit BookCover(QWidget *parent = 0);
    ~BookCover();

    QString inputText(int nbook);
    void setTip(int ncharpter);
    void setTip2(int ncharpter);
    int getBookIndex();
private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::BookCover *ui;
};

#endif // BOOKCOVER_H
