#include <QApplication>

#include "nstartwindow.h"
#include "nmainwindow.h"
#include "nspacemanager.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    auto startWindow = new NTA::NStartWindow();
    startWindow->show();
    startWindow->connect(startWindow, &NTA::NStartWindow::openSpace, startWindow,
                         [&startWindow](const QSharedPointer<NTA::Space>& space)
                         {
                             if (space)
                                 if (NTA::NSpaceManager::createInstance(space))
                                 {
                                     const auto mainWindow = new NTA::NMainWindow();
                                     mainWindow->show();
                                     startWindow->deleteLater();
                                 }
                         });
    return QApplication::exec();
}
