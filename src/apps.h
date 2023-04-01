#ifndef APPS_H
#define APPS_H

#include <QObject>
#include <QCoreApplication>
#include <QApplication>
#include <QDir>
#include <QProcessEnvironment>

#include "config.h"
#include "mainwindow.h"

class MedleyApp
{

public:
    MedleyApp(int &argc, char **argv);
    ~MedleyApp();
    Config *readConfigFile();
    void figureOutDirectories();
    QString figureOutMedleyDir(QString invokePath);
    QString searchEnvPathForExec(QString execName);
    QString figureOutMaikoDir(QString medleyDir);
    QString testMaikoExecDir(QString maikoDirCandidate);
    void runMedley();

    QProcessEnvironment env;
    QString argv0;
    bool isGuiApp;
    bool isMacOSBundle;
    QDir medleyDir;
    QDir maikoDir;
    QDir maikoExecDir;
    QDir defaultLoginDir;
    QString greetFileNoGreet;
    QString greetFileDefault;
    QString greetFileApps;
    QDir invokeDir;
    QDir InvokeName;


    static MedleyApp *app;
    static Config *config;
    static Config *runConfig;
    static QString osversion();
    static QString machinetype();
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
    MainWindow *mainWindow;
    int startApp();
};


#endif // APPS_H
