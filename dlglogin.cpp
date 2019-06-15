#include "dlglogin.h"
#include "ui_dlglogin.h"

bool permissionFlag;
extern const QString ORGANIZATION;
extern const QString APP_NAME;
extern const QString LAB_USER;
extern const QString LAB_PASS;
extern const QString EDT_TIP_USER;
extern const QString EDT_TIP_PASS;
extern const QString CHB_ADMINI;
extern const QString CHB_SAVE;
extern const QString CHB_AUTO;
extern const QString BTN_LOGIN;
extern const QString BTN_CANCEL;
extern const QString BTN_FIND_PASS;
extern const QString LAB_REGESTER;
extern const QString SQL_DB_TYPE;
extern const int SQL_DB_PORT;
extern const QString SQL_DB_HOST;
extern const QString SQL_DB_USER;
extern const QString SQL_DB_PSWD;
extern const QString SQL_DB_NAME;
extern const QString ERR_DB_OPEN;
extern const QString WAR_TITLE;
extern const QString WAR_EXITE;
extern const QString ERR_CRITICLE_LOGIN;
extern const QString ERR_LAST_CHANCE;
extern const QString ERR_WRONG_PSWD;

DlgLogin::DlgLogin(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DlgLogin)
{
    bIsMoving = false;
    dlgRegister = nullptr;
    dlgFindPswd = nullptr;
    ui->setupUi(this);
    initializeUi();
    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(buttonLogin()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
    connect(ui->btnFindPass, SIGNAL(clicked()), this, SLOT(buttonOpenFindPswdDlg()));
    connect(ui->btnRegister, SIGNAL(clicked()), this, SLOT(buttonOpenRegisterDlg()));
    connect(ui->chbAuto, SIGNAL(clicked(bool)), this, SLOT(chkboxSetSave(bool)));
    connect(ui->chbPass, SIGNAL(clicked(bool)), this, SLOT(chkboxSetAuto(bool)));
    ui->edtPass->setEchoMode(QLineEdit::Password);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlag(Qt::SplashScreen);
    connectDatabase();
}

DlgLogin::~DlgLogin()
{
    delete ui;
}

void DlgLogin::initializeUi()
{
    ui->labUser->setText(LAB_USER);
    ui->labPass->setText(LAB_PASS);
    ui->edtUser->setToolTip(EDT_TIP_USER);
    ui->edtPass->setToolTip(EDT_TIP_PASS);
    ui->chbAdmin->setText(CHB_ADMINI);
    ui->chbPass->setText(CHB_SAVE);
    ui->chbAuto->setText(CHB_AUTO);
    ui->btnLogin->setText(BTN_LOGIN);
    ui->btnCancel->setText(BTN_QUIT);
    ui->btnFindPass->setText(BTN_FIND_PASS);
    ui->labBkg->setFixedSize(this->width(), this->height());
    ui->btnRegister->setText(LAB_REGESTER);
}

void DlgLogin::connectDatabase()
{
    QSqlDatabase *tmp = DBConnector::ConnectDB();
    if(tmp == nullptr){
        exit(0);
    }else{
        db = *DBConnector::ConnectDB();
    }
}

void DlgLogin::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        bIsMoving = true;
        qpLastPoint = event->globalPos() - pos();
    }
    return QDialog::mousePressEvent(event);
}

void DlgLogin::mouseMoveEvent(QMouseEvent* event)
{
    if (bIsMoving && (event->buttons() & Qt::LeftButton)
        && (event->globalPos() - qpLastPoint).manhattanLength()
            > QApplication::startDragDistance()) {
        move(event->globalPos() - qpLastPoint);
        qpLastPoint = event->globalPos() - pos();
    }
    return QDialog::mouseMoveEvent(event);
}

void DlgLogin::mouseReleaseEvent(QMouseEvent* event)
{
    bIsMoving = false;
    return QDialog::mouseReleaseEvent(event);
}

void DlgLogin::closeEvent(QCloseEvent* event)
{
    //    writeSettings();
    return QDialog::closeEvent(event);
}

void DlgLogin::buttonLogin()
{
    bool legal = false;
    strUserNumber = ui->edtUser->text();
    strUserPass = ui->edtPass->text();
    if (strUserNumber.isEmpty() || strUserPass.isEmpty()) {
        QMessageBox::critical(this,ERR_LOGIN,ERR_LOGIN_COMPELETE);
        return;
    }
    if(!db.isOpen()){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_NULL);
        return;
    }

    QSqlQuery query(db);
    QString sql;
    bool bIsAdmin = ui->chbAdmin->isChecked();
    if(bIsAdmin){
        sql = "select adminNumber from adminInfo where adminPass = ?;";
    }else{
        sql ="select cdtNumber from conductor where cdtPass  = ?;";
    }
    query.prepare(sql);
    query.bindValue(0,strUserPass);
    if(query.exec() && query.lastError().type() == QSqlError::NoError){
        while(query.next()){
            QString user = query.value(0).toString();
            if(strUserNumber == user){
                legal = true;
            }
        }
    }else{
        QString error = "Error Code: " + query.lastError().nativeErrorCode();
        error += ("\nError Message: " + query.lastError().text());
        QMessageBox::critical(this, ERR_DB_QUERY, error);
        return;
    }

    if(!legal){
        tryCount++;
        if (tryCount > 3) {
            QMessageBox::critical(this, ERR_LOGIN, ERR_LAST_CHANCE);
            this->reject();
        } else {
            QMessageBox::critical(this,ERR_LOGIN,ERR_LOGIN_WRONG);
        }
        return;
    }
    //for legal user
    permissionFlag = bIsAdmin;
    this->accept();
    return;
}

void DlgLogin::buttonCancel()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, WAR_TITLE, WAR_EXITE,
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        this->close();
    }
    return;
}

void DlgLogin::chkboxSetAuto(bool _checked)
{
    if (!_checked) {
        ui->chbAuto->setChecked(_checked);
    }
}

void DlgLogin::chkboxSetSave(bool _checked)
{
    if (_checked) {
        ui->chbPass->setChecked(_checked);
    }
}

void DlgLogin::buttonOpenRegisterDlg()
{
    if (dlgRegister == nullptr) {
        dlgRegister = new DlgRegister(this);
    }
    dlgRegister->show();
    return;
}

void DlgLogin::buttonOpenFindPswdDlg()
{
    if (dlgFindPswd == nullptr) {
        dlgFindPswd = new DlgFindPswd(this);
    }
    dlgFindPswd->show();
    hide();
    return;
}

QString DlgLogin::organization = ORGANIZATION;
QString DlgLogin::appName = APP_NAME;
