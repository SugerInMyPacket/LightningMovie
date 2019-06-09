#include "dlgaddmovie.h"
#include "ui_dlgaddmovie.h"

dlgAddMovie::dlgAddMovie(QWidget *parent) :
QDialog(parent),
ui(new Ui::dlgAddMovie)
{
    ui->setupUi(this);
    ui->labMovieName->setText(MOVIE_NAME);
    ui->labMovieDirector->setText(MOVIE_DIRECTOR);
    ui->labMovieStar->setText(MOVIE_STAR);
    ui->btnOkay->setText(BTN_OKAY);
}

dlgAddMovie::~dlgAddMovie()
{
    delete ui;
}


QString dlgAddMovie::getMovie(){
    //15是电影编号的位数
    srand(static_cast<unsigned>(time(nullptr)));
    char id[16];
    id[0] = rand()%26 + 97;
    for (size_t i= 1; i<15; ++i) {
        do{
            id[i] = rand()%26 + 97;
        }while(id[i] == id[i-1]);
    }
    id[15] = '\0'; //不能省略
    QString strMovieId(id);
    strMovie = "'"+strMovieId;
    strMovie += "','" + ui->edtMovieName->text();
    strMovie += "','" + ui->edtMovieDirector->text();
    strMovie += "','" + ui->edtMovieStar->text()+"'";
    return strMovie;
}

//ligbnkczmdabzdy
//ybcfwektdpfakwa

//QDialog *dlgData = new QDialog(this);
//dlgData->setFont(*font);
//QPushButton *btnOkay = new QPushButton(BTN_OKAY);
//QLabel *labMovieName = new QLabel(MOVIE_NAME);
//QLineEdit *edtMovieName = new QLineEdit();
//QLabel *labDirector = new QLabel(MOVIE_DIRECTOR);
//QLineEdit *edtDirector = new QLineEdit();
//QLabel *labStar = new QLabel(MOVIE_STAR);
//QLineEdit *edtStar = new QLineEdit();
//QGridLayout *grid = new QGridLayout();
//grid->addWidget(labMovieName,0,0,1,1);
//grid->addWidget(edtMovieName,0,1,1,2);
//grid->addWidget(labDirector,1,0,1,2);
//grid->addWidget(edtDirector,1,1,1,2);
//grid->addWidget(labStar,2,0,1,2);
//grid->addWidget(edtStar,2,1,1,2);
//grid->addWidget(btnOkay,3,1,1,1);
//dlgData->setLayout(grid);
////    dlgData->setMaximumSize(dlgData->size());
////    dlgData->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
////    dlgData->show();
//connect(btnOkay,SIGNAL(clicked()),dlgData,accepted);
//if(dlgData->exec()==QDialog::Accepted){
//    qDebug()<<dlgData->result();
//}

void dlgAddMovie::on_btnOkay_clicked()
{

}
