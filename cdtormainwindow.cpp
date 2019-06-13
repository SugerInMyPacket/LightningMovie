#include "cdtormainwindow.h"
#include "ui_cdtormainwindow.h"

CdtorMainWindow::CdtorMainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::CdtorMainWindow)
{
    ui->setupUi(this);
    ui->tables->setTabsClosable(true);
    openMovie = false;
    font = new QFont("Microsoft YaHei",15);
    ConnectSS();
    ConnectDB();
}

CdtorMainWindow::~CdtorMainWindow()
{
    delete ui;
}


void CdtorMainWindow::ConnectDB(){
    dbSQL = DBConnector::ConnectDB();
}

void CdtorMainWindow::ConnectSS(){
    connect(ui->actConnectDB,SIGNAL(triggered()),this,SLOT(ConnectDB()));
    connect(ui->actShowMovie,SIGNAL(triggered()),this,SLOT(OpenMovieTable()));
    connect(ui->actSearchStage,SIGNAL(triggered()),this,SLOT(FindStage()));
    connect(ui->tables, SIGNAL(tabCloseRequested(int)), this, SLOT(CloseOneTable(int)));
    connect(ui->btnSearch,SIGNAL(clicked()),this,SLOT(FindStage()));
}

void CdtorMainWindow::OpenMovieTable(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_OPEN);
        return;
    }

    if(openMovie)return;

    QSqlQuery query(*dbSQL);
    const QString sql = "select * from movieview order by 电影,导演,主演";
    query.prepare(sql);
    if(!query.exec() || !(query.lastError().type() == QSqlError::NoError) ){
        QString error = "error code: "+ query.lastError().nativeErrorCode();
        error += "\nerror message: " + query.lastError().text();
        QMessageBox::critical(this,ERR_DB_QUERY,error);
        return;
    }

    QStringList header = GetHeader("movieview");
     DisplayQuery(query,header,"movie");

    openMovie = true;
    return;
}

void CdtorMainWindow::FindStage(){
    if(dbSQL==nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_OPEN);
        return;
    }

    QString strMovieName = ui->edtCondition->text();
    if(strMovieName.length()<=0) return;

    QSqlQuery query(*dbSQL);
    QString sql= "select * from stageview where movieName = ? order by playState, playDate,playTime,price,hallId;";
    query.prepare(sql);
    query.addBindValue(strMovieName);
    if(query.exec()){
        QStringList header = GetHeader("stageview");
        QString title = "stage."+strMovieName;
        DisplayQuery(query,header,title);
    }else{
        QString error = "errorCode: " + query.lastError().nativeErrorCode();
        error += ("\nerrorMessage: " + query.lastError().text());
        QMessageBox::critical(this, ERR_DB_QUERY, error);
    }
}

void CdtorMainWindow::CloseOneTable(int _currentIndex){
    QString tableName = ui->tables->tabText(_currentIndex);
    ui->tables->removeTab(_currentIndex);
    if (tableName =="movie" && openMovie) {
        openMovie = false;
    }
    return;
}

void CdtorMainWindow::DisplayQuery(QSqlQuery &_query, QStringList &_TableHeader, QString _TableName){
    QTableView *page = new QTableView();
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(_TableHeader);
    int n = _TableHeader.size();

    while (_query.next()) {
        QList<QStandardItem*> items;
        for (int i = 0; i < n; ++i) {
            QStandardItem* item = new QStandardItem(_query.value(i).toString());
            item->setTextAlignment(Qt::AlignCenter);
            item->setFont(*font);
            items.append(item);
        }
        model->appendRow(items);
    }

    page->setModel(model);
    page->resizeColumnsToContents();
    page->setEditTriggers(QTableView::NoEditTriggers);
    ui->tables->setCurrentIndex(
        ui->tables->addTab(
            page, QIcon(":/icon/table.png"), _TableName));
}

QStringList CdtorMainWindow::GetHeader(QString _TableName){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_OPEN);
        return QStringList();
    }
    QSqlQuery query(*dbSQL);
    QString sql = "show columns from "+_TableName+";";
    query.prepare(sql);
    if(query.exec() && query.lastError().type() == QSqlError::NoError){
        QStringList header;
        while(query.next()){
            header.append(query.value(0).toString());
        }
        return header;
    }else{
        QString error = "errorCode: " + query.lastError().nativeErrorCode();
        error += ("\nerrorMessage: " + query.lastError().text());
        QMessageBox::critical(this, ERR_DB_QUERY, error);
        return QStringList();
    }
}
