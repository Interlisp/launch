#ifndef MEDLEYAPP_H
#define MEDLEYAPP_H


#include <QObject>
#include <QCoreApplication>
#include <QApplication>
#include <QDir>
#include <QProcessEnvironment>

#include "config.h"
#include "runner.h"

class MedleyApp: public Runner
{

public:
    MedleyApp(int &argc, char **argv);
    ~MedleyApp();
    void readConfigFile(Config *config);
    void figureOutDirectories(Config *config);
    QString figureOutMedleyDir(QString invokePath);
    QString figureOutMaikoDir(QString medleyDir);
    QString testMaikoExecDir(QString maikoDirCandidate);

    QString argv0;
    static MedleyApp *app;
    static Config *config;
    static QString searchEnvPathForExec(QString execName);

};

#endif // MEDLEYAPP_H
