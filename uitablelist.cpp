#include "uitablelist.h"

uiTableList::uiTableList(QWidget *parent) :
    QWidget(parent)
{
    setupUi();
}


uiTableList::~uiTableList()
{
    //delete ui;
}

void uiTableList::setupUi()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("uiTableList"));
    this->resize(300, 640);
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    pushButtonAdd = new QPushButton(this);
    pushButtonAdd->setObjectName(QStringLiteral("pushButtonAdd"));
    QIcon icon;
    QString iconThemeName = QStringLiteral("insert-table");
    if (QIcon::hasThemeIcon(iconThemeName)) {
        icon = QIcon::fromTheme(iconThemeName);
    } else {
        icon.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
    }
    pushButtonAdd->setIcon(icon);
    pushButtonAdd->setIconSize(QSize(44, 44));
    pushButtonAdd->setFlat(true);

    gridLayout->addWidget(pushButtonAdd, 0, 0, 1, 1);

    QString iconThemeName2 = QStringLiteral("delete_table");
    if (QIcon::hasThemeIcon(iconThemeName2)) {
        icon = QIcon::fromTheme(iconThemeName2);
    } else {
        icon.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
    }

    pushButtonDelete = new QPushButton(this);
    pushButtonDelete->setObjectName(QStringLiteral("pushButtonDelete"));
    pushButtonDelete->setIcon(icon);
    pushButtonDelete->setIconSize(QSize(44, 44));
    pushButtonDelete->setFlat(true);

    gridLayout->addWidget(pushButtonDelete, 0, 1, 1, 1);

    treeWidget = new QTreeWidget(this);
    QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
    __qtreewidgetitem->setText(0,"Tables");
    treeWidget->setHeaderItem(__qtreewidgetitem);
    gridLayout->addWidget(treeWidget, 1, 0, 1, 2);


}
