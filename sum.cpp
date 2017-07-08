#include "sum.h"
#include <QFileInfo>

SUM::SUM(const QString &_db)
{
   db = new xsDatabase(QFileInfo(_db), "sum");
   if(!db->getTables().contains("stronghold"))
        qWarning() << "Database manager didn't find on directory " << _db;
   else
       db->useTable("stronghold");
}

bool SUM::login(const QString &name, const QString &password)
{
    int index = db->findValue(QSqlField("name", QVariant::String), name);
    if(index < 0)
        return false;
    QList<QVariant> values = db->getRow(index);

    xsPassword encrypt(password);
    if(values.isEmpty() || encrypt.Check(values.at(2).toString()))
        return false;
    user = new User(values.at(0).toInt(), values.at(1).toString(), password, values.at(3).toString(), values.at(4).toInt());
    return true;
}

bool SUM::add(const QString &name, const QString &password, const QString &database, int level) //TODO: xsPassword only for internal use
{
    if(user->getLevel() < level)
        return false;

    QList<QVariant> values;
    xsPassword pw(password);
    values.append(name);
    values.append(pw.getPassword());
    values.append(database);
    values.append(level);

    if(db->addValue(values))
        return true;
    else
    {
        qDebug() << db->getLastQuery() << endl << db->getMessage() << endl;
        return false;
    }
}

int SUM::add()
{
    db->addValue();
}

bool SUM::update(int field, int row, const QVariant &value)
{
    QSqlField in = db->getField(field);
    QVariant buffer = value;
    if(in.name() == "password")
    {
        xsPassword password;
        password.setPassword(value.toString());
        buffer = password.getPassword();
    }
    else if(in.name() == "level" && value.canConvert(QVariant::Int))
    {
        if(user->getLevel() < value.toInt())
            return false;
        buffer.convert(QVariant::Int);
    }
    db->updateValue(in, buffer, row);
    return true;
}

QStringList SUM::getUsers()
{
    QList<QVariant> in = db->getColumn(QSqlField("name", QVariant::String));
    QStringList out;
    for(int i = 0; i < in.count(); i++)
        out.append(in.at(i).toString());
    return out;
}

User* SUM::getUser()
{
    return user;
}
