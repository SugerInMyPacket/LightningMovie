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

    connect(ui->actShowStage,SIGNAL(triggered()),this,SLOT(showStage()));
    connect(ui->actAddStage,SIGNAL(triggered()),this,SLOT(addStage()));
    connect(ui->actRemoveStage,SIGNAL(triggered()),this,SLOT(removeStage()));
    connect(ui->actModifyStage,SIGNAL(triggered()),this,SLOT(modifyStage()));

    connect(ui->actShowPlayState,SIGNAL(triggered()),this,SLOT(showPlayState()));
    connect(ui->actAddPlayState,SIGNAL(triggered()),this,SLOT(addPlayState()));
    connect(ui->actRemovePlayState,SIGNAL(triggered()),this,SLOT(removePlayState()));

    connect(ui->actShowTicket, SIGNAL(triggered()), this, SLOT(showTicket()));
    connect(ui->actAddSellState,SIGNAL(triggered()), this, SLOT(addTicketState()));
    connect(ui->actRemoveSellState,SIGNAL(triggered()),this,SLOT(removeTicketState()));
   
    
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
    QString sql = "show columns from ?;";
    QTableView* page = new QTableView;
    connect(page,SIGNAL(clicked(const QModelIndex &)),this,SLOT(selectCurrentTuple(const QModelIndex &)));
    QStandardItemModel* model = new QStandardItemModel;
    query.prepare(sql);
    query.addBindValue(_tableName);
    query.exec();

    ui->statusBar->showMessage("set tablel headers...");
    int count = query.size();
    QStringList headers;
    while (query.next()) {
        headers.append(query.value(0).toString());
    }
    model->setHorizontalHeaderLabels(headers);

    ui->statusBar->showMessage("select data...");
    sql = "select * from ?;";
    query.prepare(sql);
    query.addBindValue(_tableName);
    query.exec();
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
        QString strMovieName = edtMovieName->text();
        QString strMovieDirector = edtDirector->text();
        QString strMovieStar = edtStar->text();
        QSqlQuery query(*dbSQL);
        dbSQL->transaction(); // 开启一个事务
        QString sql = "call addMovie(?,?,?);";
        query.prepare(sql); // 防止注入sql攻击
        query.bindValue(0,strMovieName);
        query.bindValue(1,strMovieDirector);
        query.bindValue(2,strMovieStar);
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
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnRemove = new QPushButton(BTN_OKAY);
    connect(btnRemove,SIGNAL(clicked()),dlgData,SLOT(accept()));
    QLabel *labMovieName = new QLabel(MOVIE_NAME);
    QLineEdit *edtMovieName = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labMovieName,0,0,1,1);
    grid->addWidget(edtMovieName,0,1,1,2);
    grid->addWidget(btnRemove,1,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec() == QDialog::Accepted){
        QString movieName = edtMovieName->text();
        QString sql = "call removeMovie(?);";
        dbSQL->transaction();
        QSqlQuery query(*dbSQL);
        query.prepare(sql);
        query.addBindValue(movieName);
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

void AdminMainWindow::modifyMovie(){
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
        QString strMovieName = edtMovieName->text();
        QString strMovieDirector = edtDirector->text();
        QString strMovieStar = edtStar->text();
        QString sql;
        if(!strMovieDirector.isEmpty()){
            sql = "call modifyDirector(?,?);";
            dbSQL->transaction(); // 开启一个事务
            QSqlQuery query(*dbSQL);
            query.prepare(sql);
            query.bindValue(0,strMovieName);
            query.bindValue(1,strMovieDirector);
            if(query.exec() && query.lastError().type() == QSqlError::NoError){
                dbSQL->commit();  //成功则提交
            }else {
                dbSQL->rollback();  //失败则回滚
                QString error = "errorCode: " + query.lastError().nativeErrorCode();
                error += ("\nerrorMessage: " + query.lastError().text());
                QMessageBox::critical(this, ERR_DB_QUERY, error);
            }
        }

        if(!strMovieStar.isEmpty()){
            sql = "call modifyStar(?,?);";
            dbSQL->transaction(); // 开启一个事务
            QSqlQuery query(*dbSQL);
            query.prepare(sql);
            query.bindValue(0,strMovieName);
            query.bindValue(1,strMovieStar);
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
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnRemove = new QPushButton(BTN_OKAY);
    connect(btnRemove,SIGNAL(clicked()),dlgData,SLOT(accept()));
    QLabel *labLabelId = new QLabel(LABEL_ID);
    QLineEdit *edtLabelId = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labLabelId,0,0,1,1);
    grid->addWidget(edtLabelId,0,1,1,2);
    grid->addWidget(btnRemove,1,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec() == QDialog::Accepted){
        QString labelId = edtLabelId->text();
        QString sql = "call removeLabel(?);";
        dbSQL->transaction();
        QSqlQuery query(*dbSQL);
        query.prepare(sql);
        query.addBindValue(labelId);
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

void AdminMainWindow::showMovieLabel()
{
    openOneTable("movielabel");
}

void AdminMainWindow::addMovieLabel(){
 if(dbSQL == nullptr){
          QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
          return;
      }
      QDialog *dlgData = new QDialog(this);
      QPushButton *btnOkay = new QPushButton(BTN_OKAY);
      connect(btnOkay, SIGNAL(clicked()), dlgData, SLOT(accept()));
      QLabel *LabMovieName=new QLabel(MOVIE_NAME);
      QLineEdit *edtMovieName=new QLineEdit();
      QLabel *LabLabelId=new QLabel(LABEL_ID);
      QLineEdit *edtLabelId=new QLineEdit();
      QGridLayout *grid = new QGridLayout();
      dlgData->setFont(*font);
      grid->addWidget(LabMovieName,0,0,1,1);
      grid->addWidget(edtMovieName,0,1,1,2);
      grid->addWidget(LabLabelId,1,0,1,1);
      grid->addWidget(edtLabelId,1,1,1,2);
      grid->addWidget(btnOkay,2,1,1,1);
      dlgData->setLayout(grid);
      if(dlgData->exec()==QDialog::Accepted){
          QString strMovieName = edtMovieName->text();
          QString strLabelId = edtLabelId->text();
          QSqlQuery query(*dbSQL);
          dbSQL->transaction(); // 开启一个事务
          QString sql = "call addMovieLabel(?,?);";
          query.prepare(sql); // 防止注入sql攻击
          query.bindValue(0,strMovieName);
          query.bindValue(1,strLabelId);
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

void AdminMainWindow::removeMovieLabel(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnRemove = new QPushButton(BTN_OKAY);
    connect(btnRemove,SIGNAL(clicked()),dlgData,SLOT(accept()));
    QLabel *labMovieName = new QLabel(MOVIE_NAME);
    QLineEdit *edtMovieName = new QLineEdit();
    QLabel *labLabelId = new QLabel(LABEL_ID);
    QLineEdit *edtLabelId = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labMovieName,0,0,1,1);
    grid->addWidget(edtMovieName,0,1,1,2);
    grid->addWidget(labLabelId,1,0,1,1);
    grid->addWidget(edtLabelId,1,1,1,2);
    grid->addWidget(btnRemove,2,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec() == QDialog::Accepted){
        QString strMovieName = edtMovieName->text();
        QString strLabelId = edtLabelId->text();
        QString sql = "call removeMovieLabel(?,?);";
        dbSQL->transaction();
        QSqlQuery query(*dbSQL);
        query.prepare(sql);
        query.bindValue(0,strMovieName);
        query.bindValue(1,strLabelId);
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
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnRemove = new QPushButton(BTN_OKAY);
    connect(btnRemove,SIGNAL(clicked()),dlgData,SLOT(accept()));
    QLabel *labHallId = new QLabel(HALL_ID);
    QLineEdit *edtHallId = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labHallId,0,0,1,1);
    grid->addWidget(edtHallId,0,1,1,2);
    grid->addWidget(btnRemove,1,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec() == QDialog::Accepted){
        QString labels = edtHallId->text();
        QString sql = "call removeHall(?);";
        dbSQL->transaction();
        QSqlQuery query(*dbSQL);
        query.prepare(sql);
        query.addBindValue(labels);
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

void AdminMainWindow::modifyHall(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnRemove = new QPushButton(BTN_OKAY);
    connect(btnRemove,SIGNAL(clicked()),dlgData,SLOT(accept()));
    QLabel *labHallId = new QLabel(HALL_ID);
    QLineEdit *edtHallId = new QLineEdit();
    QLabel *labHallName = new QLabel(HALL_NAME);
    QLineEdit *edtHallName = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labHallId,0,0,1,1);
    grid->addWidget(edtHallId,0,1,1,2);
    grid->addWidget(labHallName,1,0,1,1);
    grid->addWidget(edtHallName,1,1,1,2);
    grid->addWidget(btnRemove,2,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec() == QDialog::Accepted){
        QString hallId = edtHallId->text();
        QString hallName = edtHallName->text();
        QString sql = "call modifyHall(?,?);";
        dbSQL->transaction();
        QSqlQuery query(*dbSQL);
        query.prepare(sql);
        query.bindValue(0,hallId);
        query.bindValue(1,hallName);
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

void AdminMainWindow::showTimeLine()
{
    openOneTable("timeline");
}

void AdminMainWindow::addTimeLine(){

}

void AdminMainWindow::removeTimeLine(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnRemove = new QPushButton(BTN_OKAY);
    connect(btnRemove,SIGNAL(clicked()),dlgData,SLOT(accept()));

    QLabel *labTimeLine = new QLabel(TIME_NUMBER);
    QLineEdit *edtTimeLine = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labTimeLine,0,0,1,1);
    grid->addWidget(edtTimeLine,0,1,1,2);
    grid->addWidget(btnRemove,1,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec() == QDialog::Accepted){
        QString timeLine = edtTimeLine->text();
        QString sql = "call removeTimeLine('"+timeLine+"');";   //RemoveTimeLine存储过程未写
        dbSQL->transaction();
        QSqlQuery query(*dbSQL);
        query.prepare(sql);
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
void AdminMainWindow::modifyTimeLine(){

}

void AdminMainWindow::showPlayState()
{
    openOneTable("playstate");
}

void AdminMainWindow::addPlayState(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnOkay = new QPushButton(BTN_OKAY);
    connect(btnOkay, SIGNAL(clicked()), dlgData, SLOT(accept()));
    QLabel *labStateId = new QLabel(STATE_ID);
    QLineEdit *edtStateID = new QLineEdit();
    QLabel *labStateRow = new QLabel(STATE_DSB);
    QLineEdit *edtStateDsb = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labStateId,0,0,1,1);
    grid->addWidget(edtStateID,0,1,1,2);
    grid->addWidget(labStateRow,1,0,1,2);
    grid->addWidget(edtStateDsb,1,1,1,2);
    grid->addWidget(btnOkay,2,1,1,2);
    dlgData->setLayout(grid);
    if(dlgData->exec()==QDialog::Accepted){
        QString strPlayState="'"+edtStateID->text();
        strPlayState+="','"+edtStateDsb->text()+"'";
        QSqlQuery query(*dbSQL);
        dbSQL->transaction();   // 开启一个事务
        QString sql = "call addPlayState("+strPlayState+");";
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

void AdminMainWindow::removePlayState(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnRemove = new QPushButton(BTN_OKAY);
    connect(btnRemove,SIGNAL(clicked()),dlgData,SLOT(accept()));
    QLabel *labStateId = new QLabel(PLAYSTATE_ID);
    QLineEdit *edtStateId = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labStateId,0,0,1,1);
    grid->addWidget(edtStateId,0,1,1,2);
    grid->addWidget(btnRemove,1,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec() == QDialog::Accepted){
        QString state = edtStateId->text();
        state = state.remove(2,state.length()-2);
        QString sql = "call removePlayState('"+state+"');";
        dbSQL->transaction();
        QSqlQuery query(*dbSQL);
        query.prepare(sql);
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

void AdminMainWindow::showTicket()
{
    openOneTable("ticket");
}

void AdminMainWindow::addTicketState(){
if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnOkay = new QPushButton(BTN_OKAY);
    connect(btnOkay, SIGNAL(clicked()), dlgData, SLOT(accept()));
    QLabel *labTktStateId=new QLabel(TKT_STATE_ID);
    QLabel *labTktStateDsb=new QLabel(TKT_STATE_DSB);
    QLineEdit *edtTktStateId=new QLineEdit();
    QLineEdit *edtTktStateDsb=new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labTktStateId,0,0,1,1);
    grid->addWidget(edtTktStateId,0,1,1,2);
    grid->addWidget(labTktStateDsb,1,0,1,1);
    grid->addWidget(edtTktStateDsb,1,1,1,2);
    grid->addWidget(btnOkay,2,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec()==QDialog::Accepted){
        QString strTimeLine = "'" + edtTktStateId->text();
        strTimeLine += "','" + edtTktStateDsb->text()+"'";
        QSqlQuery query(*dbSQL);
        dbSQL->transaction(); // 开启一个事务
        QString sql = "call addTimeLine(" + strTimeLine+");";
        query.prepare(sql);  // 防止注入sql攻击
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
void AdminMainWindow::removeTicketState(){
if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnRemove = new QPushButton(BTN_OKAY);
    connect(btnRemove,SIGNAL(clicked()),dlgData,SLOT(accept()));
    QLabel *labTicketState = new QLabel(TICKET_STATE);
    QLineEdit *edtTicketState = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labTicketState,0,0,1,1);
    grid->addWidget(edtTicketState,0,1,1,2);
    grid->addWidget(btnRemove,1,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec() == QDialog::Accepted){
        QString ticketState = edtTicketState->text();
        QString sql = "call removeTicketState('"+ticketState+"');";
        dbSQL->transaction();
        QSqlQuery query(*dbSQL);
        query.prepare(sql);
        if(query.exec() && query.lastError().type() == QSqlError::NoError){
            dbSQL->commit();
        }else {
            dbSQL->rollback();
            QString error = "errorCode: " + query.lastError().nativeErrorCode();
            error += ("\nerrorMessage: " + query.lastError().text());
            QMessageBox::critical(this, ERR_DB_QUERY, error);
        }
    }
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
 if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnOkay = new QPushButton(BTN_OKAY);
    connect(btnOkay, SIGNAL(clicked()), dlgData, SLOT(accept()));
    QLabel *labHallId=new QLabel(HALL_ID);
    QLabel *labMovId=new QLabel(MOVIE_NAME);
    QLabel *labPrice=new QLabel(MOVIE_PRICE);
    QLabel *labStageNumber=new QLabel(STAGE_ID);
//    QLabel *labStateId=new QLabel(STATE_ID);
    QLabel *labTimeNum=new QLabel(TIMELINE_NUM);
    QLineEdit *edtHallId=new QLineEdit();
    QLineEdit *edtMovieName=new QLineEdit();
    QLineEdit *edtPrice=new QLineEdit();
    QLineEdit *edtStageNumber=new QLineEdit();
//    QLineEdit *edtStateId=new QLineEdit();
    QLineEdit *edtTimeNum=new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labHallId,0,0,1,1);
    grid->addWidget(edtHallId,0,1,1,2);
    grid->addWidget(labMovId,1,0,1,1);
    grid->addWidget(edtMovieName,1,1,1,2);
    grid->addWidget(labPrice,2,0,1,1);
    grid->addWidget(edtPrice,2,1,1,2);
    grid->addWidget(labStageNumber,3,0,1,1);
    grid->addWidget(edtStageNumber,3,1,1,2);
//    grid->addWidget(labStateId,4,0,1,1);
//    grid->addWidget(edtStateId,4,1,1,2);
    grid->addWidget(labTimeNum,5,0,1,1);
    grid->addWidget(edtTimeNum,5,1,1,2);
    grid->addWidget(btnOkay,6,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec()==QDialog::Accepted){
        QString strStage=edtHallId->text();
        strStage+=",'"+edtMovieName->text();
        strStage+="',"+edtPrice->text();
        strStage+=","+edtStageNumber->text();
//        strStage+=",'"+edtStateId->text()+"'";
        strStage+=","+edtTimeNum->text();

        QSqlQuery query(*dbSQL);
        dbSQL->transaction(); // 开启一个事务
        QString sql = "call addStage(" + strStage+");";
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
void AdminMainWindow::modifyStage(){

}
void AdminMainWindow::removeStage(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_DISCONNECT);
        return;
    }
    QDialog *dlgData = new QDialog(this);
    QPushButton *btnRemove = new QPushButton(BTN_OKAY);
    connect(btnRemove,SIGNAL(clicked()),dlgData,SLOT(accept()));
    QLabel *labHallId = new QLabel(HALL_ID);
    QLineEdit *edtHallId = new QLineEdit();
    QLabel *labTimeNum = new QLabel(TIMELINE_NUM);
    QLineEdit *edtTimeNum = new QLineEdit();
    QGridLayout *grid = new QGridLayout();
    dlgData->setFont(*font);
    grid->addWidget(labHallId,0,0,1,1);
    grid->addWidget(edtHallId,0,1,1,2);
    grid->addWidget(labTimeNum,1,0,1,1);
    grid->addWidget(edtTimeNum,1,1,1,2);
    grid->addWidget(btnRemove,2,1,1,1);
    dlgData->setLayout(grid);
    if(dlgData->exec() == QDialog::Accepted){
        QString stage = edtHallId->text()+","+edtTimeNum->text();
        QString sql = "call removeStage("+stage+");";
        dbSQL->transaction();
        QSqlQuery query(*dbSQL);
        query.prepare(sql);
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

void AdminMainWindow::clearHistory()
{
    ui->stateArea->clear();
}

void AdminMainWindow::saveHistory()
{
    // 保存到文件
}


