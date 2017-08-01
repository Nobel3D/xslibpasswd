#ifndef SUI_H
#define SUI_H

#include "pem.h"
#include "sum.h"

#include <QTableView>
#include <QItemSelectionModel>
#include <QStandardItemModel>

class SUI : public QObject
{
    Q_OBJECT
public:
    explicit SUI(QObject *parent = nullptr);
    void init(QTableView* _view);
    void adminTable();
    QList<QStandardItem*> getRow(int index);
    qlonglong getID(qlonglong index);

    QStringList tableList();

    bool bChanges = false;
    bool bAdmin = false;
    QStandardItemModel* table;
    QTableView* view;
    SUM* sum;
    PEM* pem;

signals:

public slots:
    int commit();
    void loadTable(const QString &table_name);
    void addRecord();
    void removeRecord();
    void updateRecord(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void importTable();
    void exportTable();
    void exportDatabase();
    void generate();
    void createTable();
    void aboutQT();
    void aboutXSoftware();
};

#endif // SUI_H
