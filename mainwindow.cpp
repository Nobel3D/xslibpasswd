#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QAction>
#include <QFileDialog>
#include <QFileInfoList>
#include <QPluginLoader>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
 /*   Plugin* s;
    QPluginLoader pluginLoader("/home/luca/Programmazione/proj/Stronghold/plugins/test/bin/libMyPlugin.so");
            QObject *plugin = pluginLoader.instance();
            if (plugin)
            {
                s = qobject_cast<Plugin*>(plugin);
                QMessageBox::warning(nullptr, "plugin", QString::number(s->use()));
            }
    */
    sui = new SUI();
    ui->setupUi(this);
    sui->init(ui->tableView);

    connect(ui->actionAbout_QT, SIGNAL(triggered(bool)), sui, SLOT(aboutQT()));
    connect(ui->actionAbout_XSoftware, SIGNAL(triggered(bool)), sui, SLOT(aboutXSoftware()));

    connect(ui->actionExport_Database, SIGNAL(triggered()), sui, SLOT(exportDatabase()));
    connect(ui->actionExport_Table, SIGNAL(triggered()), sui, SLOT(exportTable()));
    connect(ui->actionImport_Table, SIGNAL(triggered()), sui, SLOT(importTable()));

    connect(ui->buttonGenerate, SIGNAL(clicked(bool)), sui, SLOT(generate()));
    connect(ui->buttonAdd, SIGNAL(clicked(bool)), sui, SLOT(addRecord()));
    connect(ui->buttonRemove, SIGNAL(clicked(bool)), sui, SLOT(removeRecord()));
    connect(ui->comboTable, SIGNAL(currentIndexChanged(QString)), sui, SLOT(loadTable(QString)));
    connect(ui->buttonCommit, SIGNAL(clicked(bool)), sui, SLOT(commit()));
    connect(ui->buttonCreate, SIGNAL(clicked(bool)), sui, SLOT(createTable()));

    ui->comboTable->addItems(sui->tableList());

     connect(ui->tableView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), sui, SLOT(updateRecord(QModelIndex,QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonSwitch_clicked()
{
    mode = !mode;
    if(!mode)
    {
        ui->buttonCommit->setDisabled(false);
        ui->comboTable->clear();
        ui->buttonAdd->setIcon(QIcon::fromTheme("insert-table-row"));
        ui->buttonRemove->setIcon(QIcon::fromTheme("delete-table-row"));
        ui->comboTable->addItems(sui->tableList());
        sui->loadTable(ui->comboTable->currentText());
    }
    else
    {
        ui->buttonCommit->setDisabled(true);
        ui->buttonAdd->setIcon(QIcon::fromTheme("list-add-user"));
        ui->buttonRemove->setIcon(QIcon::fromTheme("list-remove-user"));
        ui->comboTable->clear();
        ui->comboTable->addItem("Admin View");
        sui->adminTable();
    }
}

