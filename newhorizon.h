#ifndef NEWHORIZON_H
#define NEWHORIZON_H

#include <QMainWindow>
#include "statcalc.h"
#include "about.h"

namespace Ui {
class NewHorizon;
}

class NewHorizon : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewHorizon(QWidget *parent = nullptr);
    ~NewHorizon();

private slots:
    void on_pushButton_close_clicked();

    void on_actionRead_File_triggered();

    void on_actionAbout_triggered();

private:
    Ui::NewHorizon *ui;
    StatCalc *statcalc_window;
    About * about_window = nullptr;
};

#endif // NEWHORIZON_H
