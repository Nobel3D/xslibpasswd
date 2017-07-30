#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uitable.h"
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
    Plugin* s;
    QPluginLoader pluginLoader("/home/luca/Programmazione/proj/Stronghold/plugins/test/bin/libMyPlugin.so");
            QObject *plugin = pluginLoader.instance();
            if (plugin)
            {
                s = qobject_cast<Plugin*>(plugin);
                QMessageBox::warning(nullptr, "plugin", QString::number(s->use()));
            }

    sum = new SUM(LOGINFILE);
    uiJoin = new DialogJoin(sum);
    while(uiJoin->exec() != QDialog::Accepted);
    ui->setupUi(this);
    pem = new PEM(sum->getUser());
    ui->tableView->init(sum, pem);
    ui->comboTable->addItems(pem->tableList());
    connect(ui->actionAbout_QT, SIGNAL(triggered(bool)), this, SLOT(on_menuAboutQT()));
    connect(ui->actionAbout_XSoftware, SIGNAL(triggered(bool)), this, SLOT(on_menuAboutXSoftware()));
    connect(ui->actionExport_Database, SIGNAL(triggered()), this, SLOT(on_menuExportDatabase()));
    connect(ui->actionExport_Table, SIGNAL(triggered()), this, SLOT(on_menuExportTable()));
    connect(ui->actionImport_Database, SIGNAL(triggered()), this, SLOT(on_menuImportDatabase()));
    connect(ui->actionImport_Table, SIGNAL(triggered()), this, SLOT(on_menuImportTable()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_comboTable_currentIndexChanged(int index)
{
    if(ui->tableView->bChanges)
    {
        ui->tableView->bChanges = false;
        int res = QMessageBox::warning(this, "Commit Changes?",
                             "If you switch table without doing a commit, your changes will be lost!\nDo you want continue?",
                             QMessageBox::Save | QMessageBox::Discard);
        if(res == QMessageBox::Save)
            pem->commit();
    }
    pem->tableUse(ui->comboTable->currentText());
    ui->tableView->loadTable();
}

void MainWindow::on_buttonAdd_clicked()
{
    ui->tableView->addRecord();
}

void MainWindow::on_buttonRemove_clicked()
{
    ui->tableView->removeRecord();
}

void MainWindow::on_buttonCreate_clicked()
{
    create = new winCreate(pem);
    if(create->exec() == QDialog::Accepted)
        ui->comboTable->addItem(create->name);

}

void MainWindow::on_menuAboutQT()
{
    QMessageBox::aboutQt(0, "About...");
}

void MainWindow::on_menuAboutXSoftware()
{
    about = new xsAbout;
    about->show();
}

void MainWindow::on_menuImportTable()
{
    QString file;
    file = QFileDialog::getOpenFileName(nullptr, "Select file...", QDir::homePath(), "*.csv");
    if(!file.isEmpty())
        pem->importTable(QFileInfo(file).completeBaseName(), file);
    ui->comboTable->addItem(QFileInfo(file).completeBaseName());
}

void MainWindow::on_menuImportDatabase()
{
    QFileInfoList files;
    QStringList buffer;
    buffer = QFileDialog::getOpenFileNames(nullptr, "Select files...", QDir::homePath(), "*.csv");
    for(int i = 0; i < buffer.count(); i++)
        files.append(QFileInfo(buffer.at(i)));
    if(!files.isEmpty())
        pem->importDatabase(files);
}

void MainWindow::on_menuExportTable()
{
    QString file;
    file = QFileDialog::getSaveFileName(nullptr, "Select file...", QDir::homePath(), "*.csv");
    if(!file.isEmpty())
        pem->exportTable(file);
}

void MainWindow::on_menuExportDatabase()
{
    QString file = QFileDialog::getExistingDirectory(nullptr, "Select directory...", QDir::homePath());
    QDir dir(file);
    if(!file.isEmpty())
        pem->exportDatabase(dir);
}


void MainWindow::on_buttonSwitch_clicked()
{
    mode = !mode;
    if(!mode)
    {
        ui->comboTable->clear();
        ui->comboTable->addItems(pem->tableList());
        ui->tableView->loadTable();
    }
    else
    {
        ui->buttonCommit->setDisabled(true);
        ui->buttonAdd->setIcon(QIcon::fromTheme("list-add-user"));
        ui->buttonRemove->setIcon(QIcon::fromTheme("list-remove-user"));
        ui->comboTable->clear();
        ui->comboTable->addItem("Admin View");
        ui->tableView->adminTable();
    }
}

void MainWindow::on_buttonCommit_clicked()
{
    if(QMessageBox::warning(nullptr, "Commit?", "Red cells will be save with new value!\nDo you want continue?", QMessageBox::Save | QMessageBox::Discard) == QMessageBox::Save)
    {
        pem->commit();
        ui->tableView->bChanges = false;
        ui->tableView->loadTable();
    }
}

void MainWindow::on_buttonGenerate_clicked()
{
    generate = new winGenerate;
    generate->show();
}
