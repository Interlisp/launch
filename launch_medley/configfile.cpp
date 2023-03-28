#include "configfile.h"
#include "config.h"

#include <optional>

#include <QChar>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

ConfigFile::ConfigFile()
{
    configPath = QDir::homePath() + "/il/medley.config";
}

ConfigFile::~ConfigFile() {}


QStringList *ConfigFile::readConfig() {


    // check if configPath exists, is a file and is readable?
    // If not, return a null string
    QFileInfo fi = QFileInfo(this->configPath);
    bool fileExists = fi.exists() && fi.isFile() && fi.isReadable();
    if (!fileExists) {
        return new QStringList();
    }

    // Read file in, line by line, ignoring comments, and splitting
    // each line into individual arguments (paying attention to quotes
    // and backslashes). Stash indivual arguments into argList.
    QFile file(this->configPath);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    QStringList *argList = new QStringList();

    while(!in.atEnd()) {
        QString line = in.readLine().trimmed();
        qsizetype len = line.size();

        if((len < 1) || (line.at(0) == QChar('#'))) continue;

        QString current = QString();
        bool inDoubleQuotes = false;
        bool inSingleQuotes = false;
        bool backQuoted = false;
        bool newBackQuoted = false;

        for(qsizetype idx = 0; idx < len; ++idx) {
            QChar c = line.at(idx);
            if (c == QChar(' ')) {
                if(inDoubleQuotes || inSingleQuotes || backQuoted) {
                    current += c;
                } else if(!current.isNull()) {
                   *argList << current;
                   current = QString();
                }
            } else if (c == QChar('"')) {
                if(backQuoted || inSingleQuotes) {
                    current += c;
                } else if(inDoubleQuotes) {
                    inDoubleQuotes = false;
                    //current += c;
                    *argList << current;
                    current = QString();
                } else {
                    if(!current.isNull()) {
                        *argList << current;
                        current = QString();
                    }
                    inDoubleQuotes = true;
                    //current += c;
                }
            } else if (c == QChar('\'')) {
                if(backQuoted || inDoubleQuotes) {
                    current += c;
                } else if(inSingleQuotes) {
                    inSingleQuotes = false;
                    //current += c;
                    *argList << current;
                    current = QString();
                } else {
                    if(!current.isNull()) {
                        *argList << current;
                        current = QString();
                    }
                    inSingleQuotes = true;
                    //current += c;
                }
            } else if (c == QChar('\\')) {
                backQuoted = true;
                newBackQuoted = true;
            } else {
                current += c;
            }
            if(!newBackQuoted) {
                backQuoted = false;
            } else {
                newBackQuoted = false;
            }
        }
        //if(inDoubleQuotes) current += QChar('"');
        //if(inSingleQuotes) current += QChar('\'');
        if(!current.isNull()) {
            *argList << current;
            current = QString();
        }
    }
    file.close();
    return argList;
}

void ConfigFile::writeConfig(Config *config) {

    // Check if configPath exists, is a file and is writeable?
    // If not try to create it.
    QFileInfo fi = QFileInfo(this->configPath);
    bool fileExists = fi.exists() && fi.isFile() && fi.isWritable();
    if(!fileExists) {
        QString dirPath = fi.absolutePath();
        QFileInfo di = QFileInfo(dirPath);
        bool dirExists = di.exists() && di.isDir() && di.isWritable();
        QDir dir(dirPath);
        bool success = dir.mkpath(dirPath);
        if(!success) {
            QMessageBox::information(0, "error", "Unable to create directory for medley.config");
            return;
        }
    }
    QFile file(this->configPath);
    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    QTextStream out(&file);

    config->toTextStream(out, QChar('\n'));

    file.close();

}

