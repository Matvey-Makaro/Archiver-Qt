#include "mainwindow.h"

#include <QApplication>
#include<string>
#include"Archiver.h"
#include"ArchiverUI.h"

using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
//    Archiver archiver;
//    string fileName = "TestDOCX.docx";
//    string zipFileName = "OutPut.txt";
//    string unzipFileName = "Test.docx";
//    string pngName = "C:\\Users\\Lenovo\\Dropbox\\Мой ПК (DESKTOP-5RFI7SD)\\Desktop\\apple.png";
//    string exeName = "C:\\Users\\Lenovo\\Dropbox\\Мой ПК (DESKTOP-5RFI7SD)\\Desktop\\play_dog.exe";
//    string docxName = "C:\\Users\\Lenovo\\Dropbox\\Мой ПК (DESKTOP-5RFI7SD)\\Desktop\\helloworld.docx";
//    string jpgName = "jpgFile.jpg";
//    string pdfName = "E:\\Учебные материалы\\1Philosophy_практикум.pdf";
//    string mp4Name = "E:\\Учебные материалы\\Алгоритмы и структуры данных\\АиСД 1. Введение..mp4";
//    string mp4NameTest = "TestMP4.mp4";

//        //что хотим заархивировать - куда хотим заархивировать
//        archiver.inCompressFile(fileName, zipFileName);	//16
//        //что хотим разархиваровать - куда хотим разархиваровать
//        archiver.outCompressArchive(zipFileName, unzipFileName); //12
    Archiver archiver;
    ArchiverUI archiverWidget(&archiver);
    archiverWidget.show();
    return a.exec();
}
