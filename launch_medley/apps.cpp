#include "apps.h"
#include "config.h"
#include "ConfigFile.h"
#include "mainwindow.h"

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDir>

MedleyApp *MedleyApp::app = nullptr;

MedleyApp::MedleyApp(int &argc, char **argv)
{
    argv0 = QString(argv[0]);
    env = QProcessEnvironment();
    ldeEnv = QProcessEnvironment(env);
    medleyDir=QDir();
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
            mdPath = isMedleyDir(invokeFi->path());
            if(!mdPath.isNull())
                medleyDir = QDir(mdPath);
            else if(invokeFi->isSymLink()) {
                QFileInfo targetFi;
                targetFi = QFileInfo(invokeFi->symLinkTarget());
                mdPath = isMedleyDir(targetFi.path());
                if(!mdPath.isNull())
                    medleyDir = QDir(mdPath);
            }
        }
        if(invokeFi != nullptr) delete invokeFi;
    }
    if(medleyDir.dirName().isEmpty())
        throw("Unable to find MedleyDir");
    ldeEnv.insert("MEDLEYDIR", medleyDir.absolutePath());

    // LOGINDIR
    if(QFileInfo(medleyDir.absolutePath()).isWritable())
        defaultLoginDir = QDir(medleyDir.absolutePath() + "/il");
    else
        defaultLoginDir = QDir(QDir::homePath() + "/il");
}

QString MedleyApp::isMedleyDir(QString path) {
    QFileInfo fi = QFileInfo(path + "/../lispusers");
    if(fi.exists() && fi.isDir())
        return QDir::cleanPath(QDir(path).absolutePath());
    else return QString();
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

    figureOutDirectories();
    config = readConfigFile();

    MainWindow w(this, config);
    w.show();
    return exec();
}

