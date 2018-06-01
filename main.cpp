#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowFlags(w.windowFlags() & ~Qt::WindowMaximizeButtonHint);
    w.showMaximized();

    if(argc>1 && argv[1]!=NULL)
    {
        w.SetFileName(argv[1]);
    }

    w.show();

    return a.exec();
}
