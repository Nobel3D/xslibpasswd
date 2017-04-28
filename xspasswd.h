#ifndef XSLIBPASSWD_H
#define XSLIBPASSWD_H

#include <QString>
#include <xslib/xslib.h>

#define PWFILE "/.xspasswd"
#define DBFILE "/.xsdata.db"

#include "xspasswd_global.h"

class XSPASSWDSHARED_EXPORT xsPasswd
{

public:
    xsPasswd();

    QString mainUsage();
    int dataAdd(QStringList &arg);
    QStringList dataGet(const QStringList& arg);
    int tableUse(const QString &table);
    int tableCreate(const QString &table, const QStringList &fields);
    int userCreate(const QString &passwd);
    int userJoin(const QString &passwd);
    int userPasswd(const QString &key);
    QByteArray hashkey(const QString &key);

    QString strStatus;
    int iMaxHit = 6;
    int iHit = 0;

    xsBlowfish* blowfish;
    xsDatabase* database;

};

#endif // XSLIBPASSWD_H
