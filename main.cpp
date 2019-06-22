#include "newhorizon.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NewHorizon w;
    w.show();

    return a.exec();
}
