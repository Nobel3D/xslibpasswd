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

    pushButtonSwitch->setIcon(xsUi::getFromIcons(QSL(":/icons/system-switch-user.svg")));
    pushButtonSwitch->setIconSize(QSize(44, 44));
    pushButtonSwitch->setFlat(true);

    verticalLayout->addWidget(pushButtonSwitch);

    pushButtonAdd = new QPushButton(this);
    pushButtonAdd->setObjectName(QStringLiteral("pushButtonAdd"));

    pushButtonAdd->setIcon(xsUi::getFromIcons(QSL(":/icons/insert-table-row.svg")));
    pushButtonAdd->setIconSize(QSize(44, 44));
    pushButtonAdd->setFlat(true);

    verticalLayout->addWidget(pushButtonAdd);

    pushButtonDelete = new QPushButton(this);
    pushButtonDelete->setObjectName(QStringLiteral("pushButtonDelete"));

    pushButtonDelete->setIcon(xsUi::getFromIcons(QSL(":/icons/delete-table-row.svg")));
    pushButtonDelete->setIconSize(QSize(44, 44));
    pushButtonDelete->setFlat(true);

    verticalLayout->addWidget(pushButtonDelete);

    pushButtonGenerate = new QPushButton(this);
    pushButtonGenerate->setObjectName(QStringLiteral("pushButtonGenerate"));

    pushButtonGenerate->setIcon(xsUi::getFromIcons(QSL(":/icons/password-generate.svg")));
    pushButtonGenerate->setIconSize(QSize(44, 44));
    pushButtonGenerate->setFlat(true);

    verticalLayout->addWidget(pushButtonGenerate);

    pushButtonCommit = new QPushButton(this);
    pushButtonCommit->setObjectName(QStringLiteral("pushButtonCommit"));

    pushButtonCommit->setIcon(xsUi::getFromIcons(QSL(":/icons/document-save.svg")));
    pushButtonCommit->setIconSize(QSize(44, 44));
    pushButtonCommit->setFlat(true);

    verticalLayout->addWidget(pushButtonCommit);
}
