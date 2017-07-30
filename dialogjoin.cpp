#include "dialogjoin.h"
#include "ui_dialogjoin.h"
#include <QMessageBox>
#include <QComboBox>

DialogJoin::DialogJoin(SUM* _sum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogJoin)
{
    sum = _sum;
    ui->setupUi(this);
    ui->comboBox->addItems(sum->getUsers());
}

DialogJoin::~DialogJoin()
{
    delete ui;
}


void DialogJoin::on_buttonBox_accepted()
{
    if(!sum->login(ui->comboBox->currentText(), ui->linePassword->text()))
    {
        ui->label_hit->setText(sum->hit());
        reject();
    }
}

void DialogJoin::on_buttonBox_rejected()
{
    exit(0);
}
