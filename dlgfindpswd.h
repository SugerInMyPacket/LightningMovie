#ifndef DLGFINDPSWD_H
#define DLGFINDPSWD_H

#include <QDialog>

namespace Ui {
class DlgFindPswd;
}

class DlgFindPswd : public QDialog {
    Q_OBJECT

public:
    explicit DlgFindPswd(QWidget* parent = nullptr);
    ~DlgFindPswd();

private:
    Ui::DlgFindPswd* ui;
};

#endif // DLGFINDPSWD_H
