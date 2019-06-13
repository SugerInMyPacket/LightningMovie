#ifndef CDTORMAINWINDOW_H
#define CDTORMAINWINDOW_H

#include <QMainWindow>
#include <qmessagebox.h>
#include <qtableview.h>
#include <qtabwidget.h>
#include <qstandarditemmodel.h>
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
    bool openMovie;

    void ConnectSS();
private slots:
    void ConnectDB();
    void OpenMovieTable();
    void CloseOneTable(int _currentIndex);
    void FindStage();
private:
    Ui::CdtorMainWindow* ui;
};

#endif // CDTORMAINWINDOW_H
