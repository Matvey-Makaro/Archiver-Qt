#include"ArchiverUI.h"

#include<string>
#include<QHBoxLayout>
#include<QMessageBox>
#include <QFileDialog>
#include<QString>

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
    resize(300, 100);
    setWindowTitle("Archiver");
}

void ArchiverUI::zip()
{
    std::string fileName = QFileDialog::getOpenFileName(this, "Choose file to zip", "", "*.pdf *.txt *.docx *.mp4").toStdString();
    if(fileName.empty())
        return;
    disableButtons();
    int errorCode = archiver->inCompressFile(fileName);

    if(errorCode == archiver->FILE_DOES_NOT_OPEN)
    {
        QMessageBox::warning(this, "Warning", "File doesn't open.");
    }
    else if(errorCode == archiver->FILE_EXTENSION_NOT_SUPPORTED)
    {
        QMessageBox::warning(this, "Warning", "This file extension is not supported");
    }
    else if (errorCode == archiver->NO_ERRORS)
    {
        QMessageBox::information(this, "Information", "Zip completed.");
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Unknown problem.");
    }
    enableButtons();
}

void ArchiverUI::unzip()
{
    std::string fileName = QFileDialog::getOpenFileName(this, "Choose file to unzip", "", "*.pdf *.txt *.docx *mp4").toStdString();
    if(fileName.empty())
        return;
    disableButtons();
    int errorCode = archiver->outCompressArchive(fileName);

    if(errorCode == archiver->FILE_DOES_NOT_OPEN)
    {
        QMessageBox::warning(this, "Warning", "File doesn't open.");
    }
    else if(errorCode == archiver->FILE_EXTENSION_NOT_SUPPORTED)
    {
        QMessageBox::warning(this, "Warning", "This file extension is not supported.");
    }
    else if (errorCode == archiver->NO_ERRORS)
    {
        QMessageBox::information(this, "Information", "Unzip completed.");
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Unknown problem.");
    }
    enableButtons();
}

void ArchiverUI::enableButtons()
{
    zipButton->setText("Zip");
    zipButton->setEnabled(true);
    unzipButton->setText("Unzip");
    unzipButton->setEnabled(true);
    repaint();
}

void ArchiverUI::disableButtons()
{
    zipButton->setText("Processing");
    zipButton->setEnabled(false);
    unzipButton->setText("Processing");
    unzipButton->setEnabled(false);
    repaint();
}
