#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>

class Plugin
{
public:
    virtual int use() = 0;
};

Q_DECLARE_INTERFACE(Plugin, "org.xsoftware.stronghold.plugin")

#endif // PLUGIN_H
