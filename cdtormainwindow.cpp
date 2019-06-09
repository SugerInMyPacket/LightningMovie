#include "cdtormainwindow.h"
#include "ui_cdtormainwindow.h"

CdtorMainWindow::CdtorMainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::CdtorMainWindow)
{
    ui->setupUi(this);
}

CdtorMainWindow::~CdtorMainWindow()
{
    delete ui;
}
