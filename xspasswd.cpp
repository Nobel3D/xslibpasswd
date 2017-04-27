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

QString xsPasswd::mainStatus()
{
    return "[" + strUser + " ~ " + database->getTable() + "]$ ";
}

void xsPasswd::dataAdd(QString arg)
{
    QStringList arglist = arg.split(" ");
    QStringList addlist;
    for(int i = 1; i < arglist.size(); i++)
        addlist.append(blowfish->encrypt(arglist.value(i).toLatin1()).toHex());

    if(!database->addValue(addlist))
    {
        xsConsole() << "Impossible to add user or password into " << database->getTable() << "\n";
        xsConsole() << database->getMessage() << "\n" << database->getLastQuery() << "\n";
    }
}

void xsPasswd::dataGet(QString arg)
{
    QStringList listarg = arg.split(" ");
    if(listarg.size() == 2)
    {
        QStringList table = database->printColumn(listarg.value(1));
        for(int i = 0; i < table.size(); i++)
            xsConsole() << blowfish->decrypt(QByteArray::fromHex(table.value(i).toLatin1())) << "\n";
    }
    else if(listarg.size() == 3)
    {
        int x = database->findValue(listarg.value(1), QString(blowfish->encrypt(listarg.value(2).toLatin1()).toHex()));
        for(int i = 1; i < database->getFieldCount(); i++)
            xsConsole() << database->getFieldName(i) << " -> " <<
                           blowfish->decrypt(QByteArray::fromHex(database->findValue(i,x).toLatin1())) << "\n";
    }
}

void xsPasswd::tableUse(QString arg)
{
    QString table = arg.split(" ").value(1);
    if(!database->useTable(table))
        xsConsole() << "Impossible to open table " << table << "\n";
}

int xsPasswd::userCreate(QString user)
{
    QFile newpw(HOME_PATH + user + PWFILE);
    if(newpw.exists())
        return FAIL;
    QString hot1,hot2;
    xsConsole() << "Enter new password -> ";
    hot1 = xsConsole::ReadPasswd();
    xsConsole() << "Repeat your password -> ";
    hot2 = xsConsole::ReadPasswd();
    if(hot1 != hot2)
    {
        xsConsole() << "Password mismatch!";
        return FAIL;
    }
    hot2.clear();
    QCryptographicHash hasher(QCryptographicHash::Sha512);
    hasher.addData(hot1.toLatin1());
    hot1.clear();
    QByteArray salad = hasher.result();
    newpw.open(QIODevice::WriteOnly);
    newpw.write(salad);
    newpw.close();

    userJoin(user);

    QString newtable;
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
    if(database->createTable(newtable, formfields.left(formfields.length() - 1)))
        return OK;

    xsConsole() << "Impossible to create a new database into " << HOME_PATH << user << "\n";
    xsConsole() << database->getMessage() << "\n" << database->getLastQuery();
    return FAIL;
}

int xsPasswd::userJoin(QString tmpuser)
{
    strUser = tmpuser;
    for(int i = 0; i < MAXHIT; i++)
    {
        xsConsole() << "Enter your password -> ";
        if(userPasswd(xsConsole::ReadPasswd()) == OK)
        {
            if(!database->connect(HOME_PATH + strUser + DBFILE))
                xsConsole() << "Cannot access into your database file (" << HOME_PATH + strUser + DBFILE << ")\n";
            return OK;
        }
    }
    strUser = "tmp";
    return FAIL;
}

int xsPasswd::userPasswd(QString key)
{
    QFile readpw(HOME_PATH + strUser + PWFILE);
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

QByteArray xsPasswd::hashkey(QString& key)
{
    QCryptographicHash hasher(QCryptographicHash::Sha512);
    hasher.addData(key.toLatin1());
    return hasher.result();
}
