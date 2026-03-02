#include <QApplication>

#include "nstartwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    NTA::NStartWindow startWindow;
    startWindow.show();
    return QApplication::exec();
}
