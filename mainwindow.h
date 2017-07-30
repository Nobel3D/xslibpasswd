#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialogjoin.h"
#include "wincreate.h"
#include "wingenerate.h"
#include "xsabout.h"
#include "stronghold_global.h"

#include "pem.h"
#include "sum.h"
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
    void on_comboTable_currentIndexChanged(int index);
    void on_buttonAdd_clicked();
    void on_buttonRemove_clicked();
    void on_buttonCreate_clicked();
    void on_menuAboutQT();
    void on_menuAboutXSoftware();
    void on_menuImportTable();
    void on_menuImportDatabase();
    void on_menuExportTable();
    void on_menuExportDatabase();

    void on_buttonSwitch_clicked();
    void on_buttonCommit_clicked();

    void on_buttonGenerate_clicked();

private:
    Ui::MainWindow *ui;
    DialogJoin* uiJoin;
    winCreate* create;
    winGenerate* generate;
    xsAbout* about;
    SUM* sum;
    PEM* pem;
    bool mode = false;
};

#endif // MAINWINDOW_H
