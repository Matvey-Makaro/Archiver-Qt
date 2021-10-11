#include"ArchiverUI.h"

#include<iostream>
#include<string>
#include<QHBoxLayout>

ArchiverUI::ArchiverUI(Archiver* arch, QWidget* parent):
    QWidget(parent)
{
        archiver = arch;
        initUI();
}

void ArchiverUI::initUI()
{
    zipButton = new QPushButton("Zip", this);
    unzipButton = new QPushButton("Unzip", this);
    QHBoxLayout* hbox = new QHBoxLayout(this);
    hbox->addWidget(zipButton);
    hbox->addWidget(unzipButton);
    connect(zipButton, &QPushButton::clicked, this, &ArchiverUI::zip);
    connect(unzipButton, &QPushButton::clicked, this, &ArchiverUI::unzip);
}

void ArchiverUI::zip()
{
    std::string fileName = QFileDialog::getOpenFileName(this, "Choose file to zip", "", "*.pdf *.txt *.docx *.mp4").toStdString();
    archiver->inCompressFile(fileName);
}

void ArchiverUI::unzip()
{
    std::string fileName = QFileDialog::getOpenFileName(this, "Choose file to unzip", "", "*.pdf *.txt *.docx *mp4").toStdString();
    archiver->outCompressArchive(fileName);
}
