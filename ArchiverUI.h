#ifndef ARCHIVERUI_H
#define ARCHIVERUI_H

#include <QFileDialog>
#include <QPushButton>

#include"Archiver.h"

class ArchiverUI : public QWidget
{
    Q_OBJECT;
public:
    ArchiverUI(Archiver* arch, QWidget* parent = nullptr);
private:
    void initUI();
private slots:
    void zip();
    void unzip();
private:
    Archiver* archiver;
    QPushButton* zipButton;
    QPushButton* unzipButton;


};

#endif // ARCHIVERUI_H
