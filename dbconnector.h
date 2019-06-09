#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include "stringsource.h"
//#include <QWidget>
#include <qmessagebox.h>
#include <qsqldatabase.h>
#include <qsqlerror.h>
#include <qstring.h>
#include <qsqlquery.h>


class DBConnector {
private:
  static QSqlDatabase dbSQL;
public:
  DBConnector();
  virtual ~DBConnector() = default;
  static QSqlDatabase* ConnectDB();
};

#endif // DBCONNECTOR_H
