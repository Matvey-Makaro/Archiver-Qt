#include <QApplication>
#include"Archiver.h"
#include"ArchiverUI.h"

using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Archiver archiver;
    ArchiverUI archiverWidget(&archiver);
    archiverWidget.show();
    return a.exec();
}
