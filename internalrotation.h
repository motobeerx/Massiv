#ifndef INTERNALROTATION_H
#define INTERNALROTATION_H

#include <QMainWindow>
#include <QtMath>
#include "results.h"


namespace Ui {
class InternalRotation;
class InnerData;
}

class InternalRotation : public QMainWindow
{
    Q_OBJECT

public:
    explicit InternalRotation(QWidget *parent = nullptr);
    ~InternalRotation();

private slots:
    void on_pushButton_addtop_clicked();

    void on_pushButton_finish_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_top_number_clicked();

public:
    Results * result_window;
    Ui::InternalRotation *ui_cosinuses;
};

class InnerData: public InternalRotation{
private:
    double frq_inn;
    double I_r;
    double Vmax;
    int min_number;
    double a1;
    double a2;
    double a3;
    double a4;
    double a5;
    double a6;
    double a7;
    double a8;
    double Q_inn,U_inn,H_inn,G_inn,S_inn,Cp_inn;
public:
    const double k_inner = qSqrt(2*M_PI*1.38065*qPow(10,-23))/(6.62607*qPow(10,-34));
    const double R = 8.314;
    InnerData():frq_inn(0),I_r(0),Vmax(0),min_number(1),
    a1(0),a2(0),a3(0),a4(0),a5(0),a6(0),a7(0),a8(0),
    Q_inn(0),U_inn(0),H_inn(0),G_inn(0),S_inn(0),Cp_inn(0){}

    double inn_rot_potential(double angle);
    double integral_func(int n_der, double angle);

    double slice_area(double x_1, double x_2,const int n_der, double precision);//calculate integrals
    double calc_integral(int n, int n_der);

    void setInnerFunction(int top_id);//в этой функции внешние переменные становятся результатом и также исправляются частоты
    void readInnerData(InternalRotation *ui_window);
};

#endif // INTERNALROTATION_H
