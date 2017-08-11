#ifndef UIACTIONS_H
#define UIACTIONS_H

#include <QWidget>
#include <QDockWidget>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>

#include "stronghold_global.h"



class STRONGHOLDSHARED_EXPORT uiActions : public QWidget
{
    Q_OBJECT

public:
    explicit uiActions(QWidget *parent = 0);
    ~uiActions();

    QVBoxLayout *verticalLayout;
    QPushButton *pushButtonSwitch;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonDelete;
    QPushButton *pushButtonGenerate;
    QPushButton *pushButtonCommit;

    void setupUi();
};

#endif // UIACTIONS_H
