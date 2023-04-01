#include "apps.h"
#include "config.h"
#include "configfile.h"
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
#include <QMessageBox>
#include <QFileInfo>

MedleyApp *MedleyApp::app = nullptr;
Config *MedleyApp::config = nullptr;
Config *MedleyApp::runConfig = nullptr;

MedleyApp::MedleyApp(int &argc, char **argv) :
      argv0(QString(argv[0]))
    , env(QProcessEnvironment())
    , medleyDir(QDir())
    , isGuiApp(false)
    , isMacOSBundle(false)
    , greetFileDefault(QString())
    , greetFileNoGreet(QString())
    , greetFileApps(QString())
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
            throw(QStringLiteral("Unable to find Maiko directory"));
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
    QDir greetfilesDir = QDir(medleyDir.absolutePath() + "/greetfiles");
    if(!greetfilesDir.exists())
        throw(QStringLiteral("greetfiles subdirectory of MEDLEYDIR (%1) does not exist"));
    greetfilesDir.setFilter(QDir::Files | QDir::Readable);
    greetfilesDir.setSorting(QDir::Type);
    QFileInfoList greetfiles = greetfilesDir.entryInfoList();
    for(auto& fi : greetfiles) {
        if(fi.fileName().contains(Config::re_VersionedFile)) continue;
        QString bn = fi.baseName();
        if(bn == QStringLiteral("NOGREET"))
            greetFileNoGreet = fi.absoluteFilePath();
        else if(bn == QStringLiteral("MEDLEYDIR-INIT"))
            greetFileDefault = fi.absoluteFilePath();
        else if(bn == QStringLiteral("APPS-INIT"))
            greetFileApps = fi.absoluteFilePath();
    }
    if(greetFileNoGreet.isNull())
        throw(QStringLiteral("NoGreet INIT file (<MEDLEYDIR>/greetfiles/NOGREET[.LCOM]) either doesn't exist or is not readable"));
    if(greetFileDefault.isNull())
        throw(QStringLiteral("Default INIT file (<MEDLEYDIR>/greetfiles/MEDLEYDIR-INIT[.LCOM]) either doesn't exist or is not readable"));
    if(greetFileApps.isNull())
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
    QTextStream xout(stdout);
    xout << "7777777777777777777777777777" << Qt::endl;
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
    QString err = QString();
    try {
        figureOutDirectories();
        config = readConfigFile();
    } catch(QString err_msg) {
        err = err_msg;
        QMessageBox::critical(nullptr, "Error", err);
        std::exit(1);
    }
    mainWindow = new MainWindow();
    mainWindow->show();
    return exec();
}

