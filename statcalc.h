#ifndef STATCALC_H
#define STATCALC_H

#include <QMainWindow>
#include <QTextStream>
#include <QTemporaryFile>
#include <QtMath>
#include "results.h"
#include "internalrotation.h"

namespace Ui {
class StatCalc;
class Data;
}

class StatCalc : public QMainWindow
{
    Q_OBJECT

public:
    explicit StatCalc(QWidget *parent = nullptr);
    ~StatCalc();


 private slots:
    void on_pushButton_Next_clicked();

    void on_pushButton_Back_clicked();

    void on_toolButton_freq_clicked();

    void on_pushButton_temperature_clicked();

    void on_pushButton_scalefactor_clicked();

    public://Почему они ПАБЛИК??
    Results * result_window;
    Ui::StatCalc *ui_2;
    InternalRotation * int_rot_window;
    };

class Data : public StatCalc{
  private:

    double T;
    double SF;
    int *center_number;
    double *atomic_mass;
    double *x_out_centr_mass;
    double *y_out_centr_mass;
    double *z_out_centr_mass;
    //read-param
    int norm_modes;
    //
    double *frequencies;
    int symetry_number;
    double molec_mass;
    int N_atoms;
    double IaIbIc;
    //transition
    double Q_tra;
    double U_tra;
    double H_tra;
    double G_tra;
    double Cp_tra;
    double S_tra;
    //rotation
    double Q_rot;
    double U_rot;
    double H_rot;
    double G_rot;
    double Cp_rot;
    double S_rot;
    //freq
    double Q_frq;
    double U_frq;
    double H_frq;
    double G_frq;
    double Cp_frq;
    double S_frq;
  public:
    const double R =8.314;
    const double k_tra = 5.006* qPow(10, 31);
    const double k_rot = 2.19 * qPow(10, 68);
    Data():T(Ui::r_T),SF(Ui::scal_fac),IaIbIc(0),
      Q_tra(0),U_tra(0),H_tra(0), G_tra(0),Cp_tra(0), S_tra(0),
      Q_rot(0),U_rot(0),H_rot(0),G_rot(0),Cp_rot(0), S_rot(0),
      Q_frq(0),U_frq(0), H_frq(0), G_frq(0), Cp_frq(0),S_frq(0) {}
    //~Data();
    void read_freq(StatCalc *ui_window);
    void calc_tra();
    void calc_rot();
    void calc_frq();
    void save_result(double results[][6]);


};

#endif // STATCALC_H
