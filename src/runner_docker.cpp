#include "runner_docker.h"

#include <QThread>

DockerRunner::DockerRunner()
{

}

void DockerRunner::runMedley()
{
    int vncPort = -1;
    QProcess *dockerProcess;

    if(!isDockerInstalled()) {
        throw("Docker not installed.");
    }
    if(!isDockerRunning()) {
        throw("Docker installed but not currently running");
    }
    pullDockerImageIfNeeded();

    vncPort = findOpenPort();
    if(vncPort < 0)
        throw("Could not find open port between 5900 and 5999 to use for the connection to Docker");

    dockerProcess = runDocker(vncPort);
    QThread::msleep(500);

    //run vncviewer

    //wait for start
    //wait for both end

}

QProcess *DockerRunner::runDocker(int vncPort)
{

    QStringList dockerArgs;
    QProcess *dockerProcess = new QProcess();

    dockerArgs << "run" << "-it" << "--rm";
    dockerArgs << "-p" << QString::number(vncPort) << ":5900";
    dockerArgs << "-v" << ("\"" + runConfig->logindir.value() + "\"") << ":/home/medley/il";
    dockerArgs << "--entrypoint" << "medley";
    dockerArgs << "--env" << "TERM=xterm";
    dockerArgs << "interlisp/medley:" + runConfig->dockerTag.value();
    dockerArgs << "--windows";
    dockerArgs += medleyArgs();

    dockerProcess->setProgram("docker");
    dockerProcess->setArguments(dockerArgs);
    dockerProcess->setProcessChannelMode(QProcess::ForwardedChannels);
    dockerProcess->start();
    if(!dockerProcess->waitForStarted())
        throw("Unable to execute \"docker run interlisp/medley\"");

    return dockerProcess;

}

QStringList DockerRunner::medleyArgs()
{
    QStringList medleyArgs;
    if(runConfig->interlisp_exec.has_value() && runConfig->interlisp_exec.value())
        medleyArgs << "-e";
    if(runConfig->geometry.has_value())
        medleyArgs << "-g" << runConfig->geometry.value();
    if(runConfig->id.has_value())
        medleyArgs << "-i" << runConfig->id.value();
    if(runConfig->vmem.has_value())
        medleyArgs << "-k" << runConfig->vmem.value();
    if(runConfig->noscroll.has_value() && runConfig->noscroll.value())
        medleyArgs << "-n";
    // ToDO: -o greetfile
    if(runConfig->screensize.has_value())
        medleyArgs << "-s" << runConfig->screensize.value();
    if(runConfig->title.has_value())
        medleyArgs << "-t" << runConfig->title.value();
    if(runConfig->sysout.has_value())
        medleyArgs << "-y" << runConfig->sysout.value();

    return medleyArgs;
}

void DockerRunner::pullDockerImageIfNeeded()
{
    bool isPulled;
    QString image = "interlisp/medley:" + runConfig->dockerTag.value();
    {
        QProcess p;
        p.setProgram("docker");
        p.setArguments(QStringList() << "image" << "ls" << image);
        p.start();
        if(!p.waitForStarted() || !p.waitForFinished())
            throw("Unable to access docker to test for interlisp/medley image.");
        isPulled = QString(p.readAllStandardOutput()).contains("medley");
    }
    if(!isPulled || (runConfig->update.has_value() && runConfig->update.value())) {
        QProcess p;
        p.setProgram("docker");
        p.setArguments(QStringList() << "pull" << image);
        p.start();
        if(!p.waitForStarted() || !p.waitForFinished())
            throw("Unable to \"docker pull\" interlisp/medley image.");
    }
}

bool DockerRunner::isDockerInstalled()
{
    QProcess p;
    p.setProgram("docker");
    p.start();
    if(!p.waitForStarted(5000) || !p.waitForFinished(5000))
        throw("Docker Desktop required, but it does not seem to be installed");
    return true;
}

bool DockerRunner::isDockerRunning()
{
    QProcess p;
    p.setProgram("docker");
    p.setArguments(QStringList("info"));
    p.start();
    if(!p.waitForStarted() || !p.waitForFinished())
        throw("Unable to access docker to test for interlisp/medley image.");
    return !QString(p.readAllStandardOutput()).contains("docker daemon is not running");
}

QString DockerRunner::setupVNCViewer()
{
    QString winUserProfile = windowsUserProfileFromWSL();
    QString winVNCPath = winUserProfile + QStringLiteral("\\AppData\\Local\\Interlisp");
    QProcess p;
    p.startCommand(QStringLiteral("wslpath ")+ winVNCPath);
    p.waitForFinished();
    QTextStream t(p.readAllStandardOutput());
    QString wslVNCPath = t.readLine();
    QString vncExe = QStringLiteral("vncviewer64-1.12.0.exe");
    QFileInfo fi = QFileInfo(wslVNCPath + "/" + vncExe);
    if(fi.exists())
        return fi.absoluteFilePath();
    QFileInfo fi2 = QFileInfo(runConfig->ilDir.absolutePath() + "/wsl/" + vncExe);
    if(fi2.exists()) {
        if(QFile::copy(fi2.absoluteFilePath(), fi.absoluteFilePath()))
            return fi.absoluteFilePath();
    }
    throw(QStringLiteral("VNC display requested, but the VncViewer cannot be found."));
}

