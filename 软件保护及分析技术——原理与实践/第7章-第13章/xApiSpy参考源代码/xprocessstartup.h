#ifndef XPROCESSSTARTUP_H
#define XPROCESSSTARTUP_H

#include <QDialog>
#include <QtWidgets>
#include <QVBoxLayout>

class xProcessStartup : public QDialog
{
    Q_OBJECT
public:
    xProcessStartup();
    void getInfo(QString& fullname,QString& args,QString& workdir);
private slots:
    void psSelectExecuteFile();
    void psSelectWorkDirectory();
private:
    QLineEdit* m_txtExePath;
    QLineEdit* m_txtWorkDir;
    QLineEdit* m_txtArgs;
};

#endif // XPROCESSSTARTUP_H
