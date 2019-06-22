#include "results.h"
#include "ui_results.h"
#include "statcalc.h"
#include "internalrotation.h"

#include <QString>
#include <QTextEdit>
#include <QTextBrowser>
#include <QTemporaryFile>
#include <QTextStream>
#include <QIODevice>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

Results::Results(QWidget *parent) :
    QMainWindow(parent),
    ui_res(new Ui::Results)
{
    ui_res->setupUi(this);
    result_table = get_result(Ui::r_T, Ui::result, Ui::inner_result);
    ui_res->textEdit_result->setText(result_table);
    ui_res->statusbar_res->showMessage("Calculations complete");//Problem, no check is here
}

Results::~Results()
{

    delete ui_res;
}


void Results::on_pushButton_back_clicked()
{   Ui::r_T = 298.15;
    Ui::scal_fac =1;
    delete [] Ui::inner_result;
    Ui::counter = 0;
    close();//Issue, it's needed to back with 'cockies' and 'breadcrums'
    StatCalc *window = new StatCalc(this);
    window->show();
}

void Results::on_pushButton_quit_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exit",
"If you leave All data will be lost. Would you like to leave?", QMessageBox::Yes|QMessageBox::No|QMessageBox::Save);
          if(reply == QMessageBox::Yes){
              QApplication::quit();
          } else if (reply == QMessageBox::Save){
              QString res_filename = QFileDialog::getSaveFileName(this, tr("Save Results"),
              "/home", "All Files(*.*);;Text Files(*.txt);; PDF Files (*.pdf)");
              if(res_filename.isEmpty()){
                  return;
              }else{
                  QFile res_file(res_filename);
                  if(!res_file.open(QIODevice::WriteOnly)){
                      QMessageBox::information(this, tr("Unable to save file"), res_file.errorString());
                      return;
                  }
                  QTextStream res_out(&res_file);
                  res_out<<result_table;
                  res_file.close();
              }
              QApplication::quit();
          };
}

void Results::on_pushButton_writemode_clicked()
{

    if(!ui_res->textEdit_result->isReadOnly()){
        ui_res->textEdit_result->setReadOnly(true);
        ui_res->pushButton_writemode->setText("Enable\nWritemode");

    }else{
        ui_res->textEdit_result->setReadOnly(false);
        ui_res->pushButton_writemode->setText("Disable\nWritemode");
    }
}



void Results::on_pushButton_save_clicked()
{
    QString res_filename = QFileDialog::getSaveFileName(this, tr("Save Results"),
    "/home", "All Files(*.*);;Text Files(*.txt);; PDF Files (*.pdf)");
    if(res_filename.isEmpty()){
        return;
    }else{
        QFile res_file(res_filename);
        if(!res_file.open(QIODevice::WriteOnly)){
            QMessageBox::information(this, tr("Unable to save file"), res_file.errorString());
            return;
        }
        QTextStream res_out(&res_file);
        res_out<<result_table;
        res_file.close();
        ui_res->statusbar_res->showMessage("The files has been successfuly retained");
    }

}

void Results::on_pushButton_merge_clicked()
{
    QString merge_filename = QFileDialog::getOpenFileName(this, tr("Merge Results"),
    "/home", "All Files(*.*);;Text Files(*.txt);; PDF Files (*.pdf)");
    if(merge_filename.isEmpty()){
        return;
    }else{
        QFile merge_file(merge_filename);
        if(!merge_file.open(QIODevice::Append)){
            QMessageBox::information(this, tr("Unable to merge file"), merge_file.errorString());
            return;
    }
        QTextStream res_out(&merge_file);
        res_out<<result_table;
        merge_file.close();
        ui_res->statusbar_res->showMessage("The files has been successfuly merged");
}
}

QString Results::get_result(const double &r_t, const double res[][6], double ** inn_res){
    QTemporaryFile res_tmp;
    if(!res_tmp.open()){return "Error with tmp!";}//Problem

        QTextStream res_stream(&res_tmp);
        res_stream<<"All numbers are represented in SI units\n";
        res_stream<<QString("For Temperature %1 K:").arg(r_t)<<"\n\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<"Q(transition)"<<"Q(rotation)"<<"Q(frequency)"<<"Q(total)";

        res_stream.setFieldWidth(0);
        res_stream<<"\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<QString("%1").arg(res[0][0])<<QString("%1").arg(res[1][0])<<
        QString("%1").arg(res[2][0])<<QString("%1").arg(res[3][0]);

        res_stream.setFieldWidth(0);
        res_stream<<"\n\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<"U(transition)"<<"U(rotation)"<<"U(frequency)"<<"U(total)";

        res_stream.setFieldWidth(0);
        res_stream<<"\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<QString("%1").arg(res[0][1])<<QString("%1").arg(res[1][1])<<
        QString("%1").arg(res[2][1])<<QString("%1").arg(res[3][1]);

        res_stream.setFieldWidth(0);
        res_stream<<"\n\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<"H(transition)"<<"H(rotation)"<<"H(frequency)"<<"H(total)";

        res_stream.setFieldWidth(0);
        res_stream<<"\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<QString("%1").arg(res[0][2])<<QString("%1").arg(res[1][2])<<
        QString("%1").arg(res[2][2])<<QString("%1").arg(res[3][2]);

        res_stream.setFieldWidth(0);
        res_stream<<"\n\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<"G(transition)"<<"G(rotation)"<<"G(frequency)"<<"G(total)";

        res_stream.setFieldWidth(0);
        res_stream<<"\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<QString("%1").arg(res[0][3])<<QString("%1").arg(res[1][3])<<
        QString("%1").arg(res[2][3])<<QString("%1").arg(res[3][3]);

        res_stream.setFieldWidth(0);
        res_stream<<"\n\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<"S(transition)"<<"S(rotation)"<<"S(frequency)"<<"S(total)";

        res_stream.setFieldWidth(0);
        res_stream<<"\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<QString("%1").arg(res[0][5])<<QString("%1").arg(res[1][5])<<
        QString("%1").arg(res[2][5])<<QString("%1").arg(res[3][5]);

        res_stream.setFieldWidth(0);
        res_stream<<"\n\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<"Cp(transition)"<<"Cp(rotation)"<<"Cp(frequency)"<<"Cp(total)";

        res_stream.setFieldWidth(0);
        res_stream<<"\n";

        res_stream.setFieldWidth(20);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar(' ');
        res_stream<<QString("%1").arg(res[0][4])<<QString("%1").arg(res[1][4])<<
        QString("%1").arg(res[2][4])<<QString("%1").arg(res[3][4]);

        //To set line separator
        res_stream.setFieldWidth(0);
        res_stream<<"\n";
        res_stream.setFieldWidth(80);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar('-');
        res_stream<<"-";
        res_stream.setFieldWidth(0);
        res_stream<<"\n";

        if(Ui::inner_result){

            res_stream.setFieldWidth(10);
            res_stream.setPadChar(' ');
            res_stream.setFieldAlignment(QTextStream::AlignLeft);
            res_stream<<"#"<<"Q(inn)"<<"U(inn)"<<"H(inn)"<<"G(inn)"<<"S(inn)"
                 <<"Cp(inn)";

            res_stream.setFieldWidth(0);
            res_stream<<"\n";

            for(int i = 0; i < Ui::counter; i++){
                res_stream.setFieldWidth(10);
                res_stream.setFieldAlignment(QTextStream::AlignLeft);
                res_stream<<QString("%1)").arg(i+1)<<QString("%1").arg(inn_res[i][0])<<QString("%1").arg(inn_res[i][1])<<
                QString("%1").arg(inn_res[i][2])<<QString("%1").arg(inn_res[i][3])<<QString("%1").arg(inn_res[i][4])
                    <<QString("%1").arg(inn_res[i][5]);

                res_stream.setFieldWidth(0);
                res_stream<<"\n\n";
            }
        }

        //To set line separator
        res_stream.setFieldWidth(0);
        res_stream<<"\n";
        res_stream.setFieldWidth(80);
        res_stream.setFieldAlignment(QTextStream::AlignLeft);
        res_stream.setPadChar('=');
        res_stream<<"=";



        res_stream.setFieldWidth(0);
        res_stream<<"\n\n";


        res_stream.seek(0);
        return res_stream.readAll();


}


