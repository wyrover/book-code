#include "bookcover.h"
#include "ui_bookcover.h"

BookCover::BookCover(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookCover)
{
    ui->setupUi(this);
}

BookCover::~BookCover()
{
    delete ui;
}

QString BookCover::inputText(int nbook)
{
    return (nbook==0)?ui->cmbText->currentText():ui->cmbText2->currentText();
}

void BookCover::setTip(int ncharpter)
{
    QString si = QString::fromWCharArray(L"<html><head/><body><p><span style=\" font-size:16pt;\">本书第%1章的标题为：</span></p></body></html>");
    QString disp = QString(si).arg(ncharpter);
    ui->labTip->setText(disp);
}

void BookCover::setTip2(int ncharpter)
{
    QString si = QString::fromWCharArray(L"<html><head/><body><p><span style=\" font-size:16pt;\">本书第%1章的标题为：</span></p></body></html>");
    QString disp = QString(si).arg(ncharpter);
    ui->labTip2->setText(disp);
}

int BookCover::getBookIndex()
{
    return ui->rbk1->isChecked()?0:1;
}

void BookCover::on_btnCancel_clicked()
{
    reject();
}

void BookCover::on_btnOK_clicked()
{
    accept();
}
