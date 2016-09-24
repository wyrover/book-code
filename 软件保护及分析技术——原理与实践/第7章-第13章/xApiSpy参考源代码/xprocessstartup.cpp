#include "xprocessstartup.h"

xProcessStartup::xProcessStartup()
{
    setWindowTitle(QString::fromUtf8("创建进程"));
    QVBoxLayout* layout = new QVBoxLayout;

    QHBoxLayout* hlay1 = new QHBoxLayout;
    QLabel* lab1 = new QLabel(QString::fromUtf8("执行文件:"));
    lab1->setMinimumWidth(80);
    lab1->setAlignment(Qt::AlignRight);

    m_txtExePath = new QLineEdit;
    m_txtExePath->setMinimumWidth(250);
    QPushButton* btn1 = new QPushButton(QString::fromUtf8("选择(&S)..."));
    connect(btn1,SIGNAL(clicked()),this,SLOT(psSelectExecuteFile()));

    hlay1->addWidget(lab1);
    hlay1->addWidget(m_txtExePath);
    hlay1->addWidget(btn1);


    QHBoxLayout* hlay2 = new QHBoxLayout;
    QLabel* lab2 = new QLabel(QString::fromUtf8("参数:"));
    lab2->setMinimumWidth(80);
    lab2->setAlignment(Qt::AlignRight);
    m_txtArgs = new QLineEdit;
    hlay2->addWidget(lab2);
    hlay2->addWidget(m_txtArgs);

    QHBoxLayout* hlay3 = new QHBoxLayout;
    QLabel* lab3 = new QLabel(QString::fromUtf8("启动目录:"));
    lab3->setMinimumWidth(80);
    lab3->setAlignment(Qt::AlignRight);
    m_txtWorkDir = new QLineEdit;
    QPushButton* btn3 = new QPushButton(QString::fromUtf8("选择(&S)..."));
    connect(btn3,SIGNAL(clicked()),this,SLOT(psSelectWorkDirectory()));
    hlay3->addWidget(lab3);
    hlay3->addWidget(m_txtWorkDir);
    hlay3->addWidget(btn3);


    QHBoxLayout* hlay4 = new QHBoxLayout;
    hlay4->setAlignment(Qt::AlignRight);
    QPushButton* btn4 = new QPushButton(QString::fromUtf8("确定(&O)"));
    connect(btn4,SIGNAL(clicked()),this,SLOT(accept()));
    hlay4->addWidget(btn4);
    QPushButton* btn5 = new QPushButton(QString::fromUtf8("取消(&C)"));
    connect(btn5,SIGNAL(clicked()),this,SLOT(reject()));
    hlay4->addWidget(btn5);

    layout->addLayout(hlay1);
    layout->addLayout(hlay2);
    layout->addLayout(hlay3);
    layout->addLayout(hlay4);
    setLayout(layout);

    //resize(400,150);

}

void xProcessStartup::getInfo(QString &fullname, QString &args, QString &workdir)
{
    fullname = QDir::toNativeSeparators(m_txtExePath->text());
    args = m_txtArgs->text();
    workdir = QDir::toNativeSeparators(m_txtWorkDir->text());
}

void xProcessStartup::psSelectExecuteFile()
{
    QString sPath = QFileDialog::getOpenFileName(this);
    if (sPath.isEmpty())
        return;
    QFileInfo fi(sPath);
    m_txtExePath->setText(fi.absoluteFilePath());
    m_txtWorkDir->setText(fi.absoluteDir().absolutePath());
}

void xProcessStartup::psSelectWorkDirectory()
{
    QString sPath = QFileDialog::getExistingDirectory(this);
    if (sPath.isEmpty())
        return;
    m_txtWorkDir->setText(sPath);
}
