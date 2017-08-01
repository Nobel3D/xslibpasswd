#include "sui.h"
#include <QMessageBox>
#include <QFileDialog>
#include <dialogjoin.h>
#include <wingenerate.h>
#include <wincreate.h>
#include <xsabout.h>

SUI::SUI(QObject *parent) : QObject(parent)
{
    sum = new SUM(LOGINFILE);
    DialogJoin uiJoin(sum);
    while(uiJoin.exec() != QDialog::Accepted);
    pem = new PEM(sum->getUser());
}

void SUI::init(QTableView *_view)
{
    view = _view;
    table = new QStandardItemModel;
    view->setModel(table);
}

int SUI::commit()
{
    if(!bChanges)
        return 0;
    if(QMessageBox::warning(nullptr, "Commit?", "Red cells will be save with new value!\nDo you want continue?", QMessageBox::Save | QMessageBox::Discard) == QMessageBox::Save)
    {
        pem->commit();
        bChanges = false;
        loadTable(pem->tableActive());
        return OK;
    }
    else
    {
        bChanges = false;
        pem->commitClear();
        return 0;
    }
}


void SUI::loadTable(const QString &table_name)
{
    commit();
    pem->tableUse(table_name);
    bAdmin = false;

    table->clear();
    QStringList fields = pem->tableField();

    for(int i = 0; i < fields.count(); i++)
        table->setHorizontalHeaderItem(i, new QStandardItem(fields.at(i)));

    for(int i = 0; i <= pem->db->getRecordCount(); i++)
        table->appendRow(getRow(i));
}

void SUI::adminTable()
{
    bAdmin = true;
    if(table != nullptr)
        delete table;
    QList<QSqlField> fields = sum->db->getFields();
    table = new QStandardItemModel(0,fields.count(),this);

    for(int i = 0; i < fields.count(); i++)
        table->setHorizontalHeaderItem(i, new QStandardItem(fields.at(i).name()));

    view->setModel(table);
    for(int i = 0; i <= sum->db->getRecordCount(); i++)
        table->appendRow(getRow(i));
}

QList<QStandardItem*> SUI::getRow(int index)
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

QStringList SUI::tableList()
{
    return pem->tableList();
}

qlonglong SUI::getID(qlonglong index)
{
    return table->index(index, 0).data().toInt();
}

void SUI::addRecord()
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

void SUI::removeRecord()
{
    qlonglong in = view->currentIndex().row();
    bChanges = true;
    if(bAdmin)
        sum->remove(getID(in));
    else
        pem->remove(getID(in));
    table->removeRow(in);
}

void SUI::updateRecord(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    bChanges = true;
    //topLeft == bottonRight
    table->setData(bottomRight, QBrush(QColor("#da4453")), Qt::BackgroundColorRole);
    if(bAdmin)
        sum->update(topLeft.column(), getID(topLeft.row()), topLeft.data());
    else
        pem->update(topLeft.column(), topLeft.data().toString(), getID(topLeft.row()));
}

void SUI::importTable()
{
    QString file;
    file = QFileDialog::getOpenFileName(nullptr, "Select file...", QDir::homePath(), "*.csv");
    if(!file.isEmpty())
        pem->importTable(QFileInfo(file).completeBaseName(), file); //TODO: NEXT LINE!!!
    //ui->comboTable->addItem(QFileInfo(file).completeBaseName());
}

void SUI::exportTable()
{
    QString file;
    file = QFileDialog::getSaveFileName(nullptr, "Select file...", QDir::homePath(), "*.csv");
    if(!file.isEmpty())
        pem->exportTable(file);
}

void SUI::exportDatabase()
{
    QString file = QFileDialog::getExistingDirectory(nullptr, "Select directory...", QDir::homePath());
    QDir dir(file);
    if(!file.isEmpty())
        pem->exportDatabase(dir);
}

void SUI::generate()
{
    winGenerate* generate = new winGenerate;
    generate->show();
}

void SUI::createTable()
{
    winCreate* create = new winCreate(pem);
    if(create->exec() == QDialog::Accepted); //TODO: NEXT LINE!!!
        //ui->comboTable->addItem(create->name);
}

void SUI::aboutQT()
{
    QMessageBox::aboutQt(0, "About...");
}

void SUI::aboutXSoftware()
{
    xsAbout* about = new xsAbout;
    about->show();
}
