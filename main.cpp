#include "adminmainwindow.h"
#include "cdtormainwindow.h"
#include "dlglogin.h"
#include "dlgregister.h"
#include <QApplication>

extern bool permissionFlag;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    DlgLogin *login = new DlgLogin();
    if(login->exec() == QDialog::Accepted){
        if(permissionFlag){
            AdminMainWindow *admin = new AdminMainWindow;
            admin->show();
        }else{
            CdtorMainWindow *cdt = new CdtorMainWindow;
            cdt->show();
        }
    }else{
        exit(0);
    }
    return a.exec();
}
