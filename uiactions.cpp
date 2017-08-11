#include "uiactions.h"

uiActions::uiActions(QWidget *parent) :
    QWidget(parent)
{
    setupUi();

}

uiActions::~uiActions()
{
}

void uiActions::setupUi()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("uiActions"));
    this->resize(100, 640);
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    pushButtonSwitch = new QPushButton(this);
    pushButtonSwitch->setObjectName(QStringLiteral("pushButtonSwitch"));
    QIcon icon;
    QString iconThemeName = QStringLiteral("view-refresh");
    if (QIcon::hasThemeIcon(iconThemeName)) {
        icon = QIcon::fromTheme(iconThemeName);
    } else {
        icon.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
    }
    pushButtonSwitch->setIcon(icon);
    pushButtonSwitch->setIconSize(QSize(44, 44));
    pushButtonSwitch->setFlat(true);

    verticalLayout->addWidget(pushButtonSwitch);

    pushButtonAdd = new QPushButton(this);
    pushButtonAdd->setObjectName(QStringLiteral("pushButtonAdd"));
    QIcon icon1;
    iconThemeName = QStringLiteral("insert-table-row");
    if (QIcon::hasThemeIcon(iconThemeName)) {
        icon1 = QIcon::fromTheme(iconThemeName);
    } else {
        icon1.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
    }
    pushButtonAdd->setIcon(icon1);
    pushButtonAdd->setIconSize(QSize(44, 44));
    pushButtonAdd->setFlat(true);

    verticalLayout->addWidget(pushButtonAdd);

    pushButtonDelete = new QPushButton(this);
    pushButtonDelete->setObjectName(QStringLiteral("pushButtonDelete"));
    QIcon icon2;
    iconThemeName = QStringLiteral("delete-table-row");
    if (QIcon::hasThemeIcon(iconThemeName)) {
        icon2 = QIcon::fromTheme(iconThemeName);
    } else {
        icon2.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
    }
    pushButtonDelete->setIcon(icon2);
    pushButtonDelete->setIconSize(QSize(44, 44));
    pushButtonDelete->setFlat(true);

    verticalLayout->addWidget(pushButtonDelete);

    pushButtonGenerate = new QPushButton(this);
    pushButtonGenerate->setObjectName(QStringLiteral("pushButtonGenerate"));
    QIcon icon3;
    iconThemeName = QStringLiteral("password-copy");
    if (QIcon::hasThemeIcon(iconThemeName)) {
        icon3 = QIcon::fromTheme(iconThemeName);
    } else {
        icon3.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
    }
    pushButtonGenerate->setIcon(icon3);
    pushButtonGenerate->setIconSize(QSize(44, 44));
    pushButtonGenerate->setFlat(true);

    verticalLayout->addWidget(pushButtonGenerate);

    pushButtonCommit = new QPushButton(this);
    pushButtonCommit->setObjectName(QStringLiteral("pushButtonCommit"));
    QIcon icon4;
    iconThemeName = QStringLiteral("document-save");
    if (QIcon::hasThemeIcon(iconThemeName)) {
        icon4 = QIcon::fromTheme(iconThemeName);
    } else {
        icon4.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
    }
    pushButtonCommit->setIcon(icon4);
    pushButtonCommit->setIconSize(QSize(44, 44));
    pushButtonCommit->setFlat(true);

    verticalLayout->addWidget(pushButtonCommit);
}
