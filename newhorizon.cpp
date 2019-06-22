#include "newhorizon.h"
#include "ui_newhorizon.h"
#include <QMessageBox>

NewHorizon::NewHorizon(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewHorizon)
{
    ui->setupUi(this);
}

NewHorizon::~NewHorizon()
{
    delete ui;
}

void NewHorizon::on_pushButton_close_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exit", "Would you like to leave?", QMessageBox::Yes|QMessageBox::No);
          if(reply == QMessageBox::Yes){
              QApplication::quit();
          };
}



void NewHorizon::on_actionRead_File_triggered()
{
    close();
    statcalc_window = new StatCalc(this);
    statcalc_window->show();
}

void NewHorizon::on_actionAbout_triggered()
{
if(!about_window){
    about_window = new About (this);
    about_window->show();
   }else {
    about_window->show();
   }

}
