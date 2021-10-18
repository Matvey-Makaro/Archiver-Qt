#ifndef ARCHIVERUI_H
#define ARCHIVERUI_H

#include <QPushButton>

#include"Archiver.h"

class ArchiverUI : public QWidget
{
    Q_OBJECT;
public:
    ArchiverUI(Archiver* arch, QWidget* parent = nullptr);
private:
    void initUI();
    void disableButtons();
    void enableButtons();
private slots:
    void zip();
    void unzip();
private:
    Archiver* archiver;
    QPushButton* zipButton;
    QPushButton* unzipButton;
};

#endif // ARCHIVERUI_H
