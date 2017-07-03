#ifndef STRONGHOLD_H
#define STRONGHOLD_H

#include "stronghold_global.h"
#include "login.h"

class STRONGHOLDSHARED_EXPORT Stronghold : public xsObject
{

public:
    Stronghold(const QString &manager);

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
    bool userCreate(const QString &name, const xsPassword &passwd, const QString &file);
    bool userJoin(const QString &user, const QString &passwd);
    QString generatePassword(const QStringList &arg);

    QVariant decode(const QVariant& encoded);
    QVariant encode(const QVariant& decoded);

    QStringList convert(const QList<QVariant>& data);

    bool exportTable(const QString &dir);
    bool exportDatabase(const QDir &dir);
    bool importTable(const QString &name, const QString &dir);
    bool importDatabase(const QFileInfoList &dir);

    xsBlowfish* blowfish;
    xsDatabase* database;

    Login* login;

};

#endif // STRONGHOLD_H
