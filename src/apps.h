#ifndef APPS_H
#define APPS_H

#include "medleyapp.h"
#include <QCoreApplication>
#include <QApplication>

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
