#ifndef DLGADDMOVIE_H
#define DLGADDMOVIE_H

#include <QDialog>
#include "stringsource.h"


namespace Ui {
class dlgAddMovie;
}

class dlgAddMovie : public QDialog
{
Q_OBJECT

public:
explicit dlgAddMovie(QWidget *parent = nullptr);
~dlgAddMovie();

private:
    QString strMovie;
public:
    QString getMovie();
private slots:
    void on_btnOkay_clicked();

private:
Ui::dlgAddMovie *ui;
};

#endif // DLGADDMOVIE_H
