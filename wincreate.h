#ifndef WINCREATE_H
#define WINCREATE_H

#include <QDialog>
#include "stronghold_global.h"
#include <QStandardItemModel>
#include "pem.h"

namespace Ui {
class winCreate;
}

class STRONGHOLDSHARED_EXPORT winCreate : public QDialog
{
    Q_OBJECT

public:
    explicit winCreate(PEM* _pem, QWidget *parent = 0);
    ~winCreate();

    QString name;
private slots:
    void on_pushAdd_clicked();

    void on_pushRemove_clicked();

    void on_buttonBox_accepted();

private:
    Ui::winCreate *ui;
    PEM* pem;
    QStandardItemModel* list;
};

#endif // WINCREATE_H