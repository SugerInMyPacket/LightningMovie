#include "adminmainwindow.h"
#include "ui_adminmainwindow.h"

extern const QString ERR_DB_NULL;
extern const QString ERR_DB_QUERY;
extern const QString WIN_TITLE_ADMIN;
extern const QString WIN_TITLE_CDCTOR;
extern const QString TAB_TITLE_WELCOME;
extern const QString SQL_DB_NAME;
extern const QString SQL_DB_DETAIL;

AdminMainWindow::AdminMainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminMainWindow)
{
    ui->setupUi(this);
    ui->tables->setTabsClosable(true);
    map = new QMap<QString, bool>;
    dbSQL = DBConnector::ConnectDB();
    showDBTree = true;
    initializeUI();
    initializeMap();
    openOneTable("movie");
    Connect_Signal_Slot();
    ui->statusBar->showMessage("ready...");
}

AdminMainWindow::~AdminMainWindow()
{
    delete map;
}

void AdminMainWindow::initializeUI()
{
    font  = new QFont("Microsoft YaHei",20);
    ui->btnSearch->setText(SEARCH);
}

void AdminMainWindow::initializeMap()
{
    if (dbSQL == nullptr)
        return;
    QSqlQuery* query = new QSqlQuery(*dbSQL);
    if (!query->exec(SQL_USE_DB)) {
        QMessageBox::critical(this, ERR_DB_QUERY, query->lastError().text());
        return;
    }
    query->exec(SQL_SHOW_TABLES);
    while (query->next()) {
        QString tableName = query->value(0).toString();
        map->insert(tableName, false);
    }
}

void AdminMainWindow::Connect_Signal_Slot()
{
    connect(ui->tables, SIGNAL(tabCloseRequested(int)), this, SLOT(closeOneTable(int)));
    connect(ui->actReConnectDB, SIGNAL(triggered()), this, SLOT(reconnectDB()));
    
    connect(ui->actShowMovie, SIGNAL(triggered()), this, SLOT(showMovie()));
    connect(ui->actAddMoive,SIGNAL(triggered()),this,SLOT(addMovie()));
    connect(ui->actRemoveMovie,SIGNAL(triggered()),this,SLOT(removeMovie()));
    connect(ui->actModifyMovie,SIGNAL(triggered()),this,SLOT(modifyMovie()));

    connect(ui->actShowLabel, SIGNAL(triggered()), this, SLOT(showLabel()));
    connect(ui->actShowMovieLabel, SIGNAL(triggered()), this, SLOT(showMovieLabel()));
    
    connect(ui->actShowHall, SIGNAL(triggered()), this, SLOT(showHall()));
    
    connect(ui->actShowTime, SIGNAL(triggered()), this, SLOT(showTimeLine()));
    
    connect(ui->actShowPlayState,SIGNAL(triggered()),this,SLOT(showPlayState()));
    
   connect(ui->actShowTicket, SIGNAL(triggered()), this, SLOT(showTicket()));

   
    
    connect(ui->actClearHistory, SIGNAL(triggered()), this, SLOT(clearHistory()));
    connect(ui->actSaveHistory, SIGNAL(triggered()), this, SLOT(saveHistory()));
}

void AdminMainWindow::openOneTable(const QString _tableName)
{
    if (dbSQL == nullptr)
        return;
    ui->statusBar->showMessage(_tableName);
    if (!map->contains(_tableName) || (*map)[_tableName]) {
        return;
    } else {
        (*map)[_tableName] = true;
    }
    QSqlQuery query(*dbSQL);
    if (!query.exec(SQL_USE_DB)) {
        QString error = "errorCode: " + query.lastError().nativeErrorCode();
        error += ("\nerrorMessage: " + query.lastError().text());
        QMessageBox::critical(this, ERR_DB_QUERY, error);
        return;
    }
    ui->statusBar->showMessage(SQL_USE_DB);

    datetime = QDateTime::currentDateTime();
    QString log = datetime.toString("[yyyy-MM-dd hh:mm:ss]");
    log += ("\n正在打开表：" + _tableName + "……");
    ui->stateArea->appendPlainText(log);

    // create a new table
    QString sql = "show columns from " + _tableName + ";";
    QTableView* page = new QTableView;
    QStandardItemModel* model = new QStandardItemModel;
    query.exec(sql);

    ui->statusBar->showMessage("set tablel headers...");
    int count = query.size();
    QStringList headers;
    while (query.next()) {
        headers.append(query.value(0).toString());
    }
    model->setHorizontalHeaderLabels(headers);

    ui->statusBar->showMessage("select data...");
    sql = "select * from " + _tableName + ";";
    query.exec(sql);
    while (query.next()) {
        QList<QStandardItem*> items;
        for (int i = 0; i < count; ++i) {
            QStandardItem* item = new QStandardItem(query.value(i).toString());
            item->setTextAlignment(Qt::AlignCenter);
            items.append(item);
        }
        model->appendRow(items);
    }

    ui->statusBar->showMessage("set data...");
    page->setModel(model);
    page->resizeColumnsToContents();
    page->setEditTriggers(QTableView::NoEditTriggers);
    ui->tables->setCurrentIndex(
        ui->tables->addTab(
            page, QIcon(":/icon/table.png"), _tableName));
    ui->stateArea->appendPlainText("成功！\n");
    return;
}

void AdminMainWindow::closeOneTable(int _currentIndex)
{
    QString tableName = ui->tables->tabText(_currentIndex);
    if (map->contains(tableName) && (*map)[tableName]) {
        (*map)[tableName] = false;
        ui->tables->removeTab(_currentIndex);
    }
    return;
}

void AdminMainWindow::removeTuples()
{
}

void AdminMainWindow::reconnectDB()
{
    dbSQL = DBConnector::ConnectDB();
}

void AdminMainWindow::showMovie()
{
    openOneTable("movie");
}

void AdminMainWindow::addMovie(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }

    QString movie;
    dlgAddMovie *addMovie = new dlgAddMovie();
    if(addMovie->exec() == QDialog::Accepted){
        movie = addMovie->getMovie();
        qDebug()<<movie<<endl;
    }

    QSqlQuery query(*dbSQL);
    QString sql = "call addMovie(" + movie+");";
    query.exec(sql);
}

void AdminMainWindow::removeMovie(){

}

void AdminMainWindow::modifyMovie(){

}

void AdminMainWindow::showLabel()
{
    openOneTable("label");
}

void AdminMainWindow::showMovieLabel()
{
    openOneTable("movielabel");
}

void AdminMainWindow::showHall()
{
    openOneTable("hall");
}

void AdminMainWindow::showTimeLine()
{
    openOneTable("timeline");
}

void AdminMainWindow::showPlayState()
{
    openOneTable("playstate");
}

void AdminMainWindow::showTicket()
{
    openOneTable("ticket");
}

void AdminMainWindow::showTicketState()
{
    openOneTable("ticketstate");
}

void AdminMainWindow::showStage()
{
    openOneTable("stage");
}

void AdminMainWindow::clearHistory()
{
    ui->stateArea->clear();
}

void AdminMainWindow::saveHistory()
{
    // 保存到文件
}


