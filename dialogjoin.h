#ifndef DIALOGJOIN_H
#define DIALOGJOIN_H

#include <QDialog>
#include "stronghold_global.h"
#include "sum.h"

namespace Ui {
class DialogJoin;
}

class STRONGHOLDSHARED_EXPORT DialogJoin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogJoin(SUM *_sum, QWidget *parent = 0);
    ~DialogJoin();
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogJoin *ui;
    SUM* sum;
    User* user;
};

#endif // DIALOGJOIN_H
