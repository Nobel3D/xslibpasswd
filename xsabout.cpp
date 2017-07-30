#include "xsabout.h"
#include "ui_xsabout.h"
#include <QGraphicsPixmapItem>
#include <xslib/xslib_global.h>
#include <libStronghold/stronghold_global.h>

xsAbout::xsAbout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::xsAbout)
{
    ui->setupUi(this);
    QString html1 = "<html><head/><body><p><span style=\" font-weight:600; color:#ffb442;\">";
    QString html2 = "</span></p></body></html>";
    ui->label_libsh_v->setText(html1 + XSLIB_STRONGHOLD  + html2);
    ui->label_xslib_v->setText(html1 + XSLIB  + html2);
    //QGraphicsScene* scene = new QGraphicsScene();
    //scene->addItem(new QGraphicsPixmapItem(QPixmap(":/resource/res/men.png")));
    //ui->graphicsView->setScene(scene);
}

xsAbout::~xsAbout()
{
    delete ui;
}
