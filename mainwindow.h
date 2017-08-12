#ifndef MainWindow_H
#define MainWindow_H

#include "pem.h"
#include "sum.h"
#include "dialogjoin.h"
#include "wincreate.h"
#include "wingenerate.h"
#include "xsabout.h"
#include "stronghold_global.h"
#include "uitablelist.h"
#include "uiactions.h"
#include "plugin.h"

#include <QMainWindow>
#include <QTreeWidget>
#include <QTableView>
#include <QItemSelectionModel>
#include <QStandardItemModel>


class STRONGHOLDSHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void adminTable();
    void databaseTable(const QString &tablename);
    QList<QStandardItem*> getRow(int index);
    qlonglong getID(qlonglong index);

    QStringList tableList();

    void setupUi();
    void retranslateUi();

    bool bChanges = false;
    bool bAdmin = false;
    QStandardItemModel* table;
    SUM* sum;
    PEM* pem;

    QAction *actionImport_Table;
    QAction *actionImport_Database;
    QAction *actionExport_Table;
    QAction *actionExport_Database;
    QAction *actionGenerator;
    QAction *actionAbout_XSoftware;
    QAction *actionAbout_QT;
    QAction *actionAdminMode;
    QAction *actionDatabaseMode;
    QAction *actionAdd_Row;
    QAction *actionDelete_Row;
    QAction *actionSave_Changes;
    QAction *actionPassword_Generator;
    QAction *actionSwitch_Mode;
    QAction *actionAdd_Table;
    QAction *actionDelete_Table;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTableView *tableView;
    uiActions *actions;
    uiTableList *tables;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuEdit;
    QMenu *menuTools;

signals:

public slots:
    int commit();
    void switchMode();
    void loadTable(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void addRecord();
    void removeRecord();
    void updateRecord(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void importTable();
    void exportTable();
    void exportDatabase();
    void generate();
    void createTable(const QString &tablename, const QStringList &fieldname);
    void deleteTable();
    void addTable();
    void aboutQT();
    void aboutXSoftware();
};

#endif // MainWindow_H
