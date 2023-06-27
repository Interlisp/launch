#ifndef CONFIG_H
#define CONFIG_H

#include "mainwindow.h"

#include <optional>
#include <QChar>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QDir>

#define DEFAULT_TITLE "Medley Interlisp"
#define DEFAULT_DISPLAY ":0"
#define DEFAULT_ID "default"
#define DEFAULT_SCROLLBAR_WIDTH 22
#define DEFAULT_GEO_WIDTH 1440
#define DEFAULT_GEO_HEIGHT 900
#define DEFAULT_SCREENSIZE_WIDTH 1440-DEFAULT_SCROLLBAR_WIDTH
#define DEFAULT_SCREENSIZE_HEIGHT 900-DEFAULT_SCROLLBAR_WIDTH
#define DEFAULT_MEMORY 256

class Config
{
public:
    Config();
    Config(Config &config);
    ~Config();
    void determineContext();
    void processArgList(QStringList *argList, bool fromConfigFile);
    void toTextStream(QTextStream &out, QChar separator);
    void prepareConfigForRunMedley();
    struct Geometries {
        int gw;
        int gh;
        int sw;
        int sh;
        Geometries(int gw, int gh, int sw, int sh): gw(gw), gh(gh), sw(sw), sh(sh) {}
    };
    struct Geometries figureOutGeometries(bool setConfig = false);
    // -a (reserved for synomym of --sysout ~apps)
    // -b (Windows only)
    std::optional<bool> background;
    // -c
    bool save_config;
    // -d
    std::optional<QString> display;
    // -e
    std::optional<bool> interlisp_exec;
    // -f (reserved for synomym of --sysout ~full)
    // -g
    std::optional<QString> geometry;
    // -h (reserved for synonym of --help)
    // -i
    std::optional<QString> id;
    // -j (available)
    // -k
    std::optional<QString> vmem;
    // -l (reserved for synomym of --sysout ~lisp)
    // -m
    std::optional<int> mem;
    // -n
    std::optional<bool> noscroll;
    // -o
    std::optional<QString> greet;
    // -p (Windows only)
    std::optional<int> port;
    // -q (available)
    // -r (reserved for synonym of --sysout ~resume)
    // -s
    std::optional<QString> screensize;
    // -t
    std::optional<QString> title;
    // -u (Windows only)
    std::optional<bool> update;
    // -v
    std::optional<bool> vnc;
    // -w (Windows only)
    std::optional<bool> use_wsl;
    // -x
    std::optional<QString> logindir;
    // -y
    std::optional<QString> sysout;
    // -z (reserved as synonym for --man
    // -draft
    std::optional<QString> dockerTag;


    bool isGuiApp;
    bool isLinux;
    bool isWSL;
    uint8_t wslVersion;
    bool isDocker;
    bool isMacOS;
    bool isMacOSBundle;
    bool isWindows;
    QString osString;
    QString arch;

    QDir medleyDir;
    QDir maikoDir;
    QDir maikoExecDir;
    QDir defaultLoginDir;
    QString greetFileNoGreet;
    QString greetFileDefault;
    QString greetFileApps;
    QDir invokeDir;
    MainWindow *mainWindow;
    QDir ilDir;

    static QRegularExpression re_xOrX;
    static QRegularExpression re_WxH;
    static QRegularExpression re_ExceptAlphaNum;
    static QRegularExpression re_VersionedFile;

private:
    static QStringList sysoutTildes;
    bool checkForDashOrEnd(QString arg1, std::optional<QString> arg2);
};


#endif // CONFIG_H
