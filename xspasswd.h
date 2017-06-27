#ifndef XSLIBPASSWD_H
#define XSLIBPASSWD_H

#include <QString>
#include <QDir>
#include <xslib/xslib.h>

#define HOME QDir::homePath()
#define PWFILE HOME + "/.xspasswd"
#define DBFILE HOME + "/.xsdata.db"

#include "xspasswd_global.h"

class XSPASSWDSHARED_EXPORT xsPasswd : public xsObject
{

public:
    xsPasswd();

    int dataAdd(QStringList &arg);
    int dataAdd(const QStringList &fields, const QStringList &values);
    QStringList dataGet(const QString& field, const QString& value);
    QStringList dataGet(const QString& field);
    QStringList dataGet(int row);
    int dataUpdate(const QString &field, const QString &oldvalue, const QString &newvalue);
    int dataUpdate(const QString &field, const QString &oldvalue, int id);
    bool dataDelete(qlonglong id);
    int tableUse(const QString &table);
    int tableCreate(const QString &table, const QStringList &fields);
    QStringList tableList();
    QStringList tableField();
    bool tableActive();
    int userCreate(const xsPassword &passwd, const QString &file);
    int userJoin(const xsPassword &passwd);
    int loadPassword(const QString &filepw);
    QString generatePassword(const QStringList &arg);

    QVariant decode(const QVariant& encoded);
    QVariant encode(const QVariant& decoded);

    QStringList convert(const QList<QVariant>& data);

    xsBlowfish* blowfish;
    xsDatabase* database;
    xsPassword* password;

};

#endif // XSLIBPASSWD_H
