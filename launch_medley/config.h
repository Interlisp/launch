#ifndef CONFIG_H
#define CONFIG_H

#include <optional>
#include <QChar>
#include <QString>
#include <QStringList>
#include <QTextStream>

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
    Config(QStringList *argList, bool fromConfigFile = false);
    ~Config();
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

    // -b (Windows only)
    std::optional<bool> background;
    // -c
    bool save_config;
    // -d
    std::optional<QString> display;
    // -e
    std::optional<bool> interlisp_exec;
    // -g
    std::optional<QString> geometry;
    // -i
    std::optional<QString> id;
    // -k
    std::optional<QString> vmem;
    // -m
    std::optional<int> mem;
    // -n
    std::optional<bool> noscroll;
    // -o
    std::optional<QString> greet;
    // -p (Windows only)
    std::optional<int> port;
    // -s
    std::optional<QString> screensize;
    // -t
    std::optional<QString> title;
    // -v
    std::optional<bool> vnc;
    // -w (WSL only)
    std::optional<bool> wsl;
    // -x
    std::optional<QString> logindir;
    // -y
    std::optional<QString> sysout;

    static QRegularExpression re_xOrX;
    static QRegularExpression re_WxH;

private:
    static QStringList sysoutTildes;
    bool checkForDashOrEnd(QString arg1, std::optional<QString> arg2);
};


#endif // CONFIG_H
