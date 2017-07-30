#ifndef UITABLE_H
#define UITABLE_H

#include <QTableView>
#include <QItemSelectionModel>
#include <QStandardItemModel>

#include "stronghold_global.h"
#include "pem.h"
#include "sum.h"

class STRONGHOLDSHARED_EXPORT uiTable : public QTableView
{
    Q_OBJECT

public:
    explicit uiTable(QWidget *parent = nullptr);
    void init(SUM *_sum, PEM *_pem);
    void addRecord();
    void removeRecord();
    void loadTable();
    void adminTable();
    bool bChanges = false;
protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()) override;
private:
    QStandardItemModel* table = nullptr;
    PEM* pem;
    SUM* sum;
    QList<QStandardItem*> getRow(int index);
    bool bAdmin = false;
    int getID(int row);
};

#endif // UITABLE_H
