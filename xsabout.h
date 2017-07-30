#ifndef XSABOUT_H
#define XSABOUT_H

#include <QWidget>
#include "stronghold_global.h"

namespace Ui {
class xsAbout;
}

class STRONGHOLDSHARED_EXPORT xsAbout : public QWidget
{
    Q_OBJECT

public:
    explicit xsAbout(QWidget *parent = 0);
    ~xsAbout();

private:
    Ui::xsAbout *ui;
};

#endif // XSABOUT_H
