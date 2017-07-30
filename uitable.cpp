#include "uitable.h"
#include <QDebug>

uiTable::uiTable(QWidget *parent)
    : QTableView(parent)
{
}

void uiTable::init(SUM *_sum, PEM *_pem)
{
    sum = _sum;
    pem = _pem;
}

void uiTable::loadTable()
{
    bAdmin = false;
    if(table != nullptr)
        delete table;
    QStringList fields = pem->tableField();
    table = new QStandardItemModel(0,fields.count(),this);

    for(int i = 0; i < fields.count(); i++)
        table->setHorizontalHeaderItem(i, new QStandardItem(fields.at(i)));

    setModel(table);
    for(int i = 0; i <= pem->db->getRecordCount(); i++)
        table->appendRow(getRow(i));
}

void uiTable::adminTable()
{
    bAdmin = true;
    if(table != nullptr)
        delete table;
    QList<QSqlField> fields = sum->db->getFields();
    table = new QStandardItemModel(0,fields.count(),this);

    for(int i = 0; i < fields.count(); i++)
        table->setHorizontalHeaderItem(i, new QStandardItem(fields.at(i).name()));

    setModel(table);
    for(int i = 0; i <= sum->db->getRecordCount(); i++)
        table->appendRow(getRow(i));
}

QList<QStandardItem*> uiTable::getRow(int index)
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

int uiTable::getID(int row)
{
    return table->index(row, 0).data().toInt();
}

void uiTable::addRecord()
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

void uiTable::removeRecord()
{
    bChanges = true;
    int buf = currentIndex().row();
    if(bAdmin)
        sum->remove(getID(buf));
    else
        pem->remove(getID(buf));
    table->removeRow(buf);
}

void uiTable::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    bChanges = true;
    //topLeft == bottonRight
    model()->setData(bottomRight, QBrush(QColor("#da4453")), Qt::BackgroundColorRole);
    if(bAdmin)
        sum->update(topLeft.column(), getID(topLeft.row()), topLeft.data());
    else
        pem->update(topLeft.column(), topLeft.data().toString(), getID(topLeft.row()));
}
