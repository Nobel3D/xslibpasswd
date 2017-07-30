#include "wingenerate.h"
#include "ui_wingenerate.h"
#include <xslib/xslib.h>
#include <QCheckBox>

winGenerate::winGenerate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::winGenerate)
{
    ui->setupUi(this);
}

winGenerate::~winGenerate()
{
    delete ui;
}

void winGenerate::on_pushGenerate_clicked() //TODO: lock button when working and show time elapsed
{
    ui->lineEdit->setText(xsPassword::generate(ui->spinLength->value(), ui->checkSymb->isChecked(),
                                               ui->checkSpaces->isChecked(),ui->checkUni->isChecked(),
                                               ui->checkNum->isChecked(), ui->checkLowers->isChecked(),
                                               ui->checkUppers->isChecked()));
}
