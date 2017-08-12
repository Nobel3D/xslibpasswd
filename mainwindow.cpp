#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <dialogjoin.h>
#include <wingenerate.h>
#include <wincreate.h>
#include <xsabout.h>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    sum = new SUM(LOGINFILE);
    DialogJoin uiJoin(sum);
    while(uiJoin.exec() != QDialog::Accepted);
    pem = new PEM(sum->getUser());

    /*   Plugin* s;
       QPluginLoader pluginLoader("example.so");
               QObject *plugin = pluginLoader.instance();
               if (plugin)
               {
                   s = qobject_cast<Plugin*>(plugin);
                   QMessageBox::warning(nullptr, "plugin", QString::number(s->use()));
               }
       */
    //QIcon::setThemeSearchPaths(QStringList(QStringLiteral("C:\\Users\\admin\\Desktop\\git\\xsPasswdRepo\\res\\")));
    ui->setupUi(this);
    table = new QStandardItemModel;
    ui->tableView->setModel(table);

    QStringList in = tableList();

    for(int i = 0; i < in.count(); i++)
     (new QTreeWidgetItem(ui->tables->treeWidget))->setText(0, in.at(i));

    connect(ui->actionImport_Table, SIGNAL(triggered()), this, SLOT(importTable()));
    connect(ui->actionExport_Database, SIGNAL(triggered()), this, SLOT(exportDatabase()));
    connect(ui->actionExport_Table, SIGNAL(triggered()), this, SLOT(exportTable()));
    connect(ui->actionAdd_Row, SIGNAL(triggered(bool)), this, SLOT(addRecord()));
    connect(ui->actionDelete_Row, SIGNAL(triggered(bool)), this, SLOT(removeRecord()));
    connect(ui->actionSave_Changes, SIGNAL(triggered(bool)), this, SLOT(commit()));
    connect(ui->actionAdd_Table, SIGNAL(triggered(bool)), this, SLOT(addTable()));
    connect(ui->actionDelete_Table, SIGNAL(triggered(bool)), this, SLOT(deleteTable()));
    connect(ui->actionPassword_Generator, SIGNAL(triggered(bool)), this, SLOT(generate()));
    connect(ui->actionSwitch_Mode, SIGNAL(triggered(bool)), this, SLOT(switchMode()));
    connect(ui->actionAbout_QT, SIGNAL(triggered(bool)), this, SLOT(aboutQT()));
    connect(ui->actionAbout_XSoftware, SIGNAL(triggered(bool)), this, SLOT(aboutXSoftware()));

    connect(ui->actions->pushButtonGenerate, SIGNAL(clicked(bool)), this, SLOT(generate()));
    connect(ui->actions->pushButtonAdd, SIGNAL(clicked(bool)), this, SLOT(addRecord()));
    connect(ui->actions->pushButtonDelete, SIGNAL(clicked(bool)), this, SLOT(removeRecord()));
    connect(ui->tables->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(loadTable(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->actions->pushButtonCommit, SIGNAL(clicked(bool)), this, SLOT(commit()));
    connect(ui->actions->pushButtonSwitch, SIGNAL(clicked(bool)), this, SLOT(switchMode()));
    connect(ui->tables->pushButtonAdd, SIGNAL(clicked(bool)), this, SLOT(addTable()));
    connect(ui->tables->pushButtonDelete, SIGNAL(clicked(bool)),  this, SLOT(deleteTable()));
    connect(ui->tableView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(updateRecord(QModelIndex,QModelIndex)));

    ui->tables->treeWidget->setCurrentItem(ui->tables->treeWidget->topLevelItem(0)); //TODO: set last item used if exist?
}

int MainWindow::commit()
{
    if(!bChanges)
        return 0;
    if(QMessageBox::warning(nullptr, "Commit?", "Red cells will be save with new value!\nDo you want continue?", QMessageBox::Save | QMessageBox::Discard) == QMessageBox::Save)
    {
        pem->commit();
        bChanges = false;
        databaseTable(pem->tableActive());
        return OK;
    }
    else
    {
        bChanges = false;
        pem->commitClear();
        return 0;
    }
}

void MainWindow::switchMode()
{
    if(bAdmin)
    {
        ui->actions->pushButtonCommit->setDisabled(false);
        ui->actions->pushButtonAdd->setIcon(xsUi::getFromIcons(QSL(":/icons/insert-table-row.svg")));
        ui->actions->pushButtonDelete->setIcon(xsUi::getFromIcons(QSL(":/icons/delete-table-row.svg")));
        ui->actions->pushButtonSwitch->setIcon(xsUi::getFromIcons(QSL(":/icons/system-switch-user.svg")));
        databaseTable(pem->tableActive());
    }
    else
    {
        ui->actions->pushButtonCommit->setDisabled(true);
        ui->actions->pushButtonAdd->setIcon(xsUi::getFromIcons(QSL(":/icons/list-add-user.svg")));
        ui->actions->pushButtonDelete->setIcon(xsUi::getFromIcons(QSL(":/icons/list-remove-user.svg")));
        ui->actions->pushButtonSwitch->setIcon(xsUi::getFromIcons(QSL(":/icons/table.svg")));
        adminTable();
    }
}

void MainWindow::databaseTable(const QString &tablename)
{
    commit();
    pem->tableUse(tablename);
    bAdmin = false;

    table->clear();
    QStringList fields = pem->tableField();

    for(int i = 0; i < fields.count(); i++)
        table->setHorizontalHeaderItem(i, new QStandardItem(fields.at(i)));

    for(int i = 0; i <= pem->db->getRecordCount(); i++)
        table->appendRow(getRow(i));
}

void MainWindow::loadTable(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(bAdmin)
        adminTable();
    else
        databaseTable(current->text(0));
}

void MainWindow::deleteTable()
{
    if(QMessageBox::warning(nullptr, "Commit?", "Deleting a table, all changes will be commited!\nDo you want continue?", QMessageBox::Save | QMessageBox::Discard) == QMessageBox::Save)
    {
        QTreeWidgetItem* in = ui->tables->treeWidget->currentItem();
        pem->tableDelete(in->text(0));
        pem->commit();
        delete in;
    }
}

void MainWindow::addTable()
{
    winCreate *create = new winCreate();
    connect(create, SIGNAL(getTableName(QString,QStringList)), this, SLOT(createTable(QString,QStringList)));
    if(create->exec() == QDialog::Accepted);
}

void MainWindow::adminTable()
{
    bAdmin = true;
    if(table != nullptr)
        delete table;
    QList<QSqlField> fields = sum->db->getFields();
    table = new QStandardItemModel(0,fields.count(),this);

    for(int i = 0; i < fields.count(); i++)
        table->setHorizontalHeaderItem(i, new QStandardItem(fields.at(i).name()));

    ui->tableView->setModel(table);
    for(int i = 0; i <= sum->db->getRecordCount(); i++)
        table->appendRow(getRow(i));
}

QList<QStandardItem*> MainWindow::getRow(int index)
{
    QList<QStandardItem*> out;
    if(bAdmin)
    {
        QList<QVariant> in = sum->db->getRow(index);
        for(int x = 0; x < in.count(); x++)
            out.insert(x,new QStandardItem(in.at(x).toString()));
    }
    else
    {
        QStringList in = pem->get(index);
        for(int i = 0; i < in.count(); i++)
        {
            out.insert(i,new QStandardItem(in.at(i)));

        }
    }
    return out;
}

QStringList MainWindow::tableList()
{
    return pem->tableList();
}

qlonglong MainWindow::getID(qlonglong index)
{
    return table->index(index, 0).data().toInt();
}

void MainWindow::addRecord()
{
    bChanges = true;
    QStringList values;
    QList<QStandardItem*> out;
    out.append(new QStandardItem(QString::number(getID(table->rowCount() - 1) + 1)));
    out.at(0)->setData(QBrush(QColor("#da4453")), Qt::BackgroundColorRole);
    if(bAdmin)
    {
        sum->add();
        for(int i = 1; i < sum->db->getFieldCount(); i++)
            out.append(new QStandardItem(""));
    }
    else
    {
        pem->add();
        for(int i = 1; i < pem->db->getFieldCount(); i++)
            out.append(new QStandardItem(""));
    }

    table->appendRow(out);

}

void MainWindow::removeRecord()
{
    qlonglong in = ui->tableView->currentIndex().row();
    bChanges = true;
    if(bAdmin)
        sum->remove(getID(in));
    else
        pem->remove(getID(in));
    table->removeRow(in);
}

void MainWindow::updateRecord(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    bChanges = true;
    //topLeft == bottonRight
    table->setData(bottomRight, QBrush(QColor("#da4453")), Qt::BackgroundColorRole);
    if(bAdmin)
        sum->update(topLeft.column(), getID(topLeft.row()), topLeft.data());
    else
        pem->update(topLeft.column(), topLeft.data().toString(), getID(topLeft.row()));
}

void MainWindow::importTable()
{
    QString file;
    file = QFileDialog::getOpenFileName(nullptr, "Select file...", QDir::homePath(), "*.csv");
    if(!file.isEmpty())
        pem->importTable(QFileInfo(file).completeBaseName(), file); //TODO: NEXT LINE!!!
    //ui->comboTable->addItem(QFileInfo(file).completeBaseName());
}

void MainWindow::exportTable()
{
    QString file;
    file = QFileDialog::getSaveFileName(nullptr, "Select file...", QDir::homePath(), "*.csv");
    if(!file.isEmpty())
        pem->exportTable(file);
}

void MainWindow::exportDatabase()
{
    QString file = QFileDialog::getExistingDirectory(nullptr, "Select directory...", QDir::homePath());
    QDir dir(file);
    if(!file.isEmpty())
        pem->exportDatabase(dir);
}

void MainWindow::generate()
{
    winGenerate* generate = new winGenerate;
    generate->show();
}

void MainWindow::createTable(const QString &tablename, const QStringList &fieldname)
{
    if(QMessageBox::warning(nullptr, "Commit?", "Creating a new table, all changes will be commited!\nDo you want continue?", QMessageBox::Save | QMessageBox::Discard) == QMessageBox::Save)
    {
        QTreeWidgetItem *buffer = new QTreeWidgetItem;
        buffer->setText(0,tablename);
        pem->tableCreate(tablename, fieldname);
        ui->tables->treeWidget->addTopLevelItem(buffer);
        pem->commit();
    }
}

void MainWindow::aboutQT()
{
    QMessageBox::aboutQt(0, "About...");
}

void MainWindow::aboutXSoftware()
{
    xsAbout* about = new xsAbout;
    about->show();
}
