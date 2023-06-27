#include "runner.h"
#include "medleyapp.h"
#include "config.h"

#include <QProcess>
#include <QProcessEnvironment>
#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QTcpServer>
#include <QNetworkInterface>
#include <QThread>


Runner::Runner() : runConfig(nullptr)
{

}

//
//  Basic runMedley - based on existing config,
//       set up environment variables and command line arguments
//       for call to lde, then call lde.
//
void Runner::runMedley() {
    bool use_vnc;
    int xVncDisplay = -1, vncPort;
    QProcess *ldeProcess = nullptr;
    QProcess *XvncProcess = nullptr;
    QProcess *vncViewerProcess = nullptr;

    prepareConfigForRunMedley();
    use_vnc = runConfig->vnc.has_value() && runConfig->vnc.value();
    if(use_vnc) {
        XvncProcess = runXvnc(xVncDisplay, vncPort);
        runConfig->display = ":" + QString::number(xVncDisplay);
        QThread::msleep(100);
    }
    ldeProcess = runMedleyCore();
    if(use_vnc) {
        vncViewerProcess = startVNCViewer(setupVNCViewer(), vncPort);
    }
    ldeProcess->waitForFinished(-1);
    if(vncViewerProcess != nullptr)
        terminateVncViewer(vncViewerProcess);
    if(ldeProcess != nullptr) delete ldeProcess;
    if(XvncProcess != nullptr) delete XvncProcess;
    if(vncViewerProcess != nullptr) delete vncViewerProcess;
    std::exit(0);
}

//
//
//  Start up Xvnc and VNC before running Medley
//
//
QProcess *Runner::runXvnc(int &display, int &vncPort)
{
    QString cmd;
    QProcess *XvncProcess = new QProcess();

    checkForXvnc();
    display = findOpenDisplay();
    if(display < 0)
        throw("VNC requested, but could not find an open X Display between :1 and :63 to use for Xvnc.");

    vncPort = findOpenPort();
    if(vncPort < 0)
        throw("VNC Requested, but could not find and open port between 5900 and 5999 to use for VNC.");

    if(!runConfig->geometry.has_value())
        throw("Internal error in runMedleyVNC. runConfig->geomtery has no value!");

    cmd = QStringLiteral("/usr/bin/Xvnc :%1 -rfbport %2 -geometry %3 ")
            .arg(display)
            .arg(vncPort)
            .arg(runConfig->geometry.value());
            ;

    cmd += QStringLiteral("-SecurityTypes None -NeverShared -DisconnectClients=0 --MaxDisconnectionTime=10");

    XvncProcess->setProcessChannelMode(QProcess::ForwardedChannels);
    XvncProcess->startCommand(cmd);
    if(!XvncProcess->waitForStarted()) {
        throw("VNC requested, but failed to start Xvnc server");
    };
    return XvncProcess;
}

void Runner::prepareConfigForRunMedley()
{
    Config *save = runConfig;
    runConfig = new Config(*MedleyApp::config);
    if(save != nullptr) delete save;
    runConfig->prepareConfigForRunMedley();
}

QProcess *Runner::runMedleyCore()
{
    QProcess *lde = new QProcess();
    lde->setProgram(runConfig->maikoExecDir.absolutePath() + "/lde");
    lde->setProcessEnvironment(envForLde());
    lde->setArguments(argsForLde());
    lde->setProcessChannelMode(QProcess::ForwardedChannels);
    lde->start();
    if(runConfig->isGuiApp) runConfig->mainWindow->hide();
    if(!lde->waitForStarted(5000))
        throw("Failed to start maiko (lde)");
    return lde;
}

QProcessEnvironment Runner::envForLde()
{
    QProcessEnvironment ldeEnv(QProcessEnvironment::systemEnvironment());
    QString PATH = ldeEnv.value("PATH");
    PATH = runConfig->maikoExecDir.absolutePath() + QDir::listSeparator() + PATH;
    ldeEnv.insert("PATH", PATH);

    ldeEnv.insert("MEDLEYDIR", runConfig->medleyDir.absolutePath());
    ldeEnv.insert("MAIKODIR", runConfig->maikoDir.absolutePath());
    ldeEnv.insert("LOGINDIR", runConfig->logindir.value());
    ldeEnv.insert("LDESRCESYSOUT", runConfig->sysout.value());
    ldeEnv.insert("LDEDESTSYSOUT", runConfig->vmem.value());
    ldeEnv.insert("LDEINIT", runConfig->greet.value());
    if(runConfig->display.has_value())
        ldeEnv.insert("DISPLAY", runConfig->display.value());
    if(runConfig->interlisp_exec.has_value() && runConfig->interlisp_exec.value())
        ldeEnv.insert("MEDLEY_EXEC", "inter");
    ldeEnv.insert("LDEKBDTYPE", "x");

    return ldeEnv;
};

QStringList Runner::argsForLde()
{
    QStringList ldeArgs;
    ldeArgs << "-g" << runConfig->geometry.value();
    ldeArgs << "-sc" << runConfig->screensize.value();
    if(runConfig->noscroll.has_value() && runConfig->noscroll.value())
        ldeArgs << "-noscroll";
    if(runConfig->title.has_value())
        ldeArgs << "-title" << runConfig->title.value();
    if(runConfig->mem.has_value())
        ldeArgs << "-m" << QString::number(runConfig->mem.value());
    ldeArgs << runConfig->sysout.value();
    QTextStream xout(stdout);
    for(auto& argg : ldeArgs)
        xout << argg << " ";
    xout << Qt::endl;
    return ldeArgs;
}

//
//
// Helper functions for runMedley*
//
//

QString Runner::setupVNCViewer()
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

QProcess *Runner::startVNCViewer(QString viewerPath, int vncPort)
{
    QProcess *vncV = new QProcess();
    QStringList vncArgs;
    vncArgs << "-geometry" << "+50+50";
    vncArgs << "-ReconnectOnError=off";
    vncArgs << "−AlertOnFatalError=off";
    vncArgs << ipAddress().toString() + ":" + QString::number(vncPort);

    vncV->setProgram(viewerPath);
    vncV->setArguments(vncArgs);
    vncV->setProcessChannelMode(QProcess::ForwardedChannels);
    vncV->start();
    if(!vncV->waitForStarted(5000))
        throw("VNC requested, but failed to start vnc viewer");
    return vncV;
}


int Runner::findOpenDisplay()
{
    int result = -1;
    for(int ctr = 1; ctr <= 64; ctr++) {
        QFileInfo fi = QFileInfo(QStringLiteral("/tmp/.X%1-lock").arg(ctr));
        if(!fi.exists()) {
            result = ctr;
            break;
        }
        else {
            QString locked_pid;
            QFile file = QFile(fi.absoluteFilePath());
            if(file.open(QIODeviceBase::ReadOnly)) {
                locked_pid = QTextStream(&file).readAll();
                file.close();
            } else continue;
            if(!activePIDs().contains(locked_pid)) {
                result = ctr;
                break;
            }
        }
    }
    return result;
}

QStringList Runner::activePIDs()
{
    QProcess p;
    p.startCommand(QStringLiteral("ps -ax -o pid="));
    p.waitForFinished();
    QTextStream t(p.readAllStandardOutput());
    QStringList r;
    while(!t.atEnd())
        r << t.readLine();
    return r;
}

int Runner::findOpenPort()
{
    QTcpServer server;
    for(int port = 5900; port < 6000; ++port) {
        if(server.listen(QHostAddress::Any, port)) {
            server.close();
            return port;
        }
    }
    return -1;
}

QString Runner::windowsUserProfileFromWSL()
{
    QProcess p;
    p.startCommand(QStringLiteral("cmd.exe /c <nul set /p=%UserProfile%"));
    p.waitForFinished();
    QTextStream t(p.readAllStandardOutput());
    return t.readLine();
}

bool Runner::checkForXvnc()
{
    if(MedleyApp::searchEnvPathForExec(QStringLiteral("Xvnc")).isNull())
        return false;
    QProcess p;
    p.startCommand(QStringLiteral("Xvnc -version"));
    p.waitForFinished();
    if(!QString(p.readAllStandardOutput()).contains(QStringLiteral("tigervnc")))
        return false;
    return true;
}

QHostAddress Runner::ipAddress()
{
    QHostAddress ip;
    QNetworkInterface eth1Ip = QNetworkInterface::interfaceFromName("eth0");
    QList<QNetworkAddressEntry> entries = eth1Ip.addressEntries();
    if (!entries.isEmpty()) {
        for(auto& entry : entries) {
            if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                ip = entry.ip();
                break;
            }
        }
    }
    return ip;
}

void Runner::terminateVncViewer(QProcess *vncViewerProcess)
{
    if(vncViewerProcess->state() != QProcess::NotRunning) {

    }
}

