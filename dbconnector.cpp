#include "dbconnector.h"

extern const QString SQL_DB_TYPE;
extern const QString SQL_DB_HOST;
extern const int SQL_DB_PORT;
extern const QString SQL_DB_USER;
extern const QString SQL_DB_PSWD;
extern const QString SQL_DB_NAME;
extern const QString ERR_DB_OPEN;

DBConnector::DBConnector(){
    // we need do nothing here.
}

QSqlDatabase* DBConnector::ConnectDB(){
    if(dbSQL.open()){
        return &dbSQL;
    }
    dbSQL = QSqlDatabase::addDatabase(SQL_DB_TYPE);
    dbSQL.setHostName(SQL_DB_HOST);
    dbSQL.setPort(SQL_DB_PORT);
    dbSQL.setUserName(SQL_DB_USER);
    dbSQL.setPassword(SQL_DB_PSWD);
    dbSQL.setDatabaseName(SQL_DB_NAME);
    if (dbSQL.open()) {
        return &dbSQL;
    }else {
        QString error = dbSQL.lastError().text();
        QMessageBox::warning(nullptr,ERR_DB_OPEN,error);
         return nullptr;
    }
}

QSqlDatabase DBConnector::dbSQL;
