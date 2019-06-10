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
    connect(ui->actAddLabel,SIGNAL(triggered()),this,SLOT(addLabel()));
    connect(ui->actRemoveLabel,SIGNAL(triggered()),this,SLOT(removeLabel()));

    connect(ui->actShowMovieLabel, SIGNAL(triggered()), this, SLOT(showMovieLabel()));
    connect(ui->actAddMovieLabel, SIGNAL(triggered()), this,SLOT(addMovieLabel()));
    connect(ui->actRemoveMovieLabel, SIGNAL(triggered()), this,SLOT(removeMovieLabel()));

    connect(ui->actShowHall, SIGNAL(triggered()), this, SLOT(showHall()));
    connect(ui->actNewHall, SIGNAL(triggered()), this,SLOT(addHall()));
    connect(ui->actRemoveHall,SIGNAL(triggered()), this,SLOT(removeHall()));
    connect(ui->actRepairHall,SIGNAL(triggered()), this,SLOT(modifyHall()));

    connect(ui->actShowTime, SIGNAL(triggered()), this, SLOT(showTimeLine()));
    connect(ui->actAddTime,SIGNAL(triggered()), this,SLOT(addTimeLine()));
    connect(ui->actRemoveTime,SIGNAL(triggered()), this,SLOT(removeTimeLine()));
    connect(ui->actModifyTime,SIGNAL(triggered()), this,SLOT(modifyTimeLine()));


    connect(ui->actShowPlayState,SIGNAL(triggered()),this,SLOT(showPlayState()));
    connect(ui->actAddPlayState,SIGNAL(triggered()),this,SLOT(addPlayState()));
    connect(ui->actRemovePlayState,SIGNAL(triggered()),this,SLOT(removePlayState()));

    connect(ui->actShowTicket, SIGNAL(triggered()), this, SLOT(showTicket()));

   
    
    connect(ui->actClearHistory, SIGNAL(triggered()), this, SLOT(clearHistory()));
    connect(ui->actSaveHistory, SIGNAL(triggered()), this, SLOT(saveHistory()));
    connect(ui->actRemoveCurrent,SIGNAL(triggered()), this,SLOT(removeCurrentTuple()));
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
    connect(page,SIGNAL(clicked(const QModelIndex &)),this,SLOT(selectCurrentTuple(const QModelIndex &)));
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

void AdminMainWindow::selectCurrentTuple(const QModelIndex &index){
    int row = index.row();
    int column = index.column();
    int n = index.model()->columnCount();


}

void AdminMainWindow::removeCurrentTuple(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    if(tableName.length() <= 0 || currentKey.length()<=0){
        return;
    }
    QSqlQuery query(*dbSQL);
    QString sql = "call remove"+tableName+"('"+currentKey+"');";
    dbSQL->transaction();
    query.prepare(sql); // 防止注入sql攻击
    if(query.exec() && query.lastError().type() == QSqlError::NoError){
        dbSQL->commit();  //成功则提交
    }else {
        dbSQL->rollback();  //失败则回滚
        QString error = "errorCode: " + query.lastError().nativeErrorCode();
        error += ("\nerrorMessage: " + query.lastError().text());
        QMessageBox::critical(this, ERR_DB_QUERY, error);
    }
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
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnOkay = new QPushButton(BTN_OKAY);
    connect(btnOkay, SIGNAL(clicked()), dlgData, SLOT(accept()));
    QLabel *labMovieName = new QLabel(MOVIE_NAME);
    QLineEdit *edtMovieName = new QLineEdit();
    QLabel *labDirector = new QLabel(MOVIE_DIRECTOR);
    QLineEdit *edtDirector = new QLineEdit();
    QLabel *labStar = new QLabel(MOVIE_STAR);
    QLineEdit *edtStar = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labMovieName,0,0,1,1);
    grid->addWidget(edtMovieName,0,1,1,2);
    grid->addWidget(labDirector,1,0,1,2);
    grid->addWidget(edtDirector,1,1,1,2);
    grid->addWidget(labStar,2,0,1,2);
    grid->addWidget(edtStar,2,1,1,2);
    grid->addWidget(btnOkay,3,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec()==QDialog::Accepted){
        srand(static_cast<unsigned>(time(nullptr)));
        char id[16];
        id[0] = rand()%26 + 97;
        for (size_t i= 1; i<15; ++i) {
            do{
                id[i] = rand()%26 + 97;
            }while(id[i] == id[i-1]);
        }
        id[15] = '\0'; //不能省略
        QString strMovieId(id);
        QString strMovie = "'"+strMovieId;
        strMovie += "','" + edtMovieName->text();
        strMovie += "','" + edtDirector->text();
        strMovie += "','" + edtStar->text()+"'";
        QSqlQuery query(*dbSQL);
        dbSQL->transaction(); // 开启一个事务
        QString sql = "call addMovie(" + strMovie+");";
        query.prepare(sql); // 防止注入sql攻击
        if(query.exec() && query.lastError().type() == QSqlError::NoError){
            dbSQL->commit();  //成功则提交
        }else {
            dbSQL->rollback();  //失败则回滚
            QString error = "errorCode: " + query.lastError().nativeErrorCode();
            error += ("\nerrorMessage: " + query.lastError().text());
            QMessageBox::critical(this, ERR_DB_QUERY, error);
        }
    }
}

void AdminMainWindow::removeMovie(){

}

void AdminMainWindow::modifyMovie(){

}

void AdminMainWindow::showLabel()
{
    openOneTable("label");
}

void AdminMainWindow::addLabel(){
    if(dbSQL == nullptr){
            QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
            return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnOkay = new QPushButton(BTN_OKAY);
    connect(btnOkay, SIGNAL(clicked()), dlgData, SLOT(accept()));

    QLabel *labLabelId=new QLabel(LABEL_ID);    //char(15)
    QLineEdit *edtLabelId=new QLineEdit();
    QLabel *labLabelDsb=new QLabel(LABEL_DSB);  //nvarchar(100)
    QLineEdit *edtLabelDsb=new QLineEdit();
    dlgData->setFont(*font);
    QGridLayout *grid = new QGridLayout();
    grid->addWidget(labLabelId,0,0,1,1);
    grid->addWidget(edtLabelId,0,1,1,2);
    grid->addWidget(labLabelDsb,1,0,1,1);
    grid->addWidget(edtLabelDsb,1,1,1,2);
    grid->addWidget(btnOkay,2,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec()==QDialog::Accepted){
        QString strLabel="'"+edtLabelId->text();
        strLabel+="','"+edtLabelDsb->text()+"'";
        QSqlQuery query(*dbSQL);
        dbSQL->transaction(); // 开启一个事务
        QString sql = "call addLabel(" + strLabel+");";

        query.prepare(sql); // 防止注入sql攻击
        if(query.exec() && query.lastError().type() == QSqlError::NoError){
            dbSQL->commit();  //成功则提交
        }else {
            dbSQL->rollback();  //失败则回滚
            QString error = "errorCode: " + query.lastError().nativeErrorCode();
            error += ("\nerrorMessage: " + query.lastError().text());
            QMessageBox::critical(this, ERR_DB_QUERY, error);
        }
    }
}

void AdminMainWindow::removeLabel(){

}

void AdminMainWindow::showMovieLabel()
{
    openOneTable("movielabel");
}


void AdminMainWindow::addMovieLabel(){

}


void AdminMainWindow::removeMovieLabel(){

}

void AdminMainWindow::showHall()
{
    openOneTable("hall");
}

void AdminMainWindow::addHall(){
   if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnOkay = new QPushButton(BTN_OKAY);
    connect(btnOkay, SIGNAL(clicked()), dlgData, SLOT(accept()));
    QLabel *labHallId = new QLabel(HALL_ID);
    QLabel *labHallName = new QLabel(HALL_NAME);
    QLabel *labHallRow = new QLabel(HALL_ROW);
    QLabel *labHallColumn = new QLabel(HALL_COLUMN);
    QLineEdit *edtHallId = new QLineEdit();
    QLineEdit *edtHallName = new QLineEdit();
    QLineEdit *edtHallRow = new QLineEdit();
    QLineEdit *edtHallColumn = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labHallId,0,0,1,1);
    grid->addWidget(edtHallId,0,1,1,2);
    grid->addWidget(labHallName,1,0,1,2);
    grid->addWidget(edtHallName,1,1,1,2);
    grid->addWidget(labHallRow,2,0,1,2);
    grid->addWidget(edtHallRow,2,1,1,2);
    grid->addWidget(labHallColumn,3,0,1,2);
    grid->addWidget(edtHallColumn,3,1,1,2);
    grid->addWidget(btnOkay,4,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec()==QDialog::Accepted){
//        QString strHall = edtHallId->text();
        QString strHall= edtHallId->text();
        strHall+=",'"+edtHallName->text();
        strHall+="',"+edtHallColumn->text();
        strHall+=","+edtHallRow->text();
         QSqlQuery query(*dbSQL);
         dbSQL->transaction();   // 开启一个事务
         QString sql = "call addHall("+strHall+");";
         qDebug()<<sql;
         query.prepare(sql); // 防止注入sql攻击
         if(query.exec() && query.lastError().type() == QSqlError::NoError){
                    dbSQL->commit();  //成功则提交
          }else {
               dbSQL->rollback();  //失败则回滚
               QString error = "errorCode: " + query.lastError().nativeErrorCode();
               error += ("\nerrorMessage: " + query.lastError().text());
               QMessageBox::critical(this, ERR_DB_QUERY, error);
        }
    }
}

void AdminMainWindow::removeHall(){

}

void AdminMainWindow::modifyHall(){

}

void AdminMainWindow::showTimeLine()
{
    openOneTable("timeline");
}

void AdminMainWindow::showPlayState()
{
    openOneTable("playstate");
}

void AdminMainWindow::addPlayState(){

}

void AdminMainWindow::removePlayState(){


}

void AdminMainWindow::addTimeLine(){

}
void AdminMainWindow::removeTimeLine(){

}
void AdminMainWindow::modifyTimeLine(){

}

void AdminMainWindow::showTicket()
{
    openOneTable("ticket");
}

void AdminMainWindow::addTicketState(){

}
void AdminMainWindow::removeTicketState(){

}

void AdminMainWindow::showTicketState()
{
    openOneTable("ticketstate");
}

void AdminMainWindow::showStage()
{
    openOneTable("stage");
}

void AdminMainWindow::addStage(){

}
void AdminMainWindow::modifyStage(){

}
void AdminMainWindow::removeStage(){

}

void AdminMainWindow::clearHistory()
{
    ui->stateArea->clear();
}

void AdminMainWindow::saveHistory()
{
    // 保存到文件
}


