#ifndef DLGREGISTER_H
#define DLGREGISTER_H

#include "stringsource.h"
#include <QRegExpValidator>
#include <qcryptographichash.h>
#include <qdebug.h>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qstring.h>
#include <qtimer.h>

namespace Ui {
class DlgRegister;
}

/*
 * 1. 用户输入数据的验证器
 * 2. 数据库的管理操作submit槽函数
 * 3. 窗口图标和组件图标
 * 4. 验证码发送API 
 */

class DlgRegister : public QDialog {
    Q_OBJECT

public:
    explicit DlgRegister(QWidget* parent = nullptr);
    ~DlgRegister() Q_DECL_OVERRIDE;

private:
    QSqlDatabase db;
    QTimer* phSendTimer;
    QTimer* maSendTimer;
    qint32 phSecond;
    qint32 maSecond;
    void initializeUi();
    void connectDatabase();
    QString encrypt(const QString& _str);
private slots:
    void buttonSubmit();
    void buttonCancel();
    void buttonQuit();

private:
    Ui::DlgRegister* ui;
};

#endif // DLGREGISTER_H
