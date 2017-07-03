#include "login.h"

Login::Login(const QString &_manager)
{
    manager = new xsDatabase(_manager, "manager");
   if(!manager->getTables().contains("stronghold"))
   {
       QList<QSqlField> fields;
       fields.append(QSqlField("id", QVariant::LongLong));
       fields.append(QSqlField("user", QVariant::String));
       fields.append(QSqlField("password", QVariant::String));
       fields.append(QSqlField("database", QVariant::String));
       fields.append(QSqlField("level", QVariant::Int));
       manager->createTable("stronghold", fields);
       manager->useTable("stronghold");
       QString buf = "stronghold";
       userAdd("dev",xsPassword(buf), QDir::homePath() + "/default.db", 0);
   }
   else
       manager->useTable("stronghold");
}

QString Login::login(const QString &user, const xsPassword &password)
{
    int index = manager->findValue(QSqlField("user", QVariant::String), user);
    if(index < 0)
        return "";
    QList<QVariant> values = manager->getRow(index);

    if(values.isEmpty() || values.at(2).toString() != password.getPassword())
        return "";
    else
        return values.at(3).toString();
}

bool Login::userAdd(const QString &name, const xsPassword &password, const QString &database, int level)
{
    QList<QVariant> values;
    values.append(name);
    values.append(password.getPassword());
    values.append(database);
    values.append(level);
    if(manager->addValue(values))
        return true;
    else
    {
        qDebug() << manager->getLastQuery() << endl << manager->getMessage() << endl;
        return false;
    }
}

QStringList Login::getUsers()
{
    QList<QVariant> in = manager->getColumn(QSqlField("user", QVariant::String));
    QStringList out;
    for(int i = 0; i < in.count(); i++)
        out.append(in.at(i).toString());
    return out;
}
