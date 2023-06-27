#ifndef RUNNER_H
#define RUNNER_H

#include "config.h"

#include <QString>
#include <QStringList>
#include <QHostAddress>
#include <QProcessEnvironment>

class Runner
{
public:
    Runner();
    void runMedley();

protected:
    Config *runConfig;

    static int findOpenDisplay();
    static int findOpenPort();


private:

    void prepareConfigForRunMedley();
    QProcess *runMedleyCore();
    QProcessEnvironment envForLde();
    QStringList argsForLde();
    QProcess *runXvnc(int &openDisplay, int &vncPort);
    QString setupVNCViewer();
    QProcess *startVNCViewer(QString viewerPath, int vncPort);
    void terminateVncViewer(QProcess *vncViewerProcess);


    static QStringList activePIDs();
    static QString windowsUserProfileFromWSL();
    static bool checkForXvnc();
    static QHostAddress ipAddress();

};

#endif // RUNNER_H
