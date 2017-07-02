#include "stronghold.h"

#include <QFile>
#include <QCryptographicHash>
#include <QFileInfoList>


Stronghold::Stronghold(const QString &manager)
{
    login = new Login(manager);
}

int Stronghold::dataAdd(QStringList &arg)
{
    QList<QVariant> out;
    for(int i = 1; i < arg.size(); i++)
        out.append(QString(blowfish->encrypt(arg.at(i).toLatin1()).toHex()));

    if(!database->addValue(out))
    {
        strStatus = "Impossible to add some value into " + database->getTable() + "\n" +
                    database->getMessage() + "\n" + database->getLastQuery() + "\n";
        return FAIL;
    }
    return OK;
}

int Stronghold::dataAdd(const QStringList &fields, const QStringList &values)
{
    int count = fields.count();
    if(count != values.count())
        return FAIL;

    QList<QSqlField> fieldlist;
    QList<QVariant> valuelist;

    for(int i = 0; i < count; i++)
    {
        fieldlist.append(database->getField(fields.at(i)));
        if(fieldlist.at(i).type() == QVariant::String)
            valuelist.append(QString(blowfish->encrypt(values.at(i).toLatin1()).toHex()));
        else
            valuelist.append(values.at(i));
    }
    database->addValue(fieldlist, valuelist);
}

QStringList Stronghold::dataGet(const QString& field, const QString& value)
{
    QStringList offset;

    int x = database->findValue(database->getField(field), encode(value));
    if(x < 0)
        return offset;
    for(int i = 1; i < database->getFieldCount(); i++)
        offset.append(decode(database->findValue(database->getField(i),x)).toString());

    return offset;
}

QStringList Stronghold::dataGet(const QString& field)
{
    QList<QVariant> offset;

    offset = database->getColumn(database->getField(field));
    for(int i = 0; i < offset.size(); i++)
        offset.replace(i,decode(offset.at(i)));

    return convert(offset);
}

QStringList Stronghold::dataGet(int row)
{
    QList<QVariant> offset(database->getRow(row));
    for(int i = 0; i < offset.size(); i++)
        offset.replace(i,decode(offset.at(i)));
    return convert(offset);
}


int Stronghold::dataUpdate(const QString &field, const QString &oldvalue, const QString &newvalue)
{
    if(!database->updateValue(database->getField(field), QString(blowfish->encrypt(oldvalue.toLatin1()).toHex()),
                                 QString(blowfish->encrypt(newvalue.toLatin1()).toHex())))
    {
        qDebug() << database->getMessage() << endl << database->getLastQuery();
        return FAIL;
    }
    return OK;
}

int Stronghold::dataUpdate(const QString &field, const QString &newvalue, int id)
{
    if(!database->updateValue(database->getField(field), QString(blowfish->encrypt(newvalue.toLatin1()).toHex()), id))
    {
        strStatus = database->getMessage() + endl + database->getLastQuery();
        return FAIL;
    }
    return OK;
}

bool Stronghold::dataDelete(qlonglong id)
{
    if(!database->removeValue(QSqlField("id", QVariant::LongLong), QVariant(id)))
    {
        qDebug() << database->getMessage() << endl << database->getLastQuery();
        return false;
    }
    return true;
}

int Stronghold::tableUse(const QString &table)
{
    if(!database->useTable(table))
    {
        strStatus = "Impossible to open table " + table;
        return FAIL;
    }
    return OK;
}

int Stronghold::tableCreate(const QString &table, const QStringList &fields)
{
    QList<QSqlField> format;

    format.append(QSqlField("id", QVariant::LongLong));

    for(int i = 0; i < fields.size(); i++)
        format.append(QSqlField(fields.at(i), QVariant::String));

    if(database->createTable(table, format))
        return OK;

    return FAIL;
}

QStringList Stronghold::tableList()
{
    return database->getTables();
}

QStringList Stronghold::tableField()
{
    QStringList offset;
    QList<QSqlField> buffer = database->getFields();
    for(int i = 0; i < buffer.count(); i++)
        offset.append(buffer.at(i).name());
    return offset;
}

bool Stronghold::tableActive()
{
    return !database->getTable().isEmpty();
}

bool Stronghold::userCreate(const QString &name, const xsPassword& passwd, const QString &file)
{
    if(login->userAdd(name, passwd, file, 0)) //TODO: CHANGE LEVEL
        return true;
    else
        return false;
}

bool Stronghold::userJoin(const QString &user, QString &passwd)
{
    QString dbfile = login->login(user, xsPassword(passwd));
    if(!dbfile.isEmpty())
    {
        database = new xsDatabase(dbfile);
        blowfish = new xsBlowfish(passwd);
        return true;
    }
    return false;
}

QString Stronghold::generatePassword(const QStringList &arg)
{
    bool symbols = false, spaces = false, unicode = false, numbers = false, lowers = false, uppers = false, ok = false;
    int length = arg.at(1).toInt(&ok);
    if(arg.count() < 2 || !ok)
        return "";

    if(arg.contains("sym", Qt::CaseInsensitive))
        symbols = true;
    if(arg.contains("space", Qt::CaseInsensitive))
        spaces = true;
    if(arg.contains("uni", Qt::CaseInsensitive))
        unicode = true;
    if(arg.contains("num", Qt::CaseInsensitive))
        numbers = true;
    if(arg.contains("low", Qt::CaseInsensitive))
         lowers = true;
    if(arg.contains("up", Qt::CaseInsensitive))
         uppers = true;

    if(arg.count() == 2)
        return xsPassword::generate(length);
    else if(unicode)
        return xsPassword::generate(length, symbols, spaces, true);
    else
        return xsPassword::generate(length, symbols, spaces, false, numbers, lowers, uppers);
}

QVariant Stronghold::decode(const QVariant& encoded)
{
    if (encoded.type() == QVariant::String)
        return QString(blowfish->decrypt(QByteArray::fromHex(encoded.toString().toLatin1())));
    else
        return encoded;
}

QVariant Stronghold::encode(const QVariant& decoded)
{
    return QString(blowfish->encrypt(decoded.toString().toLatin1()).toHex());
}

QStringList Stronghold::convert(const QList<QVariant>& data)
{
    QStringList out;
    for(int i = 0; i < data.count(); i++)
        out.append(data.at(i).toString());
    return out;
}

bool Stronghold::exportTable(const QString &dir)
{
    X_PARAMS(dir.isEmpty());

    QStringList buffer;
    QFile file(dir);
    file.open(QFile::WriteOnly);
    file.write(database->format(database->getFields()).toLatin1() + "\n");

    for(int i = 0; i < database->getRecordCount() + 1; i++)
    {
        buffer = dataGet(i);
        for(int column = 0; column < buffer.count(); column++)
            file.write(buffer.at(column).toLatin1() + ",");
        file.write("\n");
    }
    file.close();
    return true;
}

bool Stronghold::exportDatabase(const QDir &dir)
{
    QStringList tables = database->getTables();
    for(int i = 0; i < tables.count(); i++)
    {
        database->useTable(tables.at(i));
        exportTable(dir.path() + "/" + tables.at(i) + ".csv");
    }
}

bool Stronghold::importTable(const QString &name, const QString &dir)
{
    X_PARAMS(dir.isEmpty());

    QString oldTable = database->getTable();
    QFile file(dir);
    file.open(QFile::ReadOnly);
    QStringList header = QString(file.readLine()).split(','); //first row (name field and type)
    header.removeFirst();
    header.last().replace('\n',"");
    if(!tableCreate(name,header))
        return false;

    tableUse(name);

    QStringList values;
    while(file.bytesAvailable())
    {
        values = QString(file.readLine()).split(',');
        values.removeLast();
        if(!dataAdd(tableField(), values))
            qDebug() << database->getLastQuery() << "\n" << database->getMessage();
        values.clear();
    }
    tableUse(oldTable);
    file.close();
    return true;
}

bool Stronghold::importDatabase(const QFileInfoList &dir)
{
    for(int i = 0; i < dir.count(); i++)
        importTable(dir.at(i).completeBaseName(), dir.at(i).absoluteFilePath());
}
