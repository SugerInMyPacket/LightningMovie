#ifndef ADMINMAINWINDOW_H
#define ADMINMAINWINDOW_H

#include <qboxlayout.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qtableview.h>
#include <qtabbar.h>
#include <qsplitter.h>
#include <qlabel.h>
#include <qstandarditemmodel.h>
#include <QStandardItem>
#include <qmap.h>
#include <qstringlist.h>
#include <qdebug.h>
#include <qicon.h>
#include <qdatetime.h>
#include "dbconnector.h"
#include "stringsource.h"
#include "dlgaddmovie.h"


namespace Ui {
class AdminMainWindow;
}

class AdminMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit AdminMainWindow(QWidget* parent = nullptr);
    ~AdminMainWindow();
private:
    QDateTime datetime;
    QSqlDatabase *dbSQL;
    QMap<QString, bool> *map;
    bool showDBTree;
    QFont *font;
private:
    void initializeUI();
    void initializeMap();
    void openOneTable(const QString _tableName);
    void Connect_Signal_Slot();
private slots:
    void showMovie();
    void addMovie();
    void removeMovie();
    void modifyMovie();

    void showLabel();
    void showMovieLabel();
    void showTimeLine();
    void showHall();
    void showPlayState();
    void showTicket();
    void showTicketState();
    void showStage();
    void closeOneTable(int _currentIndex);
    void removeTuples();
    void reconnectDB();
    void clearHistory();
    void saveHistory();
private:
    Ui::AdminMainWindow* ui;
};

#endif // ADMINMAINWINDOW_H
