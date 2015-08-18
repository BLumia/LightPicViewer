#include "picviewerwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PicViewerWindow w;
    w.show();

    return a.exec();
}
