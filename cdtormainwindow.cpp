#include "cdtormainwindow.h"
#include "ui_cdtormainwindow.h"

CdtorMainWindow::CdtorMainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::CdtorMainWindow)
{
    ui->setupUi(this);
    ui->tables->setTabsClosable(true);
    openMovie = false;
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
}

void CdtorMainWindow::OpenMovieTable(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_OPEN);
        return;
    }

    if(openMovie)return;

    QSqlQuery query(*dbSQL);
    const QString sql = "select * from movie order by movieName";
    query.prepare(sql);
    if(!query.exec() || !(query.lastError().type() == QSqlError::NoError) ){
        QString error = "error code: "+ query.lastError().nativeErrorCode();
        error += "\nerror message: " + query.lastError().text();
        QMessageBox::critical(this,ERR_DB_QUERY,error);
        return;
    }

    QTableView *page = new QTableView();
    QStandardItemModel *model = new QStandardItemModel();
    QStringList header;
    header.append("电影");
    header.append("主演");
    header.append("导演");
    model->setHorizontalHeaderLabels(header);

    while (query.next()) {
        QList<QStandardItem*> items;
        for (int i = 1; i <= 3; ++i) {
            QStandardItem* item = new QStandardItem(query.value(i).toString());
            item->setTextAlignment(Qt::AlignCenter);
            items.append(item);
        }
        model->appendRow(items);
    }

    page->setModel(model);
    page->resizeColumnsToContents();
    page->setEditTriggers(QTableView::NoEditTriggers);
    ui->tables->setCurrentIndex(
        ui->tables->addTab(
            page, QIcon(":/icon/table.png"), "movie"));
    openMovie = true;
    return;
}

void CdtorMainWindow::FindStage(){

}

void CdtorMainWindow::CloseOneTable(int _currentIndex){
    QString tableName = ui->tables->tabText(_currentIndex);
    if (openMovie) {
        ui->tables->removeTab(_currentIndex);
        openMovie = false;
    }

    return;
}
