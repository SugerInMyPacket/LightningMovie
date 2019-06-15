#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include "dlgfindpswd.h"
#include "dlgregister.h"
#include "stringsource.h"
#include <QCloseEvent>
#include <QMouseEvent>
#include <qcryptographichash.h>
#include <qdebug.h>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qsettings.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include "dbconnector.h"


namespace Ui {
class DlgLogin;
}

/*
 *1. 还欠缺账户冻结功能
 * 2. 还欠缺两个验证器(验证账户的正确性)
 * 3. 登陆信号函数还欠缺数据库查询的部分
 */

class DlgLogin : public QDialog {
    Q_OBJECT

public:
    explicit DlgLogin(QWidget* parent = nullptr);
    ~DlgLogin() Q_DECL_OVERRIDE;
    bool Identity() const
    {
        return identity;
    }

private:
    bool bIsMoving;
    QString strUserNumber;
    QString strUserPass;
    bool identity;
    QPoint qpLastPoint;
    qint32 tryCount;
    DlgRegister* dlgRegister;
    DlgFindPswd* dlgFindPswd;
    QSqlDatabase db;
    static QString organization;
    static QString appName;
    void initializeUi();
    void connectDatabase();

protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;
private slots:
    void buttonLogin();
    void buttonCancel();
    void buttonOpenRegisterDlg();
    void buttonOpenFindPswdDlg();
    void chkboxSetAuto(bool _checked);
    void chkboxSetSave(bool _checked);

private:
    Ui::DlgLogin* ui;
};

#endif // DLGLOGIN_H
