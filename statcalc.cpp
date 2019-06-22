#include "statcalc.h"
#include "ui_statcalc.h"
#include <QMessageBox>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QString>
#include "newhorizon.h"
#include "results.h"
#include <QtMath>
#include <QFileDialog>
#include <QDebug>

double Ui::r_T = 298.15;
double Ui::result [4][6];
double Ui::scal_fac = 1;


StatCalc::StatCalc(QWidget *parent) :
    QMainWindow(parent),
    ui_2(new Ui::StatCalc)
{
    ui_2->setupUi(this);
}


StatCalc::~StatCalc()
{
    delete ui_2;
}
void Data::read_freq(StatCalc *ui_window){

    //make pathway for freq file
    QString freqFile_pathway = ui_window->ui_2->lineEdit_freq->text();

    //If file was dragged!!
    if(freqFile_pathway.contains("file://")){
    freqFile_pathway.chop(2);
    freqFile_pathway.remove(0,6);};

    //open files(read only)
    QFile freqFile(freqFile_pathway);
    freqFile.open(QIODevice::ReadOnly);
    //help-variable
    bool reading_freq = true;

    //check if files were opened
    if(!freqFile.isOpen()){
    throw QString("Can't open freq-file...");};

    //seek freq
    int i=0;//for reading frequencies
    QTextStream freq_file(&freqFile);
    QString line;
    bool mode_read_stop = true;
    bool read_coord =true;
    bool read_n_atoms=true;
    bool sym_match = true;
    N_atoms = 0;
while(freq_file.status() != 1){
        freq_file>>line;

        //Check If it is Gaussian log/out file
        if(!(freqFile_pathway.contains(".out") ||freqFile_pathway.contains(".log")))throw QString("Invalid file extension");


        //Check if it is FREQ-FILE
        if(line == "#" || line == "#T"|| line == "#P"){
            QString method_check = freq_file.readLine(76);
            if(!method_check.contains("freq")){
            freqFile.close();
            throw QString(method_check+": " + "This isn't freq-file.out. Check freq-pathway please");};};

        //symmetry
        if(line == "NOp" && sym_match){
            freq_file>>line;
            QString sym_numb = line;
            symetry_number = sym_numb.toInt();
            sym_match = false;};

        //number of atoms
        if(line == "NAtoms=" && read_n_atoms){freq_file>>N_atoms; freq_file.seek(1);read_n_atoms =false;}

        //number of modes
        if(line == "freedom" && mode_read_stop){freq_file>>norm_modes;mode_read_stop = false;
            frequencies =new double[norm_modes];}




        //standart coordinates
        if(line == "Standard" && read_coord && N_atoms != 0){
            center_number = new int[N_atoms];
            atomic_mass = new double[N_atoms];
            x_out_centr_mass = new double[N_atoms];
            y_out_centr_mass = new double[N_atoms];
            z_out_centr_mass = new double[N_atoms];
            int *atomic_number = new int [N_atoms];
            read_coord = false;
            for(int i =0; i<14; i++){freq_file>>line;}//to skip all unneeded words
            for(int j=0; j<N_atoms;j++){
            freq_file>>line;
            center_number[j] = line.toInt();
            freq_file>>atomic_number[j];
            switch (atomic_number[j]) {
            case 1:atomic_mass[j] = 1.079;break;   //H
            case 6:atomic_mass[j] = 12;break;      //C
            case 8:atomic_mass[j] = 15.994;break;  //O
            case 7:atomic_mass[j] = 14;break;      //N
            default: atomic_mass[j] = 0;break;}
            freq_file>>line;
            freq_file>>line;
            x_out_centr_mass[j] = line.toDouble();
            freq_file>>line;
            y_out_centr_mass[j] = line.toDouble();
            freq_file>>line;
            z_out_centr_mass[j] = line.toDouble();}//end of reading for-loop
            delete [] atomic_number;
         }

        // mass of molecula
        if(line == "mass:" ){freq_file>>molec_mass;}

        // read all frequencies
        if(line == "Frequencies" && reading_freq){
            freq_file>>line;
            freq_file>>line;
            frequencies[i] = SF*line.toDouble(&reading_freq);
            i++;
            freq_file>>line;
            frequencies[i] = SF*line.toDouble(&reading_freq);
            i++;
            freq_file>>line;
            frequencies[i] = SF*line.toDouble(&reading_freq);
            i++;
            if(i == norm_modes){reading_freq = false;};
        }
}//end of while-loop!
freqFile.close();

//calculating moment of inertia
    double  x_0 = 0.0;
    double  y_0 = 0.0;
    double  z_0 = 0.0;
    for(int i=0; i<N_atoms; i++){
        x_0 =x_0 + x_out_centr_mass[i]*atomic_mass[i]/molec_mass;
        y_0 =y_0 + y_out_centr_mass[i]*atomic_mass[i]/molec_mass;
        z_0 =z_0 + z_out_centr_mass[i]*atomic_mass[i]/molec_mass;}//end of calc cent mass coord
    double * x_in_centr_mass = new double [N_atoms];
    double * y_in_centr_mass = new double [N_atoms];
    double * z_in_centr_mass = new double [N_atoms];
    for(int j=0; j<N_atoms; j++){
        x_in_centr_mass[j] = x_out_centr_mass[j] - x_0;
        y_in_centr_mass[j] = y_out_centr_mass[j] - y_0;
        z_in_centr_mass[j] = z_out_centr_mass[j] - z_0;}//end of calc molec true coord
    double I_xx = 0.0;
    double I_yy = 0.0;
    double I_zz = 0.0;
    double I_xy = 0.0;
    double I_xz = 0.0;
    double I_yz = 0.0;
    for(int i =0;i<N_atoms;i++){
    I_xx=I_xx + atomic_mass[i]*(y_in_centr_mass[i]*y_in_centr_mass[i]+z_in_centr_mass[i]*z_in_centr_mass[i]);
    I_yy=I_yy + atomic_mass[i]*(x_in_centr_mass[i]*x_in_centr_mass[i]+z_in_centr_mass[i]*z_in_centr_mass[i]);
    I_zz=I_zz + atomic_mass[i]*(y_in_centr_mass[i]*y_in_centr_mass[i]+x_in_centr_mass[i]*x_in_centr_mass[i]);
    I_xy=I_xy + atomic_mass[i]*x_in_centr_mass[i]*y_in_centr_mass[i];
    I_xz=I_xz + atomic_mass[i]*x_in_centr_mass[i]*z_in_centr_mass[i];
    I_yz=I_yz + atomic_mass[i]*z_in_centr_mass[i]*y_in_centr_mass[i];}//end of calc moments of inertia
    IaIbIc = ((I_xx*I_yy*I_zz)-(2*I_xy*I_xz*I_yz) - (I_xz*I_xz*I_yy) - (I_xy*I_xy*I_zz) - (I_yz*I_yz*I_xx))
            *(1.66*1.66*1.66)/1000;

    delete [] x_in_centr_mass;
    delete [] y_in_centr_mass;
    delete [] z_in_centr_mass;
    //Есть деструктор, возможно лишние
    delete [] center_number;
    delete [] atomic_mass;
    delete [] x_out_centr_mass;
    delete [] y_out_centr_mass;


};

void StatCalc::on_pushButton_Next_clicked()
{
    try {
    Data data;
    data.read_freq(this);
    data.calc_tra();
    data.calc_rot();
    data.calc_frq();
    data.save_result(Ui::result);
    if(ui_2->checkBox->isChecked()){
        close();
        int_rot_window = new InternalRotation(this);
        int_rot_window->show();
    }else{
        close();
        result_window = new Results(this);
        result_window->show();
    }

    } catch (QString &error) {
        ui_2->statusbar->showMessage(error);
    }
}
void StatCalc::on_pushButton_Back_clicked()
{
    close();
    NewHorizon *window = new NewHorizon(this);
    window->show();

}

void Data::calc_tra(){
    Q_tra = k_tra * qPow(molec_mass, 1.5)* qPow(T, 2.5);
    U_tra = 1.5 *R*T;
    H_tra = 2.5*R*T;
    Cp_tra = 2.5 * R;
    S_tra = 1.5*R*qLn(molec_mass) + 2.5*R*qLn(T) - R*qLn(0.98692) - 9.6853;
    G_tra = -T*(1.5*R*qLn(molec_mass) + 2.5*R*qLn(T) - R*qLn(0.98692) - 30.4713);
  };
void Data::calc_rot(){
    Q_rot = k_rot*qSqrt(IaIbIc/10)/symetry_number;
    U_rot = 1.5 * R *T;
    H_rot = 1.5 * R *T;
    Cp_rot = 1.5*R;
    S_rot = 0.5*R*qLn(IaIbIc) - R*qLn(symetry_number) +1.5*R*qLn(T) - 0.1447;
    G_rot = -T*(0.5*R*qLn(IaIbIc) - R*qLn(symetry_number) +1.5*R*qLn(T) - 12.6163);
}
void Data::calc_frq(){
     Q_frq = U_frq = H_frq = S_frq =Cp_frq = G_frq = 0.0;
    for(int i =0; i<norm_modes; i++){
        Q_frq = Q_frq + 1/(1 - qExp(-1.44*frequencies[i]/T));
        U_frq = U_frq + (R*1.44*frequencies[i])/(qExp(1.44*frequencies[i]/T) - 1);
        H_frq = H_frq + (R*1.44*frequencies[i])/(qExp(1.44*frequencies[i]/T) - 1);
        Cp_frq = Cp_frq + (R*(1.44*frequencies[i]/T)*(1.44*frequencies[i]/T)*qExp(1.44*frequencies[i]/T))/
                ((qExp(1.44*frequencies[i]/T) -1)*(qExp(1.44*frequencies[i]/T) -1 ) );
        S_frq = S_frq + R*(1.44*frequencies[i]/T)/(qExp(1.44*frequencies[i]/T) - 1)
                - R*qLn(1 - qExp(-1.44*frequencies[i]/T));
        G_frq = G_frq + R*T*qLn(1 - qExp(-1.44*frequencies[i]/T));
    }
}
void Data::save_result(double results[][6]){

      results[0][0] = Q_tra;
      results[0][1] = U_tra;
      results[0][2] = H_tra;
      results[0][3] = G_tra;
      results[0][4] = Cp_tra;
      results[0][5] = S_tra;

      results[1][0] = Q_rot;
      results[1][1] = U_rot;
      results[1][2] = H_rot;
      results[1][3] = G_rot;
      results[1][4] = Cp_rot;
      results[1][5] = S_rot;

      results[2][0] = Q_frq;
      results[2][1] = U_frq;
      results[2][2] = H_frq;
      results[2][3] = G_frq;
      results[2][4] = Cp_frq;
      results[2][5] = S_frq;

      results[3][0] = Q_tra*Q_rot*Q_frq;
      results[3][1] = U_tra + U_rot + U_frq;
      results[3][2] = H_tra + H_rot + H_frq;
      results[3][3] = G_tra + G_rot + G_frq;
      results[3][4] = Cp_tra + Cp_rot + Cp_frq;
      results[3][5] = S_tra + S_rot + S_frq;
}

void StatCalc::on_toolButton_freq_clicked()
{
    QString frq_filename = QFileDialog::getOpenFileName(this,
    tr("Open frq_file"), "/home", "All files(*.*);;Gaussian files(*.log);;Gaussian files(*.out);;Gaussian files(*.txt)");
    ui_2->lineEdit_freq->setText(frq_filename);
}

void StatCalc::on_pushButton_temperature_clicked()
{
   QString temperature = ui_2->lineEdit_temperature->text();
   if(!bool(temperature.toDouble()) || temperature.toDouble() <= 0){
       ui_2->statusbar->showMessage("Invalid argument. Temperature can't be less or equal 0 or text. Set another temperature.");}
   else {
       Ui::r_T = temperature.toDouble();
       ui_2->label_set_temperature->setText("Temperature is set to "+QString("%1").arg(Ui::r_T)+" K");
       ui_2->statusbar->showMessage("Temperature has been successfuly set");
   }
}

void StatCalc::on_pushButton_scalefactor_clicked()
{
    QString scale_factor = ui_2->lineEdit_scalefactor->text();
    if(!bool(scale_factor.toDouble()) || scale_factor.toDouble() <= 0){
        ui_2->statusbar->showMessage("Invalid argument. Scale factor can't be less or equal 0 or text."
                                     " Scale factor is 1 by default.");
    }
    else {
        Ui::scal_fac = scale_factor.toDouble();
        ui_2->label_set_SF->setText("Scale Factor is set to "+QString("%1").arg(Ui::scal_fac));
        ui_2->statusbar->showMessage("Scale factor has been successfuly set");
    }

}
