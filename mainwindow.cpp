#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <dialogjoin.h>
#include <wingenerate.h>
#include <wincreate.h>
#include <xsabout.h>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
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
    //QIcon::setThemeSearchPaths(QStringList(QSL("C:\\Users\\admin\\Desktop\\git\\xsPasswdRepo\\res\\")));
    setupUi();
    table = new QStandardItemModel;
    tableView->setModel(table);

    QStringList in = tableList();

    for(int i = 0; i < in.count(); i++)
     (new QTreeWidgetItem(tables->treeWidget))->setText(0, in.at(i));

    connect(actionImport_Table, SIGNAL(triggered()), this, SLOT(importTable()));
    connect(actionExport_Database, SIGNAL(triggered()), this, SLOT(exportDatabase()));
    connect(actionExport_Table, SIGNAL(triggered()), this, SLOT(exportTable()));
    connect(actionAdd_Row, SIGNAL(triggered(bool)), this, SLOT(addRecord()));
    connect(actionDelete_Row, SIGNAL(triggered(bool)), this, SLOT(removeRecord()));
    connect(actionSave_Changes, SIGNAL(triggered(bool)), this, SLOT(commit()));
    connect(actionAdd_Table, SIGNAL(triggered(bool)), this, SLOT(addTable()));
    connect(actionDelete_Table, SIGNAL(triggered(bool)), this, SLOT(deleteTable()));
    connect(actionPassword_Generator, SIGNAL(triggered(bool)), this, SLOT(generate()));
    connect(actionSwitch_Mode, SIGNAL(triggered(bool)), this, SLOT(switchMode()));
    connect(actionAbout_QT, SIGNAL(triggered(bool)), this, SLOT(aboutQT()));
    connect(actionAbout_XSoftware, SIGNAL(triggered(bool)), this, SLOT(aboutXSoftware()));

    connect(actions->pushButtonGenerate, SIGNAL(clicked(bool)), this, SLOT(generate()));
    connect(actions->pushButtonAdd, SIGNAL(clicked(bool)), this, SLOT(addRecord()));
    connect(actions->pushButtonDelete, SIGNAL(clicked(bool)), this, SLOT(removeRecord()));
    connect(tables->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(loadTable(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(actions->pushButtonCommit, SIGNAL(clicked(bool)), this, SLOT(commit()));
    connect(actions->pushButtonSwitch, SIGNAL(clicked(bool)), this, SLOT(switchMode()));
    connect(tables->pushButtonAdd, SIGNAL(clicked(bool)), this, SLOT(addTable()));
    connect(tables->pushButtonDelete, SIGNAL(clicked(bool)),  this, SLOT(deleteTable()));
    connect(tableView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(updateRecord(QModelIndex,QModelIndex)));

    tables->treeWidget->setCurrentItem(tables->treeWidget->topLevelItem(0)); //TODO: set last item used if exist?
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
        actions->pushButtonCommit->setDisabled(false);
        actions->pushButtonAdd->setIcon(xsUi::getFromIcons(QSL(":/icons/insert-table-row.svg")));
        actions->pushButtonDelete->setIcon(xsUi::getFromIcons(QSL(":/icons/delete-table-row.svg")));
        actions->pushButtonSwitch->setIcon(xsUi::getFromIcons(QSL(":/icons/system-switch-user.svg")));
        databaseTable(pem->tableActive());
    }
    else
    {
        actions->pushButtonCommit->setDisabled(true);
        actions->pushButtonAdd->setIcon(xsUi::getFromIcons(QSL(":/icons/list-add-user.svg")));
        actions->pushButtonDelete->setIcon(xsUi::getFromIcons(QSL(":/icons/list-remove-user.svg")));
        actions->pushButtonSwitch->setIcon(xsUi::getFromIcons(QSL(":/icons/table.svg")));
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
        QTreeWidgetItem* in = tables->treeWidget->currentItem();
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

    tableView->setModel(table);
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
    qlonglong in = tableView->currentIndex().row();
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
    //comboTable->addItem(QFileInfo(file).completeBaseName());
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
        tables->treeWidget->addTopLevelItem(buffer);
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

void MainWindow::setupUi()
{
    if (objectName().isEmpty())
        setObjectName(QSL("MainWindow"));
    resize(1000, 800);
    setMinimumSize(QSize(400, 400));

    setWindowIcon(QIcon(QSL(":/xs/shield.png")));
    actionImport_Table = new QAction(this);
    actionImport_Table->setObjectName(QSL("actionImport_Table"));
    actionImport_Table->setIcon(xsUi::getFromIcons(QSL(":/icons/document-import.svg")));
    actionImport_Database = new QAction(this);
    actionImport_Database->setObjectName(QSL("actionImport_Database"));
    actionExport_Table = new QAction(this);
    actionExport_Table->setObjectName(QSL("actionExport_Table"));

    actionExport_Table->setIcon(xsUi::getFromIcons(QSL(":/icons/document-export.svg")));
    actionExport_Database = new QAction(this);
    actionExport_Database->setObjectName(QSL("actionExport_Database"));

    actionExport_Database->setIcon(xsUi::getFromIcons(QSL(":/icons/network-server-database.svg")));
    actionGenerator = new QAction(this);
    actionGenerator->setObjectName(QSL("actionGenerator"));
    actionAbout_XSoftware = new QAction(this);
    actionAbout_XSoftware->setObjectName(QSL("actionAbout_XSoftware"));

    actionAbout_XSoftware->setIcon(QIcon(QSL(":/xs/men.png")));
    actionAbout_QT = new QAction(this);
    actionAbout_QT->setObjectName(QSL("actionAbout_QT"));

    actionAbout_QT->setIcon(xsUi::getFromIcons(QSL(":/icons/qtcreator.svg")));
    actionAdminMode = new QAction(this);
    actionAdminMode->setObjectName(QSL("actionAdminMode"));
    actionDatabaseMode = new QAction(this);
    actionDatabaseMode->setObjectName(QSL("actionDatabaseMode"));
    actionAdd_Row = new QAction(this);
    actionAdd_Row->setObjectName(QSL("actionAdd_Row"));

    actionAdd_Row->setIcon(xsUi::getFromIcons(QSL(":/icons/insert-table-row.svg")));
    actionDelete_Row = new QAction(this);
    actionDelete_Row->setObjectName(QSL("actionDelete_Row"));

    actionDelete_Row->setIcon(xsUi::getFromIcons(QSL(":/icons/delete-table-row.svg")));
    actionSave_Changes = new QAction(this);
    actionSave_Changes->setObjectName(QSL("actionSave_Changes"));

    actionSave_Changes->setIcon(xsUi::getFromIcons(QSL(":/icons/document-save.svg")));
    actionPassword_Generator = new QAction(this);
    actionPassword_Generator->setObjectName(QSL("actionPassword_Generator"));

    actionPassword_Generator->setIcon(xsUi::getFromIcons(QSL(":/icons/password-generate.svg")));
    actionSwitch_Mode = new QAction(this);
    actionSwitch_Mode->setObjectName(QSL("actionSwitch_Mode"));

    actionSwitch_Mode->setIcon(xsUi::getFromIcons(QSL(":/icons/system-switch-user.svg")));
    actionAdd_Table = new QAction(this);
    actionAdd_Table->setObjectName(QSL("actionAdd_Table"));

    actionAdd_Table->setIcon(xsUi::getFromIcons(QSL(":/icons/insert-table.svg")));
    actionDelete_Table = new QAction(this);
    actionDelete_Table->setObjectName(QSL("actionDelete_Table"));

    actionDelete_Table->setIcon(xsUi::getFromIcons(QSL(":/icons/delete_table.svg")));
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QSL("centralWidget"));
    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QSL("gridLayout"));
    tableView = new QTableView(centralWidget);
    tableView->setObjectName(QSL("tableView"));

    gridLayout->addWidget(tableView, 0, 2, 1, 1);

    actions = new uiActions(centralWidget);
    actions->setObjectName(QSL("actions"));
    actions->setMinimumSize(QSize(100, 0));

    gridLayout->addWidget(actions, 0, 3, 1, 1);

    tables = new uiTableList(centralWidget);
    tables->setObjectName(QSL("tables"));
    tables->setMinimumSize(QSize(100, 0));

    gridLayout->addWidget(tables, 0, 0, 1, 1);

    setCentralWidget(centralWidget);
    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QSL("statusBar"));
    setStatusBar(statusBar);
    menuBar = new QMenuBar(this);
    menuBar->setObjectName(QSL("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 1000, 21));
    menuFile = new QMenu(menuBar);
    menuFile->setObjectName(QSL("menuFile"));
    menuHelp = new QMenu(menuBar);
    menuHelp->setObjectName(QSL("menuHelp"));
    menuEdit = new QMenu(menuBar);
    menuEdit->setObjectName(QSL("menuEdit"));
    menuTools = new QMenu(menuBar);
    menuTools->setObjectName(QSL("menuTools"));
    setMenuBar(menuBar);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuEdit->menuAction());
    menuBar->addAction(menuTools->menuAction());
    menuBar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionImport_Table);
    menuFile->addAction(actionExport_Table);
    menuFile->addAction(actionExport_Database);
    menuHelp->addAction(actionAbout_XSoftware);
    menuHelp->addAction(actionAbout_QT);
    menuEdit->addAction(actionAdd_Row);
    menuEdit->addAction(actionDelete_Row);
    menuEdit->addAction(actionSave_Changes);
    menuEdit->addSeparator();
    menuEdit->addAction(actionAdd_Table);
    menuEdit->addAction(actionDelete_Table);
    menuTools->addAction(actionPassword_Generator);
    menuTools->addAction(actionSwitch_Mode);

    retranslateUi();
}

void MainWindow::retranslateUi()
{
    setWindowTitle(QApplication::translate("MainWindow", "Stronghold", Q_NULLPTR));
    actionImport_Table->setText(QApplication::translate("MainWindow", "&Import Table", Q_NULLPTR));
    actionImport_Database->setText(QApplication::translate("MainWindow", "Import &Database", Q_NULLPTR));
    actionExport_Table->setText(QApplication::translate("MainWindow", "&Export Table", Q_NULLPTR));
    actionExport_Database->setText(QApplication::translate("MainWindow", "E&xport Database", Q_NULLPTR));
    actionGenerator->setText(QApplication::translate("MainWindow", "&Generator", Q_NULLPTR));
    actionAbout_XSoftware->setText(QApplication::translate("MainWindow", "&About XSoftware", Q_NULLPTR));
    actionAbout_QT->setText(QApplication::translate("MainWindow", "About &Qt Framework", Q_NULLPTR));
    actionAdminMode->setText(QApplication::translate("MainWindow", "&Switch Admin Mode", Q_NULLPTR));
    actionDatabaseMode->setText(QApplication::translate("MainWindow", "Switch &Database Mode", Q_NULLPTR));
    actionAdd_Row->setText(QApplication::translate("MainWindow", "Add Row", Q_NULLPTR));
    actionDelete_Row->setText(QApplication::translate("MainWindow", "Delete Row", Q_NULLPTR));
    actionSave_Changes->setText(QApplication::translate("MainWindow", "Save Changes", Q_NULLPTR));
    actionPassword_Generator->setText(QApplication::translate("MainWindow", "Password Generator", Q_NULLPTR));
    actionSwitch_Mode->setText(QApplication::translate("MainWindow", "Switch Mode", Q_NULLPTR));
    actionAdd_Table->setText(QApplication::translate("MainWindow", "Add Table", Q_NULLPTR));
    actionDelete_Table->setText(QApplication::translate("MainWindow", "Delete Table", Q_NULLPTR));
    menuFile->setTitle(QApplication::translate("MainWindow", "Fi&le", Q_NULLPTR));
    menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
    menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", Q_NULLPTR));
    menuTools->setTitle(QApplication::translate("MainWindow", "Tools", Q_NULLPTR));
}
