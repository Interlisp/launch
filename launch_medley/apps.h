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
    QString figureOutMedleyDir(QString invokePath);
    QString searchEnvPathForExec(QString execName);

    QProcessEnvironment env;
    QString argv0;
    bool isGuiApp;
    bool isMacOSBundle;
    QDir medleyDir;
    QDir defaultLoginDir;
    QString defaultGreetFile;
    QDir invokeDir;
    QDir InvokeName;
    QProcessEnvironment ldeEnv;

    static MedleyApp *app;
    static Config *config;
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
