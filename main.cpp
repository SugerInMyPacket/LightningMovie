#include "adminmainwindow.h"
#include "cdtormainwindow.h"
#include "dlglogin.h"
#include "dlgregister.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
//   AdminMainWindow *admin = new AdminMainWindow;
//   admin->show();

    CdtorMainWindow *cdt = new CdtorMainWindow;
    cdt->show();
    return a.exec();
}
