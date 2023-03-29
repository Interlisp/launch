#include "apps.h"
#include "config.h"
#include "ConfigFile.h"
#include "mainwindow.h"

#include <cstdlib>
#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDir>
#include <QSysInfo>
#include <QErrorMessage>

MedleyApp *MedleyApp::app = nullptr;
Config *MedleyApp::config = nullptr;
Config *MedleyApp::runConfig = nullptr;

MedleyApp::MedleyApp(int &argc, char **argv) :
      argv0(QString(argv[0]))
    , env(QProcessEnvironment())
    , medleyDir(QDir())
    , isGuiApp(false)
    , isMacOSBundle(false)
{
    MedleyApp::app = this;
}

MedleyApp::~MedleyApp()
{

}

// Read the config file
Config* MedleyApp::readConfigFile()
{
    QStringList *argList = ConfigFile().readConfig();
    Config *c = new Config(argList, true);
    delete argList;
    return c;
}

void MedleyApp::figureOutDirectories()
{
    // MEDLEDIR
    if(env.isEmpty())
        env = QProcessEnvironment::systemEnvironment();
    if(env.contains("MEDLEYDIR")) {
        medleyDir = QDir(env.value("MEDLEYDIR"));
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
                medleyDir = QDir(mdPath);
            else if(invokeFi->isSymLink()) {
                QFileInfo targetFi;
                targetFi = QFileInfo(invokeFi->symLinkTarget());
                mdPath = figureOutMedleyDir(targetFi.path());
                if(!mdPath.isNull())
                    medleyDir = QDir(mdPath);
            }
        }
        if(invokeFi != nullptr) delete invokeFi;
    }
    if(medleyDir.dirName().isEmpty())
        throw("Unable to find MedleyDir");

    //MAIKODIR
    if(!isMacOSBundle) {
        QString maikoPath = figureOutMaikoDir(medleyDir.absolutePath());
        if(maikoPath.isNull())
            throw(505); // QStringLiteral("Unable to find Maiko directory")
        maikoDir = QDir(maikoPath);
        maikoExecDir = QDir(testMaikoExecDir(maikoPath));
    } else {
        maikoDir = QDir(medleyDir.absolutePath() + "/../../MacOS/maiko");
        maikoDir.makeAbsolute();
        maikoExecDir = QDir(maikoDir);
    }

    // LOGINDIR
    if(QFileInfo(medleyDir.absolutePath()).isWritable())
        defaultLoginDir = QDir(medleyDir.absolutePath() + "/il");
    else
        defaultLoginDir = QDir(QDir::homePath() + "/il");

    // GREET FILES
    greetFileNoGreet = medleyDir.absolutePath() + "/greetfiles/NOGREET";
    if(!QFileInfo(greetFileNoGreet).isReadable())
        throw(QStringLiteral("NoGreet INIT file (%1) either doesn't exist or is not readable").arg(greetFileNoGreet));
    greetFileDefault = medleyDir.absolutePath() + "/greetfiles/MEDLEYDIR_INIT";
    if(!QFileInfo(greetFileDefault).isReadable())
        greetFileDefault = greetFileNoGreet;
    greetFileApps = medleyDir.absolutePath() + "/greetfiles/APPS-INIT";
    if(!QFileInfo(greetFileApps).isReadable())
        greetFileApps = greetFileDefault;

}

QString MedleyApp::figureOutMedleyDir(QString invokePath) {
    QFileInfo fi = QFileInfo(invokePath + "/../lispusers");
    if(fi.exists() && fi.isDir())
        return QDir::cleanPath(QDir(invokePath + "/../").absolutePath());
    else {
        fi = QFileInfo(invokePath + "/../../");
        if (fi.isBundle()) {
            isMacOSBundle = true;
            return fi.absolutePath() + "/Resources/medley";
        }
        else return QString();
    }
}

QString MedleyApp::searchEnvPathForExec(QString execName) {
    if(env.isEmpty())
        env = QProcessEnvironment::systemEnvironment();
    QStringList envPathList = env.value("PATH").split(QDir::listSeparator());
    for(qsizetype idx = 0; idx < envPathList.size(); ++idx) {
        QFileInfo fi = QFileInfo(envPathList.at(idx) + "/" + execName);
        if(fi.exists() && fi.isExecutable()) {
            return fi.filePath();
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
    QDir dir = QDir(maikoDirCandidate + "/" + osversion() + "." + machinetype());
    if (dir.exists() && dir.exists("lde") && dir.exists("ldex"))
        return dir.absolutePath();
    else return QString();
}



void MedleyApp::runMedley() {

    Config *save = MedleyApp::runConfig;
    MedleyApp::runConfig = new Config(*MedleyApp::config);
    if(save != nullptr) delete save;
    Config *config = MedleyApp::runConfig;

    config->prepareConfigForRunMedley();

    QProcess lde;
    QProcessEnvironment ldeEnv = QProcessEnvironment(env);
    QStringList ldeArgs;
    QString ldePath = maikoExecDir.absolutePath() + "/lde";

    ldeEnv.insert("MEDLEYDIR", medleyDir.absolutePath());
    ldeEnv.insert("MAIKODIR", maikoDir.absolutePath());
    ldeEnv.insert("LOGINDIR", config->logindir.value());
    ldeEnv.insert("LDESRCESYSOUT", config->sysout.value());
    ldeEnv.insert("LDEDESTSYSOUT", config->vmem.value());
    ldeEnv.insert("LDEINIT", config->greet.value());
    if(config->display.has_value()) ldeEnv.insert("DISPLAY", config->display.value());
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
    config->sysout.value();
    lde.setArguments(ldeArgs);

    lde.setProgram(ldePath);

    lde.start();

}

QString MedleyApp::osversion()
{
#ifdef Q_OS_MACOS
    return QStringLiteral("darwin");
#endif
#ifdef Q_OS_LINUX
    return QStringLiteral("linux");
#endif
#ifdef Q_OS_WINDOWS
    return QStringLiteral("win");
#endif
}

QString MedleyApp::machinetype()
{
   return QSysInfo::currentCpuArchitecture();
}



CoreApplication::CoreApplication(int &argc, char **argv): MedleyApp(argc, argv), QCoreApplication(argc, argv)
{
    isGuiApp = false;
}

CoreApplication::~CoreApplication()
{

}

int CoreApplication::startApp()
{
    figureOutDirectories();

    // Look thru arg list to see if there is a --save with ~wo or ~none value
    // to determine if we read the config file or not.
    bool readConfig = true;
    QStringList args = QCoreApplication::arguments();
    for(qsizetype idx = 0; idx < args.size(); ++idx) {
        QString arg1 = args.at(idx);
        QString arg2 = (idx+1 >= args.size()) ? args.at(idx+1) : QString();
        if(((arg1 == QStringLiteral("-c")) || (arg1 == QStringLiteral("--config")))
           && (!arg2.isNull()
               && ((arg2 == QStringLiteral("~wo")) || (arg2 == QStringLiteral("~write-only"))
                   || ((arg2 == QStringLiteral("~n")) || (arg2 == QStringLiteral("~none")))
                  )
              )
           )
        {
            readConfig = false;
            break;
        }
    }

    config = readConfig ? readConfigFile() : new Config();

    QStringList *argList = new QStringList(QCoreApplication::arguments());
    config->processArgList(argList, false);
    delete argList;

    return 0;
}

Application::Application(int &argc, char **argv): MedleyApp(argc, argv), QApplication(argc, argv)
{
    isGuiApp = true;

}

Application::~Application()
{

}

int Application::startApp()
{
    //QString err = QString();
    int err = -1;
    try {
        figureOutDirectories();
        config = readConfigFile();
    } catch(int err_msg) {
        err = err_msg;
        QErrorMessage().showMessage(QString::number(err));
        std::exit(1);
    }
    mainWindow = new MainWindow();
    mainWindow->show();
    if(err > 0) {
        QErrorMessage(mainWindow).showMessage(QString::number(err));
    }
    return exec();
}

