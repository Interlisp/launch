#ifndef APPS_H
#define APPS_H

#include <QObject>
#include <QCoreApplication>
#include <QApplication>
#include <QDir>
#include <QProcessEnvironment>

#include "config.h"

class MedleyApp
{

public:
    MedleyApp(int &argc, char **argv);
    ~MedleyApp();
    Config *readConfigFile();
    void figureOutDirectories();
    QString isMedleyDir(QString path);
    QString searchEnvPathForExec(QString execName);

    QProcessEnvironment env;
    QString argv0;
    Config *config;
    bool isGuiApp;
    QDir medleyDir;
    QDir defaultLoginDir;
    QDir invokeDir;
    QDir InvokeName;
    QProcessEnvironment ldeEnv;

    static MedleyApp *app;
};

class CoreApplication: public MedleyApp, public QCoreApplication
{
public:
    CoreApplication(int &argc, char **argv);
    ~CoreApplication();
    int startApp();
};

class Application: public MedleyApp, public QApplication
{
public:
    Application(int &argc, char **argv);
    ~Application();
    int startApp();
};


#endif // APPS_H
