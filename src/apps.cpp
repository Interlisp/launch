#include "apps.h"

#include <QMessageBox>

CoreApplication::CoreApplication(int &argc, char **argv): MedleyApp(argc, argv), QCoreApplication(argc, argv)
{
    config->isGuiApp = false;
}

CoreApplication::~CoreApplication()
{

}

int CoreApplication::startApp()
{
    QTextStream errOut(stderr);
    try {
        figureOutDirectories(config);
        // Look thru arg list to see if there is a --save with ~wo or ~none value
        // to determine if we read the config file or not.
        bool readConfig = true;
        QStringList args = QCoreApplication::arguments();
        for(qsizetype idx = 0; idx < args.size(); ++idx) {
            QString arg1 = args.at(idx);
            QString arg2 = (idx+1 < args.size()) ? args.at(idx+1) : QString();
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
        if(readConfig) readConfigFile(config);

        QStringList *argList = new QStringList(QCoreApplication::arguments());
        config->processArgList(argList, false);
        delete argList;

        runMedley();

    } catch(QString err) {
        errOut << err << Qt::endl;
        return 1;
    }
    return 0;
}

Application::Application(int &argc, char **argv): MedleyApp(argc, argv), QApplication(argc, argv)
{
    config->isGuiApp = true;
}

Application::~Application()
{

}

int Application::startApp()
{
    QString err = QString();
    try {
        figureOutDirectories(config);
        readConfigFile(config);
    } catch(QString err_msg) {
        err = err_msg;
        QMessageBox::critical(nullptr, "Error", err);
        std::exit(1);
    }
    config->mainWindow = new MainWindow();
    config->mainWindow->show();
    return exec();
}

