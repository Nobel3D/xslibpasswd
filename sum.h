#ifndef SUM_H
#define SUM_H

#include "stronghold_global.h"
#include "user.h"

class STRONGHOLDSHARED_EXPORT SUM
{
public:
    SUM(const QString& _db);

    bool login(const QString &name, const QString &password);
    bool isFirstRun();
    QString hit();

    QStringList get(int index);
    QStringList getFields();

    bool add(const QString &name, const QString &password, const QString &database, int level);
    int add();
    bool addRoot(const QString &name, const QString &password, const QString &database);

    bool update(int field, int row, const QVariant &value);
    bool remove(qlonglong id); //TODO: Add privilege level

    QStringList getUsers();
    User getUser(int id);
    User* getUser();

    xsPassword pwhit;
    xsDatabase* db = nullptr;
    User* user = nullptr;
};

#endif // SUM_H
