#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialogjoin.h"
#include "wincreate.h"
#include "wingenerate.h"
#include "xsabout.h"
#include "stronghold_global.h"

#include "sui.h"
#include "plugin.h"

namespace Ui {
class MainWindow;
}

class STRONGHOLDSHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_buttonSwitch_clicked();
private:
    Ui::MainWindow *ui;
    SUI* sui;
    bool mode = false;
};

#endif // MAINWINDOW_H
