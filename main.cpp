#include "mainwindow.h"

#include <QApplication>
#include<string>
#include"Archiver.h"
#include"ArchiverUI.h"

using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    Archiver archiver;
    ArchiverUI archiverWidget(&archiver);
    archiverWidget.show();
    return a.exec();
}
