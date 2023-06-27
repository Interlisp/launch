#include "config.h"
#include "configfile.h"
#include "medleyapp.h"

#include <cstdlib>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDir>
#include <QSysInfo>
#include <QErrorMessage>
#include <QMessageBox>
#include <QFileInfo>

MedleyApp *MedleyApp::app = nullptr;
Config *MedleyApp::config = nullptr;

MedleyApp::MedleyApp(int &argc, char **argv) :
    argv0(QString(argv[0]))
{
    MedleyApp::app = this;
    config = new Config();
    config->determineContext();
}

MedleyApp::~MedleyApp()
{

}

// Read the config file
void MedleyApp::readConfigFile(Config *config)
{
    QStringList *argList = ConfigFile().readConfig();
    config->processArgList(argList, true);
    delete argList;
}

void MedleyApp::figureOutDirectories(Config *config)
{
    // MEDLEYDIR
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if(env.contains("MEDLEYDIR")) {
        config->medleyDir = QDir(env.value("MEDLEYDIR"));
    } else {
        QFileInfo *invokeFi = nullptr;
        if(argv0.startsWith(QChar('.')) || argv0.contains(QChar('/')))
            invokeFi = new QFileInfo(argv0);
        else {
            QString searchedFilePath = searchEnvPathForExec(argv0);
            if(!searchedFilePath.isNull())
                invokeFi = new QFileInfo(searchedFilePath);
        }
        if(invokeFi != nullptr) {
            QString mdPath;
            mdPath = figureOutMedleyDir(invokeFi->path());
            if(!mdPath.isNull())
                config->medleyDir = QDir(mdPath);
            else if(invokeFi->isSymLink()) {
                QFileInfo targetFi;
                targetFi = QFileInfo(invokeFi->symLinkTarget());
                mdPath = figureOutMedleyDir(targetFi.path());
                if(!mdPath.isNull())
                    config->medleyDir = QDir(mdPath);
            }
        }
        if(invokeFi != nullptr) delete invokeFi;
    }
    if(config->medleyDir.dirName().isEmpty())
        throw("Unable to find MedleyDir");

    // IL_DIR
    config->ilDir = QDir::cleanPath(QDir(config->medleyDir.absolutePath() + "/..").absolutePath());

    //MAIKODIR
    if(config->isMacOSBundle) {
        config->maikoDir = QDir(config->medleyDir.absolutePath() + "/../../MacOS/maiko");
        config->maikoDir.makeAbsolute();
        config->maikoExecDir = QDir(config->maikoDir);
    }
    else
    {
        QString maikoPath = figureOutMaikoDir(config->medleyDir.absolutePath());
        if(maikoPath.isNull())
            throw(QStringLiteral("Unable to find Maiko directory"));
        config-> maikoDir = QDir(maikoPath);
        config->maikoExecDir = QDir(testMaikoExecDir(maikoPath));
    }

    // LOGINDIR
    if(QFileInfo(config->medleyDir.absolutePath()).isWritable())
        config->defaultLoginDir = QDir(config->medleyDir.absolutePath() + "/il");
    else
        config->defaultLoginDir = QDir(QDir::homePath() + "/il");

    // GREET FILES
    QDir greetfilesDir = QDir(config->medleyDir.absolutePath() + "/greetfiles");
    if(!greetfilesDir.exists())
        throw(QStringLiteral("greetfiles subdirectory of MEDLEYDIR (%1) does not exist"));
    greetfilesDir.setFilter(QDir::Files | QDir::Readable);
    greetfilesDir.setSorting(QDir::Type);
    QFileInfoList greetfiles = greetfilesDir.entryInfoList();
    for(auto& fi : greetfiles) {
        if(fi.fileName().contains(Config::re_VersionedFile)) continue;
        QString bn = fi.baseName();
        if(bn == QStringLiteral("NOGREET"))
            config->greetFileNoGreet = fi.absoluteFilePath();
        else if(bn == QStringLiteral("MEDLEYDIR-INIT"))
            config->greetFileDefault = fi.absoluteFilePath();
        else if(bn == QStringLiteral("APPS-INIT"))
            config->greetFileApps = fi.absoluteFilePath();
    }
    if(config->greetFileNoGreet.isNull())
        throw(QStringLiteral("NoGreet INIT file (<MEDLEYDIR>/greetfiles/NOGREET[.LCOM]) either doesn't exist or is not readable"));
    if(config->greetFileDefault.isNull())
        throw(QStringLiteral("Default INIT file (<MEDLEYDIR>/greetfiles/MEDLEYDIR-INIT[.LCOM]) either doesn't exist or is not readable"));
    if(config->greetFileApps.isNull())
        config->greetFileApps = config->greetFileDefault;
}

QString MedleyApp::figureOutMedleyDir(QString invokePath) {
    QFileInfo fi = QFileInfo(invokePath + "/../lispusers");
    if(fi.exists() && fi.isDir())
        return QDir::cleanPath(QDir(invokePath + "/../").absolutePath());
    if(config->isMacOS) {
        fi = QFileInfo(invokePath + "/../../");
        if (fi.isBundle()) {
            config->isMacOSBundle = true;
            return fi.absolutePath() + "/Resources/medley";
        }
    }
    return QString();
}

QString MedleyApp::figureOutMaikoDir(QString medleyDir) {
    QString maikoDirCandidate = medleyDir + "/maiko";
    QFileInfo fi = QFileInfo(maikoDirCandidate);
    if(fi.exists() && fi.isDir() && testMaikoExecDir(maikoDirCandidate).isNull())
        return QDir::cleanPath(QDir(maikoDirCandidate).absolutePath());
    else {
        maikoDirCandidate = medleyDir + "/../maiko";
        fi = QFileInfo(maikoDirCandidate);
        if(fi.exists() && fi.isDir() && !testMaikoExecDir(maikoDirCandidate).isNull())
            return QDir::cleanPath(QDir(maikoDirCandidate).absolutePath());
        else return QString();
    }
}

QString MedleyApp::testMaikoExecDir(QString maikoDirCandidate) {
    QDir dir = QDir(maikoDirCandidate + "/" + config->osString + "." + config->arch);
    if (dir.exists() && dir.exists("lde") && dir.exists("ldex"))
        return dir.absolutePath();
    else return QString();
}

QString MedleyApp::searchEnvPathForExec(QString execName) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QStringList envPathList = env.value("PATH").split(QDir::listSeparator());
    for(qsizetype idx = 0; idx < envPathList.size(); ++idx) {
        QFileInfo fi = QFileInfo(envPathList.at(idx) + "/" + execName);
        if(fi.exists() && fi.isExecutable()) {
            return fi.filePath();
        }
    }
    return QString();
}


