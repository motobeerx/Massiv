#include "internalrotation.h"
#include "ui_internalrotation.h"
#include "results.h"
#include "statcalc.h"
#include <QtMath>
#include <QDebug>

int Ui::top_number;
double **Ui::inner_result = nullptr;
int Ui::counter = 0;


InternalRotation::InternalRotation(QWidget *parent) :
    QMainWindow(parent),
    ui_cosinuses(new Ui::InternalRotation)
{
    ui_cosinuses->setupUi(this);


}

InternalRotation::~InternalRotation()
{
    delete ui_cosinuses;
}

void InternalRotation::on_pushButton_addtop_clicked()
{
    try {
        InnerData data;
        data.readInnerData(this);
        data.setInnerFunction(Ui::counter);
        Ui::top_number--;
        Ui::counter++;
        ui_cosinuses->lcdNumber_tops_counter->display(Ui::top_number);
    } catch (QString &error) {
        ui_cosinuses->statusbar->showMessage(error);
    }

}

void InternalRotation::on_pushButton_finish_clicked()
{
    if(int(ui_cosinuses->lcdNumber_tops_counter->value()) == 0){
        close();
        result_window = new Results(this);
        result_window->show();
    }else {
        ui_cosinuses->statusbar->showMessage("Not all tops were set! Ether set them up or set top's number to 0");
    }
}

void InternalRotation::on_pushButton_cancel_clicked()
{
    delete [] Ui::inner_result;
    Ui::counter = 0;
    //Do i need ro reset T and SF!!
    close();//Issue, it's needed to back with 'cockies' and 'breadcrums'
    StatCalc *window = new StatCalc(this);
    window->show();
}

void InnerData::readInnerData(InternalRotation *ui_window){
    bool frq_check;
    bool mom_in_check;
    bool v_max_check;
    bool n_min_check;
    bool a1_check;
    bool a2_check;
    bool a3_check;
    bool a4_check;
    bool a5_check;
    bool a6_check;
    bool a7_check;
    bool a8_check;

     QString freq_inner = ui_window->ui_cosinuses->lineEdit_frqrot->text();
     frq_inn = freq_inner.toDouble(&frq_check);
     if(!frq_check || frq_inn < 0)throw QString("Invalid Frequency of Inner Rotation.");

     QString sI_r = ui_window->ui_cosinuses->lineEdit_red_inertia_mom->text();
     I_r = sI_r.toDouble(&mom_in_check);
     if(!mom_in_check || I_r < 0)throw QString("Invalid Reduced Moment of Inertia");

     QString sVmax = ui_window->ui_cosinuses->lineEdit_pot_barrier->text();//Не читает введенное значение!!!!
     Vmax = sVmax.toDouble(&v_max_check);
     if(!v_max_check || Vmax < 0)throw QString("Invalid Potential Barrier");

     QString smin_number = ui_window->ui_cosinuses->lineEdit_num_min->text();
     min_number = smin_number.toInt(&n_min_check);
     if(!n_min_check || min_number <= 0)throw QString("Invalid Number of Minimas");

     QString sa1 = ui_window->ui_cosinuses->lineEdit_a1->text();
     a1 = sa1.toDouble(&a1_check);
     if(!a1_check)throw QString("Invalid First Factor");

     QString sa2 = ui_window->ui_cosinuses->lineEdit_a2->text();
     a2 = sa2.toDouble(&a2_check);
     if(!a2_check)throw QString("Invalid Second Factor");

     QString sa3 = ui_window->ui_cosinuses->lineEdit_a3->text();
     a3 = sa3.toDouble(&a3_check);
     if(!a3_check)throw QString("Invalid Third Factor");

     QString sa4 = ui_window->ui_cosinuses->lineEdit_a4->text();
     a4 = sa4.toDouble(&a4_check);
     if(!a4_check)throw QString("Invalid Fourth Factor");

     QString sa5 = ui_window->ui_cosinuses->lineEdit_a5->text();
     a5 = sa5.toDouble(&a5_check);
     if(!a5_check)throw QString("Invalid Fifth Factor");

     QString sa6 = ui_window->ui_cosinuses->lineEdit_a6->text();
     a6 = sa6.toDouble(&a6_check);
     if(!a6_check)throw QString("Invalid Sixth Factor");

     QString sa7 = ui_window->ui_cosinuses->lineEdit_a7->text();
     a7 = sa7.toDouble(&a7_check);
     if(!a7_check)throw QString("Invalid Seventh Factor");

     QString sa8 = ui_window->ui_cosinuses->lineEdit_a8->text();
     a8 = sa8.toDouble(&a8_check);
     if(!a8_check)throw QString("Invalid Eighth Factor");

}

void InnerData::setInnerFunction(int top_id){

    double integral = calc_integral(100000,0);
    double der_integral = calc_integral(100000,1);
    double der_der_integral = calc_integral(100000,2);

    Q_inn = k_inner * qSqrt(I_r * Ui::r_T * qPow(10, -47))/min_number * integral;
    U_inn = R*Ui::r_T/2 + der_integral/integral;
    H_inn = R*Ui::r_T/2 + der_integral/integral;
    G_inn = -R*Ui::r_T*qLn(Q_inn);
    S_inn = R*qLn(Q_inn) + qPow(Ui::r_T, -1)*der_integral/integral;
    Cp_inn = R/2 + qPow(Ui::r_T, -2)*der_der_integral/integral -
            qPow(Ui::r_T, -2)*qPow(der_integral/integral,2);

    //correct frq and total
    if(frq_inn > 0){
    Ui::result[2][0]= Ui::result[2][0] - 1/(1 - qExp(-1.44*frq_inn/Ui::r_T));
    Ui::result[2][1]= Ui::result[2][1] - (R*1.44*frq_inn)/(qExp(1.44*frq_inn/Ui::r_T) - 1);
    Ui::result[2][2]= Ui::result[2][2] - (R*1.44*frq_inn)/(qExp(1.44*frq_inn/Ui::r_T) - 1);
    Ui::result[2][3]= Ui::result[2][3] - R*Ui::r_T*qLn(1 - qExp(-1.44*frq_inn/Ui::r_T));
    Ui::result[2][4]= Ui::result[2][4] - R*(1.44*frq_inn/Ui::r_T)/(qExp(1.44*frq_inn/Ui::r_T) - 1)
            - R*qLn(1 - qExp(-1.44*frq_inn/Ui::r_T));
    Ui::result[2][5]= Ui::result[2][5] - (R*(1.44*frq_inn/Ui::r_T)*(1.44*frq_inn/Ui::r_T)*qExp(1.44*frq_inn/Ui::r_T))/
            ((qExp(1.44*frq_inn/Ui::r_T) -1)*(qExp(1.44*frq_inn/Ui::r_T) -1 ) );

    Ui::result[3][0]= Ui::result[3][0] - 1/(1 - qExp(-1.44*frq_inn/Ui::r_T));
    Ui::result[3][1]= Ui::result[3][1] - (R*1.44*frq_inn)/(qExp(1.44*frq_inn/Ui::r_T) - 1);
    Ui::result[3][2]= Ui::result[3][2] - (R*1.44*frq_inn)/(qExp(1.44*frq_inn/Ui::r_T) - 1);
    Ui::result[3][3]= Ui::result[3][3] - R*Ui::r_T*qLn(1 - qExp(-1.44*frq_inn/Ui::r_T));
    Ui::result[3][4]= Ui::result[3][4] - R*(1.44*frq_inn/Ui::r_T)/(qExp(1.44*frq_inn/Ui::r_T) - 1)
            - R*qLn(1 - qExp(-1.44*frq_inn/Ui::r_T));
    Ui::result[3][5]= Ui::result[3][5] - (R*(1.44*frq_inn/Ui::r_T)*(1.44*frq_inn/Ui::r_T)*qExp(1.44*frq_inn/Ui::r_T))/
            ((qExp(1.44*frq_inn/Ui::r_T) -1)*(qExp(1.44*frq_inn/Ui::r_T) -1 ) );
    }

    //set Inner Function
    Ui::inner_result[top_id][0] = Q_inn;
    Ui::inner_result[top_id][1] = U_inn;
    Ui::inner_result[top_id][2] = H_inn;
    Ui::inner_result[top_id][3] = G_inn;
    Ui::inner_result[top_id][4] = S_inn;
    Ui::inner_result[top_id][5] = Cp_inn;
}

double InnerData::calc_integral(int n, int n_der){
    double s = 0;
    double xmin = 0;
    double xmax = 360 * M_PI / 180;
    double dx = (xmax - xmin) / n;
    double x = xmin;
    for (int i = 0; i < n; i++) {
        s += slice_area(x, x + dx,n_der, 0.01);
        x += dx;
    }
    return s;
}

double InnerData::slice_area(double x_1, double x_2,const int n_der, double precision) {

    double y_1 = integral_func(n_der,x_1);
    double y_2 = integral_func(n_der,x_2);
    double x_m = (x_2 - x_1) / 2 + x_1;
    double y_m = integral_func(n_der,x_m);

    double area12 = (x_2 - x_1) * (y_1 + y_2) / 2;
    double area1m = (x_m - x_1) * (y_m + y_1) / 2;
    double aream2 = (x_2 - x_m) * (y_m + y_2) / 2;
    double area1m2 = area1m + aream2;

    double error = (area1m2 - area12) * 100 / area12;
    if (error < precision)
        return area1m2;

    return slice_area(x_1, x_m,n_der, precision)
            + slice_area(x_m, x_2,n_der, precision);
}


double InnerData::integral_func(int n_der, double angle){

    double f = qPow(inn_rot_potential(angle),n_der) * qExp(-inn_rot_potential(angle)/R / Ui::r_T);
    return f;

}

double InnerData::inn_rot_potential(double angle){
    double v = Vmax + a1*qCos(angle*M_PI/180) + a2*qCos(2*angle*M_PI/180)+ a3*qCos(3*angle*M_PI/180)
            + a4*qCos(4*angle*M_PI/180)+ a5*qCos(5*angle*M_PI/180)+ a6*qCos(6*angle*M_PI/180)
            + a7*qCos(7*angle*M_PI/180)+ a8*qCos(8*angle*M_PI/180);
    return v;
}

void InternalRotation::on_pushButton_top_number_clicked()
{
    if(Ui::inner_result != nullptr){
        for(int i = 0; i<Ui::top_number; i++){
            delete [] Ui::inner_result[i];
        }
        delete [] Ui::inner_result;

    }

    QString top_num;
    bool check_top_num = false;
    top_num = ui_cosinuses->lineEdit->text();//ДОБАВЬ ПРОВЕРКУ НА ИНТ, ОГРАНИЧИТЬ 9999 - ВЕРЗНИЙ ЛИМИТ, НЕ МОДЕТ БЫТЬ МЕНЬШЕ 0
    Ui::top_number = top_num.toInt(&check_top_num);
    if(!check_top_num || top_num.toInt() < 0)throw QString("Invalid top number");
    Ui::inner_result = new double* [Ui::top_number];
    for(int i = 0; i<Ui::top_number; i++){
        Ui::inner_result[i] = new double [6];
    }
    ui_cosinuses->pushButton_addtop->setEnabled(true);
    ui_cosinuses->lcdNumber_tops_counter->display(Ui::top_number);

}
