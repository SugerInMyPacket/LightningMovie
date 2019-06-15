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
#include "dbconnector.h"

namespace Ui {
class DlgRegister;
}

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
