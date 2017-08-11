#ifndef XSLIBPASSWD_GLOBAL_H
#define XSLIBPASSWD_GLOBAL_H

#include <QtCore/qglobal.h>
#include <xslib/xslib.h>
#include <QDir>

#define XSLIB_STRONGHOLD "0.10"

#ifdef __linux__
    #define SH_DIR "/usr/share/xsoftware/Stronghold"
#endif
#define LOGINFILE "/usr/share/xsoftware/Stronghold/Stronghold.db"


#if defined(STRONGHOLD_LIBRARY)
#  define STRONGHOLDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define STRONGHOLDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XSLIBPASSWD_GLOBAL_H
