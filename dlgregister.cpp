#include "dlgregister.h"
#include "ui_dlgregister.h"

extern const QString STR_CDTOR;
extern const QString STR_ADMIN;
extern const QString LAB_USER;
extern const QString LAB_PASS;
extern const QString LAB_PASS_CONF;
extern const QString LAB_PHONE;
extern const QString LAB_PHONE_VER;
extern const QString LAB_MAIL;
extern const QString LAB_MAIL_VER;
extern const QString LAB_IDENTIFY;
extern const QString BTN_SUBMIT;
extern const QString BTN_CANCEL;
extern const QString BTN_QUIT;
extern const QString BTN_SEND;
extern const QString SQL_DB_TYPE;
extern const QString SQL_DB_HOST;
extern const int SQL_DB_PORT;
extern const QString SQL_DB_USER;
extern const QString SQL_DB_PSWD;
extern const QString SQL_DB_NAME;
extern const QString ERR_DB_OPEN;
extern const QString DLG_TITLE_REGISTER;

DlgRegister::DlgRegister(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DlgRegister)
{
    ui->setupUi(this);
    ui->edtPswd->setEchoMode(QLineEdit::Password);
    ui->edtPswdConf->setEchoMode(QLineEdit::Password);
    ui->cmbIdentity->addItem(STR_CDTOR);
    ui->cmbIdentity->addItem(STR_ADMIN);
    phSendTimer = new QTimer(this);
    maSendTimer = new QTimer(this);
    phSendTimer->setSingleShot(false);
    maSendTimer->setSingleShot(false);
    phSecond = 100;
    maSecond = 100;
    initializeUi();
    connect(ui->btnQuit, SIGNAL(clicked()), this, SLOT(buttonQuit()));
    connect(ui->btnSubmit, SIGNAL(clicked()), this, SLOT(buttonSubmit()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
    connect(ui->btnSendPhVer, SIGNAL(clicked()), this, SLOT(buttonSendPhnVerCode()));
    connect(ui->btnSendMaVer, SIGNAL(clicked()), this, SLOT(buttonSendMlVerCode()));
    connect(phSendTimer, SIGNAL(timeout()), this, SLOT(updatePhSend()));
    connect(maSendTimer, SIGNAL(timeout()), this, SLOT(updateMaSend()));
    setWindowFlag(Qt::SubWindow);
    setWindowTitle(DLG_TITLE_REGISTER);
}

DlgRegister::~DlgRegister()
{
    delete ui;
    delete phSendTimer;
    delete maSendTimer;
}

void DlgRegister::initializeUi()
{
    ui->labUser->setText(LAB_USER);
    ui->labPswd->setText(LAB_PASS);
    ui->labPswdConf->setText(LAB_PASS_CONF);
    ui->labPhone->setText(LAB_PHONE);
    ui->labPhVerCode->setText(LAB_PHONE_VER);
    ui->labMail->setText(LAB_MAIL);
    ui->labMaiVerCode->setText(LAB_MAIL_VER);
    ui->labIdentity->setText(LAB_IDENTIFY);
    ui->btnSubmit->setText(BTN_SUBMIT);
    ui->btnCancel->setText(BTN_CANCEL);
    ui->btnQuit->setText(BTN_QUIT);
    ui->btnSendPhVer->setText(BTN_SEND);
    ui->btnSendMaVer->setText(BTN_SEND);
    ui->labBak->setFixedSize(this->width(), this->height());
}

void DlgRegister::connectDatabase()
{
    db = QSqlDatabase::addDatabase(SQL_DB_TYPE);
    db.setPort(SQL_DB_PORT);
    db.setHostName(SQL_DB_HOST);
    db.setUserName(SQL_DB_USER);
    db.setPassword(SQL_DB_PSWD);
    db.setDatabaseName(SQL_DB_NAME);
    if (!db.open()) {
        QString error = "error code: ";
        error += (db.lastError().nativeErrorCode() + "\n");
        error += "error text: ";
        error += db.lastError().text();
        QMessageBox::critical(this, ERR_DB_OPEN, error);
    }
}

QString DlgRegister::encrypt(const QString& _str)
{
    QByteArray bytContainer;
    bytContainer.append(_str);
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(bytContainer);
    return hash.result().toHex();
}

void DlgRegister::buttonSubmit()
{
    /*待续*/
}

void DlgRegister::buttonCancel()
{
    ui->edtUser->setText("");
    ui->edtPswd->setText("");
    ui->edtPswdConf->setText("");
    ui->edtMail->setText("");
    ui->edtPhone->setText("");
    ui->edtPhVerCode->setText("");
    ui->edtMaiVerCode->setText("");
}

void DlgRegister::buttonQuit()
{
    this->hide();
    QObject* temp = parent();
    QDialog* parent = static_cast<QDialog*>(temp);
    parent->show();
}

void DlgRegister::buttonSendPhnVerCode()
{
    ui->btnSendPhVer->setEnabled(false);
    phSendTimer->start(1000);
    qDebug() << "运行发送短信的API" << endl;
    /*
     *运行发送短信的API
     */
}

void DlgRegister::buttonSendMlVerCode()
{
    ui->btnSendMaVer->setEnabled(false);
    maSendTimer->start(1000);
    qDebug() << "运行发送邮件的API" << endl;
    /*
     *运行发送邮件的API
     */
}

void DlgRegister::updatePhSend()
{
    phSecond--;
    if (phSecond > 0) {
        ui->btnSendPhVer->setText(QString("(%1S)").arg(phSecond));
    } else if (phSendTimer->isActive()) {
        phSecond = 100;
        phSendTimer->stop();
        ui->btnSendPhVer->setText(BTN_SEND);
        ui->btnSendPhVer->setEnabled(true);
    }
}

void DlgRegister::updateMaSend()
{
    maSecond--;
    if (maSecond > 0) {
        ui->btnSendMaVer->setText(QString("(%1S)").arg(maSecond));
    } else if (maSendTimer->isActive()) {
        maSecond = 100;
        maSendTimer->stop();
        ui->btnSendMaVer->setText(BTN_SEND);
        ui->btnSendMaVer->setEnabled(true);
    }
}
