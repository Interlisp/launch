#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include "config.h"

#include <QString>
#include <QStringList>
#include <QTextStream>

class ConfigFile
{
public:
    ConfigFile();
    ~ConfigFile();

    QStringList *readConfig();
    void writeConfig(Config *config);
    Config *configFromArgList(QStringList argList);
    void configToTextStream(Config *config, QTextStream &out, QChar separator);

private:
    QString configPath;
    //constexpr uint32_t hash(const char* data, size_t const size);
};

#endif // CONFIGFILE_H
