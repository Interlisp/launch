#include <QApplication>
#include <QStringList>
#include <QFileInfo>
#include <QTextStream>
#include <QRegularExpression>

#include "apps.h"
#include "main.h"

static QRegularExpression isGuiArg = QRegularExpression("^-{1,2}gui$");
static QRegularExpression isCliArg = QRegularExpression("^-{1,2}cli$");


int main(int argc, char *argv[])
{
    bool isGuiApp;


    // Look for --gui or --cli as first arg - or of not,
    // at baseName of this executable: if it starts with 'g' run with GUI
    // otherwise run with cli
    {
        QString arg1(argv[1]);
        if(arg1.contains(isGuiArg))
            isGuiApp = true;
        else if (arg1.contains(isCliArg))
            isGuiApp = false;
        else {
            QString baseName = QFileInfo(QString(argv[0])).baseName();
            isGuiApp = (baseName.at(0) == QChar('g'));
        }
    // DEBUG
    isGuiApp=true;
    }

    // Start the app
    if(isGuiApp) {
        Application *app = new Application(argc, argv);
        setupOrganizationEtc();
        return app->startApp();
    } else {
        CoreApplication *app = new CoreApplication(argc, argv);
        setupOrganizationEtc();
        return app->startApp();
    }
}


void setupOrganizationEtc() {
    QCoreApplication::setOrganizationName("Interlisp.org");
    QCoreApplication::setOrganizationDomain("interlisp.org");
    QCoreApplication::setApplicationName("medley");
    QCoreApplication::setApplicationVersion("0.1");
}

