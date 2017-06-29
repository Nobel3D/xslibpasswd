#include "xspasswd.h"

#include <QFile>
#include <QCryptographicHash>


xsPasswd::xsPasswd()
{
}

int xsPasswd::dataAdd(QStringList &arg)
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

int xsPasswd::dataAdd(const QStringList &fields, const QStringList &values)
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

QStringList xsPasswd::dataGet(const QString& field, const QString& value)
{
    QStringList offset;

    int x = database->findValue(database->getField(field), encode(value));
    if(x < 0)
        return offset;
    for(int i = 1; i < database->getFieldCount(); i++)
        offset.append(decode(database->findValue(database->getField(i),x)).toString());

    return offset;
}

QStringList xsPasswd::dataGet(const QString& field)
{
    QList<QVariant> offset;

    offset = database->getColumn(database->getField(field));
    for(int i = 0; i < offset.size(); i++)
        offset.replace(i,decode(offset.at(i)));

    return convert(offset);
}

QStringList xsPasswd::dataGet(int row)
{
    QList<QVariant> offset(database->getRow(row));
    for(int i = 0; i < offset.size(); i++)
        offset.replace(i,decode(offset.at(i)));
    return convert(offset);
}


int xsPasswd::dataUpdate(const QString &field, const QString &oldvalue, const QString &newvalue)
{
    if(!database->updateValue(database->getField(field), QString(blowfish->encrypt(oldvalue.toLatin1()).toHex()),
                                 QString(blowfish->encrypt(newvalue.toLatin1()).toHex())))
    {
        qDebug() << database->getMessage() << endl << database->getLastQuery();
        return FAIL;
    }
    return OK;
}

int xsPasswd::dataUpdate(const QString &field, const QString &newvalue, int id)
{
    if(!database->updateValue(database->getField(field), QString(blowfish->encrypt(newvalue.toLatin1()).toHex()), id))
    {
        strStatus = database->getMessage() + endl + database->getLastQuery();
        return FAIL;
    }
    return OK;
}

bool xsPasswd::dataDelete(qlonglong id)
{
    if(!database->removeValue(QSqlField("id", QVariant::LongLong), QVariant(id)))
    {
        qDebug() << database->getMessage() << endl << database->getLastQuery();
        return false;
    }
    return true;
}

int xsPasswd::tableUse(const QString &table)
{
    if(!database->useTable(table))
    {
        strStatus = "Impossible to open table " + table;
        return FAIL;
    }
    return OK;
}

int xsPasswd::tableCreate(const QString &table, const QStringList &fields)
{
    QList<QSqlField> format;

    format.append(QSqlField("id", QVariant::LongLong));

    for(int i = 0; i < fields.size(); i++)
        format.append(QSqlField(fields.at(i), QVariant::String));

    if(database->createTable(table, format))
        return OK;

    strStatus = "Impossible to create a new database into " + HOME + "\n" +
                database->getMessage() + endl + database->getLastQuery();
    return FAIL;
}

QStringList xsPasswd::tableList()
{
    return database->getTables();
}

QStringList xsPasswd::tableField()
{
    QStringList offset;
    QList<QSqlField> buffer = database->getFields();
    for(int i = 0; i < buffer.count(); i++)
        offset.append(buffer.at(i).name());
    return offset;
}

bool xsPasswd::tableActive()
{
    return !database->getTable().isEmpty();
}

int xsPasswd::userCreate(const xsPassword& passwd, const QString &file)
{
    if(passwd.Save(file))
        return OK;
    strStatus = "Impossible to save your user in " + file;
    return FAIL;
}

int xsPasswd::userJoin(const xsPassword &passwd)
{
    if(password->Check(passwd))
    {
        database = new xsDatabase(DBFILE);
        blowfish = new xsBlowfish(passwd.getClearPassword());
        return OK;
    }
    return FAIL;
}


int xsPasswd::loadPassword(const QString& filepw)
{
    password = new xsPassword();
    QFile loading(filepw);
    return password->Load(loading);
}

QString xsPasswd::generatePassword(const QStringList &arg)
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

QVariant xsPasswd::decode(const QVariant& encoded)
{
    if (encoded.type() == QVariant::String)
        return QString(blowfish->decrypt(QByteArray::fromHex(encoded.toString().toLatin1())));
    else
        return encoded;
}

QVariant xsPasswd::encode(const QVariant& decoded)
{
    return QString(blowfish->encrypt(decoded.toString().toLatin1()).toHex());
}

QStringList xsPasswd::convert(const QList<QVariant>& data)
{
    QStringList out;
    for(int i = 0; i < data.count(); i++)
        out.append(data.at(i).toString());
    return out;
}

bool xsPasswd::exportTable(const QString &dir)
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

bool xsPasswd::exportDatabase(const QString &dir)
{
    database->Export(dir);
}

bool xsPasswd::importTable(const QString &name, const QString &dir)
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

bool xsPasswd::importDatabase(const QString &name, const QString &dir)
{

}
