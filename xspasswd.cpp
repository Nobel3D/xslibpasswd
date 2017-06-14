#include "xspasswd.h"

#include <QFile>
#include <QCryptographicHash>


xsPasswd::xsPasswd()
{
}

int xsPasswd::dataAdd(QStringList &arg)
{
    QStringList out;
    for(int i = 1; i < arg.size(); i++)
        out.append(blowfish->encrypt(arg.at(i).toLatin1()).toHex());

    if(!database->addValue(out))
    {
        strStatus = "Impossible to add user or password into " + database->getTable() + "\n" +
                    database->getMessage() + "\n" + database->getLastQuery() + "\n";
        return FAIL;
    }
    return OK;
}

QStringList xsPasswd::dataGet(const QString& field, const QString& value)
{
    QStringList offset;

    int x = database->findValue(field, QString(blowfish->encrypt(value.toLatin1()).toHex()));
    for(int i = 1; i < database->getFieldCount(); i++)
        offset.append(blowfish->decrypt(QByteArray::fromHex(database->findValue(i,x).toLatin1())));

    return offset;
}

QStringList xsPasswd::dataGet(const QString& field)
{
    QStringList offset;

    offset = database->printColumn(field);
    for(int i = 0; i < offset.size(); i++)
        offset.replace(i,blowfish->decrypt(QByteArray::fromHex(offset.at(i).toLatin1())));

    return offset;
}

QStringList xsPasswd::dataGet()
{
    QStringList offset;
    //TODO!
    return offset;
}


int xsPasswd::dataUpdate(const QStringList &arg)
{
    bool ok;
    int value = arg.at(3).toInt(&ok);
    if (!ok)
    {
        if(!database->updateValue(arg.at(1), blowfish->encrypt(arg.at(2).toLatin1()).toHex(),
                                 QString(blowfish->encrypt(arg.at(3).toLatin1()).toHex())))
        {
            strStatus = database->getMessage() + endl + database->getLastQuery();
            return FAIL;
        }
        return OK;
    }
    else
    {
        if(!database->updateValue(arg.at(1), blowfish->encrypt(arg.at(2).toLatin1()).toHex(), value))
        {
            strStatus = database->getMessage() + endl + database->getLastQuery();
            return FAIL;
        }
        return OK;
    }
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
    QString format;
    for(int i = 0; i < fields.size(); i++)
        format.append(fields.value(i).trimmed() + " TEXT,");

    if(database->createTable(table, format.left(format.length() - 1)))
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
    return database->getFieldsList();
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
