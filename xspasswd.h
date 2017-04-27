#ifndef XSLIBPASSWD_H
#define XSLIBPASSWD_H

#include <QString>
#include <xslib/xslib.h>

#define HOME_PATH "/home/"
#define PWFILE "/.xspasswd"
#define DBFILE "/.xsdata.db"
#define MAXHIT 6
#include "xspasswd_global.h"

class XSPASSWDSHARED_EXPORT xsPasswd
{

public:
    xsPasswd();

    QString mainUsage();
    QString mainStatus();
    void dataAdd(QString arg);
    void dataGet(QString arg);
    void tableUse(QString arg);
    int userCreate(QString user);
    int userJoin(QString tmpuser);
    int userPasswd(QString key);
    QByteArray hashkey(QString& key);
    QString strUser = "tmp";
    QString strBuffer = "";

    xsBlowfish* blowfish;
    xsDatabase* database;

};

#endif // XSLIBPASSWD_H
