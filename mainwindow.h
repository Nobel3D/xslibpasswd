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

namespace Ui {
class MainWindow;
}

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

    bool bChanges = false;
    bool bAdmin = false;
    QStandardItemModel* table;
    Ui::MainWindow *ui;
    SUM* sum;
    PEM* pem;

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
