#include "dlgfindpswd.h"
#include "ui_dlgfindpswd.h"

DlgFindPswd::DlgFindPswd(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DlgFindPswd)
{
    ui->setupUi(this);
}

DlgFindPswd::~DlgFindPswd()
{
    delete ui;
}
