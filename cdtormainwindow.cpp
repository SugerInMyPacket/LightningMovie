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
    ui->labStage->setText(LAB_STAGE);
    ui->labReturn->setText(LAB_STAGE);
    ui->labRow->setText(LAB_ROW);
    ui->labRow1->setText(LAB_ROW);
    ui->labColumn->setText(LAB_COLUMN);
    ui->labColumn1->setText(LAB_COLUMN);
    ui->btnSell->setText(BTN_SELL);
    ui->btnPrint->setText(BTN_PRINT);
    ui->btnReturn->setText(BTN_BACK);
    ui->btnSearch->setText(BTN_FIND);
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
    connect(ui->btnSell,SIGNAL(clicked()),this,SLOT(SellTickets()));
    connect(ui->btnReturn,SIGNAL(clicked()),this,SLOT(BackTickets()));
}

void CdtorMainWindow::OpenMovieTable(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_OPEN);
        return;
    }

    if(openMovie)return;

    QSqlQuery query(*dbSQL);
    const QString sql = "select * from movie order by movieName,movieDirector,movieStar";
    query.prepare(sql);
    if(!query.exec() || !(query.lastError().type() == QSqlError::NoError) ){
        QString error = "error code: "+ query.lastError().nativeErrorCode();
        error += "\nerror message: " + query.lastError().text();
        QMessageBox::critical(this,ERR_DB_QUERY,error);
        return;
    }

    QStringList header = GetHeader("movie");
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

void CdtorMainWindow::ShowTickets(){

    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_OPEN);
        return;
    }

    QString stageNumber = ui->edtStage->text();

    QSqlQuery query(*dbSQL);
    const QString sql= "select sellState,hallRow,hallColumn from ticket where stageNum = ? order by hallRow,hallColumn";
    query.prepare(sql);
    query.addBindValue(stageNumber);
    if(query.exec()){
        QTableView *page = new QTableView();
        QStandardItemModel *model = new QStandardItemModel();
        connect(page,SIGNAL(clicked(const QModelIndex &)),this,
                SLOT(SelectSeat(const QModelIndex &)));
        QList<QStandardItem*> items;
        int lastRow = 1;
        while (query.next()) {
            int row = query.value(1).toInt();
            if(row != lastRow){
                model->appendRow(items);
                items.clear();
                lastRow = row;
            }
            QStandardItem* item = new QStandardItem();
            item->setTextAlignment(Qt::AlignCenter);
            item->setFont(*font);
            const QString state = query.value(0).toString();
            if(state == "ye"){
                item->setBackground(QBrush(QColor(255,0,0)));
            }else if(state == "no"){
                item->setBackground(QBrush(QColor(0,255,0)));
            }else if(state == "ba"){
                item->setBackground(QBrush(QColor(0,0,255)));
            }
            items.append(item);
        }
        model->appendRow(items);

        QString title = "stage"+stageNumber+".seats";
        page->setModel(model);
        page->resizeColumnsToContents();
        page->setEditTriggers(QTableView::NoEditTriggers);
        ui->tables->setCurrentIndex(
            ui->tables->addTab(
                page, QIcon(":/icon/table.png"), title));

    }else{
        QString error = "errorCode: " + query.lastError().nativeErrorCode();
        error += ("\nerrorMessage: " + query.lastError().text());
        QMessageBox::critical(this, ERR_DB_QUERY, error);
        return;
    }
}

void CdtorMainWindow::SellTickets(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_OPEN);
        return;
    }

    const QString stageNumber  = ui->edtStage->text();
    const QString hallRow = ui->edtRow->text();
    const QString hallColumn = ui->edtColumn->text();

    if(stageNumber.length() == 0 ||
        hallRow.length() == 0||
        hallColumn.length() == 0)
        return;

    QString sql= "call sellTicket(?,?,?,@flag);";

    dbSQL->transaction();
    QSqlQuery query(*dbSQL);
    query.prepare(sql);
    query.bindValue(0,stageNumber);
    query.bindValue(1,hallColumn);
    query.bindValue(2,hallRow);
    if(query.exec() && query.lastError().type() == QSqlError::NoError){
        sql = "select @flag;";
        query.prepare(sql);
        query.exec();
        query.next();
        int flag = query.value(0).toInt();
        dbSQL->commit();
        if(flag == 1){
            QString information ="stage: "+stageNumber;
            information+="\nhallRow: "+ hallRow;
            information += "\nhallCoumun: "+hallColumn;
            QMessageBox::information(this,"购票成功",information);
        }else{
            QString error = "此票已售出！";
            QMessageBox::critical(this,"购票失败",error);
        }
        return;
    }else{
        dbSQL->rollback();
        QString error = "Error Code: "+query.lastError().nativeErrorCode();
        error += "\nError Message: "+query.lastError().text();
        QMessageBox::critical(this,ERR_DB_QUERY,error);
        return;
    }
}

void CdtorMainWindow::BackTickets(){
    if(dbSQL == nullptr){
        QMessageBox::critical(this,ERR_DB_OPEN,ERR_DB_OPEN);
        return;
    }

    const QString stageNumber  = ui->edtReturn->text();
    const QString hallRow = ui->edtRow1->text();
    const QString hallColumn = ui->edtColumn1->text();

    if(stageNumber.length() == 0 ||
        hallRow.length() == 0||
        hallColumn.length() == 0)
        return;

    QString sql= "call backTicket(?,?,?,@flag);";

    dbSQL->transaction();
    QSqlQuery query(*dbSQL);
    query.prepare(sql);
    query.bindValue(0,stageNumber);
    query.bindValue(1,hallColumn);
    query.bindValue(2,hallRow);
    if(query.exec() && query.lastError().type() == QSqlError::NoError){
        sql = "select @flag;";
        query.prepare(sql);
        query.exec();
        query.next();
        int flag = query.value(0).toInt();
        dbSQL->commit();
        if(flag == 1){
            QString information ="stage: "+stageNumber;
            information+="\nhallRow: "+ hallRow;
            information += "\nhallCoumun: "+hallColumn;
            QMessageBox::information(this,"退票成功",information);
        }else{
            QString error = "此票未售出！";
            QMessageBox::critical(this,"退票失败",error);
        }
        return;
    }else{
        dbSQL->rollback();
        QString error = "Error Code: "+query.lastError().nativeErrorCode();
        error += "\nError Message: "+query.lastError().text();
        QMessageBox::critical(this,ERR_DB_QUERY,error);
        return;
    }
}

void CdtorMainWindow::SelectMovie(const QModelIndex &index){
    if(index.column()<=0){
        QString movie = index.data().toString();
        ui->edtCondition->setText(movie);
    }
}

void CdtorMainWindow::SelectStage(const QModelIndex &index){
    if(index.column()<=0){
        ui->edtStage->setText(index.data().toString());
        ShowTickets();
    }
}

void CdtorMainWindow::SelectSeat(const QModelIndex &index){
    ui->edtRow->setText(QString::number(index.row()+1));
    ui->edtColumn->setText(QString::number(index.column()+1));
}

void CdtorMainWindow::DisplayQuery(QSqlQuery &_query, QStringList &_TableHeader, QString _TableName){
    QTableView *page = new QTableView();
    QStandardItemModel *model = new QStandardItemModel();

    if(_TableName == "movie"){
        connect(page,SIGNAL(clicked(const QModelIndex &)),this,
                SLOT(SelectMovie(const QModelIndex &)));
    }else if(_TableName.indexOf('.') == 5){
        connect(page,SIGNAL(doubleClicked(const QModelIndex &)),this,
                SLOT(SelectStage(const QModelIndex &)));
    }

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
    if(_TableName.length() == 0) return QStringList();
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

