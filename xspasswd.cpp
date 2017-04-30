#include "xspasswd.h"

#include <QFile>
#include <QCryptographicHash>


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
    QString format;
    for(int i = 0; i < fields.size(); i++)
        format.append(fields.value(i).trimmed() + " TEXT,");

    if(database->createTable(table, format.left(format.length() - 1)))
        return OK;

    strStatus = "Impossible to create a new database into " + HOME + "\n" +
                database->getMessage() + "\n" + database->getLastQuery();
    return FAIL;
}

bool xsPasswd::userExists(const QString &strfile)
{
    QFileInfo filepw(strfile);
    return filepw.exists() && filepw.isFile();
}

int xsPasswd::userCreate(const QString& passwd)
{
    if(userExists(PWFILE))
    {
        strStatus = "User on the directory " + HOME + " already exists!";
        return FAIL;
    }

    QFile filepw(PWFILE);
    QCryptographicHash hasher(QCryptographicHash::Sha512);
    hasher.addData(passwd.toLatin1());
    QByteArray salad = hasher.result();
    filepw.open(QIODevice::WriteOnly);
    filepw.write(salad);
    filepw.close();
}

int xsPasswd::userJoin(const QString &passwd)
{
    if(iHit < iMaxHit)
    {
        if(userPasswd(passwd) == OK)
        {
            database = new xsDatabase(DBFILE);
            blowfish = new xsBlowfish(passwd);
            return OK;
        }
    }
    iHit++;
    return FAIL;
}

int xsPasswd::userPasswd(const QString& key)
{
    if(!userExists(PWFILE))
    {
        strStatus = "User on the directory " + HOME + " doesn't exist!";
        return FAIL;
    }

    QFile filepw(PWFILE);
    filepw.open(QIODevice::ReadOnly);
    QByteArray pw = filepw.readLine();
    filepw.close();
    if(pw == hashkey(key))
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
