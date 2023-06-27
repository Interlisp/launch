#ifndef DOCKERRUNNER_H
#define DOCKERRUNNER_H

#include "runner.h"

class DockerRunner : public Runner
{
public:
    DockerRunner();
    void runMedley();

private:
    QProcess *runDocker(int vncPort);
    QStringList medleyArgs();

    bool isDockerInstalled();
    bool isDockerRunning();
    void pullDockerImageIfNeeded();
    QString setupVNCViewer();
};

#endif // DOCKERRUNNER_H
