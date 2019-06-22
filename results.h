#ifndef RESULTS_H
#define RESULTS_H

#include <QMainWindow>





namespace Ui {
extern double r_T;
extern double result [4][6];
extern double scal_fac;
extern int top_number;
extern double **inner_result;
extern int counter;
class Results;
}

class Results : public QMainWindow
{
    Q_OBJECT

public:
    explicit Results(QWidget *parent = nullptr);
    ~Results();
    QString get_result(const double &r_t, const double res[][6], double ** inn_res);

private slots:
    void on_pushButton_back_clicked();

    void on_pushButton_quit_clicked();

    void on_pushButton_writemode_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_merge_clicked();

private:
    Ui::Results *ui_res;
    QString result_table;

};

#endif // RESULTS_H
