#ifndef PEM_H
#define PEM_H

#include "stronghold.h"
#include "stronghold_global.h"
#include "user.h"

class STRONGHOLDSHARED_EXPORT PEM
{
public:
    PEM(User *_user);
    int add(QStringList &arg);
    int add(const QStringList &fields, const QStringList &values);
    int add();
    QStringList get(const QString& field, const QString& value);
    QStringList get(const QString& field);
    QStringList get(int row);
    int update(const QString &field, const QString &oldvalue, const QString &newvalue);
    int update(const QString &field, const QString &oldvalue, int id);
    int update(const int field, const QString &oldvalue, int id);
    bool remove(qlonglong id);
    int tableUse(const QString &table);
    int tableCreate(const QString &table, const QStringList &fields);
    QStringList tableList();
    QStringList tableField();
    QString tableActive();

    int commit();

    bool exportTable(const QString &dir);
    bool exportDatabase(const QDir &dir);
    bool importTable(const QString &name, const QString &dir);
    bool importDatabase(const QFileInfoList &dir);

    static QStringList convert(const QList<QVariant>& data);
    static QString generatePassword(const QStringList &arg);

    xsDatabase* db = nullptr;
    User* user = nullptr;
};

#endif // PEM_H
