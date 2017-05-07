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
           "add <value1> <value2> ...\n"
           "get <field> <value>\n";
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

QStringList xsPasswd::dataGet(const QStringList& arg)
{
    QStringList offset;
    if(arg.size() == 3)
    {
        int x = database->findValue(arg.value(1), QString(blowfish->encrypt(arg.value(2).toLatin1()).toHex()));
        for(int i = 1; i < database->getFieldCount(); i++)
            offset.append(blowfish->decrypt(QByteArray::fromHex(database->findValue(i,x).toLatin1())));
    }
    if(arg.size() == 2)
    {
        offset = database->printColumn(arg.at(1));
        for(int i = 0; i < offset.size(); i++)
            offset.replace(i,blowfish->decrypt(QByteArray::fromHex(offset.at(i).toLatin1())));
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

int xsPasswd::userCreate(const xsPassword& passwd, const QString &file)
{
    if(passwd.Save(file) == OK)
        return OK;
    strStatus = "Impossible to save your user in " + file;
    return FAIL;
}

int xsPasswd::userJoin(const xsPassword &passwd)
{
    if(password->Check(passwd) == OK)
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
