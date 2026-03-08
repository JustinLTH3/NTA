#include <QApplication>

#include "nstartwindow.h"
#include "nmainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    NTA::NStartWindow startWindow;
    startWindow.show();
    NTA::NMainWindow mainWindow;
    mainWindow.show();
    return QApplication::exec();
}
