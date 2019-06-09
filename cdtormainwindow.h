#ifndef CDTORMAINWINDOW_H
#define CDTORMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class CdtorMainWindow;
}

class CdtorMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit CdtorMainWindow(QWidget* parent = nullptr);
    ~CdtorMainWindow();

private:
    Ui::CdtorMainWindow* ui;
};

#endif // CDTORMAINWINDOW_H
