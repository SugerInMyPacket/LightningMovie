#ifndef CDTORMAINWINDOW_H
#define CDTORMAINWINDOW_H

#include <QMainWindow>
#include <qmessagebox.h>
#include <qtableview.h>
#include <qtabwidget.h>
#include <qstandarditemmodel.h>
#include <qdebug.h>
#include "stringsource.h"
#include "dbconnector.h"


namespace Ui {
class CdtorMainWindow;
}

class CdtorMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit CdtorMainWindow(QWidget* parent = nullptr);
    ~CdtorMainWindow();

private:
    QSqlDatabase *dbSQL;
    QFont *font;
    bool openMovie;
    void DisplayQuery(QSqlQuery &_query, QStringList &_TableHeader, QString _TableName);
    QStringList GetHeader(QString _TableName);
    void ConnectSS();
private slots:
    void ConnectDB();
    void OpenMovieTable();
    void CloseOneTable(int _currentIndex);
    void FindStage();
    void SellTickets();
    void BackTickets();
    void ShowTickets();
    void SelectMovie(const QModelIndex &index);
    void SelectStage(const QModelIndex &index);
    void SelectSeat(const QModelIndex &index);
private:
    Ui::CdtorMainWindow* ui;
};

#endif // CDTORMAINWINDOW_H
