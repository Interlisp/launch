#include "runmedley.h"
#include "medleyapp.h"
#include "config.h"

#include <QProcess>
#include <QProcessEnvironment>
#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QTcpServer>

//
//  Basic runMedley - based on existing config,
//       set up environment variables and command line arguments
//       for call to lde, then call lde.
//
void MedleyApp::runMedley() {

    Config *save = MedleyApp::runConfig;
    MedleyApp::runConfig = new Config(*MedleyApp::config);
    if(save != nullptr) delete save;
    Config *config = MedleyApp::runConfig;

    config->prepareConfigForRunMedley();

    QProcess lde;
    QProcessEnvironment ldeEnv = QProcessEnvironment(QProcessEnvironment::systemEnvironment());
    QStringList ldeArgs;
    QString ldePath = maikoExecDir.absolutePath() + "/lde";

    QString PATH = ldeEnv.value("PATH");
    PATH = maikoExecDir.absolutePath() + QDir::listSeparator() + PATH;
    ldeEnv.insert("PATH", PATH);

    ldeEnv.insert("MEDLEYDIR", medleyDir.absolutePath());
    ldeEnv.insert("MAIKODIR", maikoDir.absolutePath());
    ldeEnv.insert("LOGINDIR", config->logindir.value());
    ldeEnv.insert("LDESRCESYSOUT", config->sysout.value());
    ldeEnv.insert("LDEDESTSYSOUT", config->vmem.value());
    ldeEnv.insert("LDEINIT", config->greet.value());
    if(config->display.has_value())
        ldeEnv.insert("DISPLAY", config->display.value());
    if(config->interlisp_exec.has_value() && config->interlisp_exec.value())
        ldeEnv.insert("MEDLEY_EXEC", "inter");
    ldeEnv.insert("LDEKBDTYPE", "x");
    lde.setProcessEnvironment(ldeEnv);

    ldeArgs << "-g" << config->geometry.value();
    ldeArgs << "-sc" << config->screensize.value();
    if(config->noscroll.has_value() && config->noscroll.value())
        ldeArgs << "-noscroll";
    if(config->title.has_value())
        ldeArgs << "-title" << config->title.value();
    if(config->mem.has_value())
        ldeArgs << "-m" << QString::number(config->mem.value());
    ldeArgs << config->sysout.value();
    QTextStream xout(stdout);
    for(auto& argg : ldeArgs)
        xout << argg << " ";
    xout << Qt::endl;


    lde.setArguments(ldeArgs);

    lde.setProgram(ldePath);

    lde.setProcessChannelMode(QProcess::ForwardedChannels);
    lde.start();
    if(isGuiApp) mainWindow->hide();
    if(!lde.waitForStarted(5000)) {
        throw("Failed to start maiko (lde)");
    }
    lde.waitForFinished(-1);
    std::exit(0);
}



//
//
// Helper functions for runMedley*
//
//
int findOpenDisplay()
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

QStringList activePIDs()
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

int findOpenPort()
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

QString windowsUserProfileFromWSL()
{
    QProcess p;
    p.startCommand(QStringLiteral("cmd.exe /c <nul set /p=%UserProfile%"));
    p.waitForFinished();
    QTextStream t(p.readAllStandardOutput());
    return t.readLine();
}

bool checkForXVnc()
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

bool checkForVNCViewer()
{

}









