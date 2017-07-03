#ifndef LOGIN_H
#define LOGIN_H

#include "stronghold_global.h"

class Login
{
public:
    Login(const QString& _manager);
    QString login(const QString &user, const xsPassword &password);
    bool userAdd(const QString &name, const xsPassword &password, const QString &database, int level);
    //bool userRemove(const QString &name, const xsPassword &password); TODO: Add privilege level

    QStringList getUsers();

    xsDatabase* manager;
};

#endif // LOGIN_H
