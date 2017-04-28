#include "xspasswd.h"

#include <QFile>
#include <QCryptographicHash>
#include <QDir>


xsPasswd::xsPasswd()
{
}

QString xsPasswd::mainUsage()
{
    return "Usage:\n"
           "join <your-home-name>\n"
           "create <your-home-name>\n"
           "use <table-name>\n"
           "add <value1> <value2>\n"
           "get <field> <value>\n";
}


int xsPasswd::dataAdd(QStringList &arg)
{
    for(int i = 1; i < arg.size(); i++)
        arg.replace(i,blowfish->encrypt(arg.value(i).toLatin1()).toHex());

    if(!database->addValue(arg))
    {
        strStatus = "Impossible to add user or password into " + database->getTable() + "\n" +
                    database->getMessage() + "\n" + database->getLastQuery() + "\n";
        return FAIL;
    }
    return OK;
}

QStringList xsPasswd::dataGet(const QStringList& arg)
{
    QStringList offset;
    if(arg.size() == 3)
    {
        int x = database->findValue(arg.value(1), QString(blowfish->encrypt(arg.value(2).toLatin1()).toHex()));
        for(int i = 1; i < database->getFieldCount(); i++)
            offset.append(blowfish->decrypt(QByteArray::fromHex(database->findValue(i,x).toLatin1())));
    }
    return offset;
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
    /*QString newtable;
    QString newfields;
    QString formfields;
    xsConsole() << "New table name ->";
    xsConsole() >> newtable;

    while(true)
    {
        xsConsole() << "New field name (just enter to stop) ->";
        xsConsole() >> newfields;
        if(newfields == "")
            break;
        formfields.append(newfields.trimmed() + " TEXT,");
    }
    */
    QString format;
    for(int i = 0; i < fields.size(); i++)
        format.append(fields.value(i).trimmed() + " TEXT,");

    if(database->createTable(table, format.left(format.length() - 1)))
        return OK;

    strStatus = "Impossible to create a new database into " + QDir::homePath() + "\n" +
                database->getMessage() + "\n" + database->getLastQuery();
    return FAIL;
}

int xsPasswd::userCreate(const QString& passwd)
{
    QFile newpw(QDir::homePath() + PWFILE);
    if(newpw.exists())
    {
        strStatus = "Database on the directory " + QDir::homePath() + " already exists!";
        return FAIL;
    }

    QCryptographicHash hasher(QCryptographicHash::Sha512);
    hasher.addData(passwd.toLatin1());
    QByteArray salad = hasher.result();
    newpw.open(QIODevice::WriteOnly);
    newpw.write(salad);
    newpw.close();
}

int xsPasswd::userJoin(const QString &passwd)
{
    if(iHit < iMaxHit)
    {
        if(userPasswd(passwd) == OK)
        {
            database = new xsDatabase(QDir::homePath() + DBFILE);
            blowfish = new xsBlowfish(passwd);
            return OK;
        }
    }
    iHit++;
    return FAIL;
}

int xsPasswd::userPasswd(const QString& key)
{
    QFile readpw(QDir::homePath() + PWFILE);
    readpw.open(QIODevice::ReadOnly);
    QByteArray binpw = readpw.readLine();
    readpw.close();
    if(binpw == hashkey(key))
    {
        blowfish = new xsBlowfish(key);
        return OK;
    }
    return FAIL;
}

QByteArray xsPasswd::hashkey(const QString& key)
{
    QCryptographicHash hasher(QCryptographicHash::Sha512);
    hasher.addData(key.toLatin1());
    return hasher.result();
}
