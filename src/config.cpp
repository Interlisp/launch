#include "config.h"
#include "mainwindow.h"

#include <QFileInfo>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QRegularExpression>
#include <QProcessEnvironment>

QStringList Config::sysoutTildes = QStringList("~apps") << "~full" << "~lisp" << "~resume" << "~default";
QRegularExpression Config::re_xOrX = QRegularExpression("[xX]");
QRegularExpression Config::re_WxH = QRegularExpression("^[0-9]+[Xx][0-9]+$");
QRegularExpression Config::re_ExceptAlphaNum = QRegularExpression("[^0-9a-zA-Z]");
QRegularExpression Config::re_VersionedFile = QRegularExpression(".~[0-9]+~$");

Config::Config()
{
    //
    // Defaults
    //
    // -b
    background = std::nullopt;
    // -c
    save_config = false;
    // -d
    display = std::nullopt;
    // -e
    interlisp_exec = std::nullopt;
    // -g
    geometry = std::nullopt;
    // -i
    id = std::nullopt;
    // -k
    vmem = std::nullopt;
    // -m
    mem = std::nullopt;
    // -n
    noscroll = false;
    // -o
    greet = std::nullopt;
    // -p (Windows only)
    port = std::nullopt;
    // -s
    screensize = std::nullopt;
    // -t
    title = std::nullopt;
    // -v
    vnc = std::nullopt;
    // -w
    use_wsl= std::nullopt;
    // -y
    sysout = QStringLiteral("~resume");
    // -draft
    dockerTag = "latest";

 1  medleyDir = QDir();

    greetFileDefault = QString();
    greetFileNoGreet = QString();
    greetFileApps = QString();
    mainWindow = nullptr;
    ilDir = QDir();

    isGuiApp = false;
    isLinux = false;
    isWSL = false;
    wslVersion = 0;
    isDocker = false;
    isMacOS = false;
    isMacOSBundle = false;
    isWindows = false;
    osString = QString();
    arch = QString();

}

Config::Config(Config &config) {

    background = config.background;
    save_config = false;
    display = config.display;
    interlisp_exec = config.interlisp_exec;
    geometry = config.geometry;
    id = config.id;
    vmem = config.vmem;
    mem = config.mem;
    noscroll = config.noscroll;
    greet = config.greet;
    port = config.port;
    screensize = config.screensize;
    title = config.title;
    vnc = config.vnc;
    use_wsl = config.use_wsl;
    logindir = config.logindir;
    sysout = config.sysout;
    dockerTag = config.dockerTag;

    medleyDir = config.medleyDir;noscroll
    maikoDir = config.maikoDir;
    maikoExecDir = config.maikoExecDir;
    defaultLoginDir = config.defaultLoginDir;
    greetFileNoGreet = config.greetFileNoGreet;
    greetFileDefault = config.greetFileDefault;
    greetFileApps = config.greetFileApps;
    invokeDir = config.invokeDir;
    mainWindow = config.mainWindow;
    ilDir = config.ilDir;

    isGuiApp = config.isGuiApp;
    isLinux = config.isLinux;
    isWSL = config.isWSL;
    wslVersion = config.wslVersion;
    isDocker = config.isDocker;
    isMacOS = config.isMacOS;
    isMacOSBundle = config.isMacOSBundle;
    isWindows = config.isWindows;
    osString = config.osString;
    arch = config.arch;
}

Config::~Config() {
}

constexpr uint32_t hash(const char* data) noexcept{
    uint32_t hash = 5381;
    size_t size = 0;
    for(; size<=4096; ++size) { if (data[size] == 0) break; }

    for(const char *c = data; c < data + size; ++c)
        hash = ((hash << 5) + hash) + (unsigned char) *c;

    return hash;
}

void Config::determineContext() {
    arch = QSysInfo::currentCpuArchitecture();
#ifdef Q_OS_LINUX
    isLinux = true;
    osString = QStringLiteral("linux");
    if(! QProcessEnvironment::systemEnvironment().value(QStringLiteral("MEDLEY_DOCKER_BUILD_DATE")).isNull())
        isDocker = true;
    else {
        QFile f("/proc/sys/kernel/osrelease");
        QString s(f.readAll());
        if(s.contains("wsl", Qt::CaseInsensitive)) {
            isWSL = true;
            wslVersion = 2;
        }
        else if(s.contains("microsoft", Qt::CaseInsensitive)) {
            isWSL = true;
            wslVersion = 1;
#ifndef Q_PROCESSOR_X86_64
            throw("Running Medley on WSL1 requires an x86_64-based PC.");
#endif
        }
    }
#elif Q_OS_MACOS
    isMacOS = true;
    osString = QStringLiteral("darwin");
#elif Q_OS_WINDOWS
    isWindows = true;
    osString = QStringLiteral("win");
#endif  /* Q_OS */
}

void Config::processArgList(QStringList *argList, bool fromConfigFile) {

    //
    //  Replace fields with with values from argList (if any)
    //
    qsizetype len = argList->size();
    for (qsizetype idx = 0; idx < len; ++idx) {
        const QString arg1 = argList->at(idx);
        std::optional<QString> arg2;
        if (idx+1 < len) arg2 = argList->at(idx+1);

        switch(hash(arg1.toStdString().c_str())) {


        // Special args - handled elsewhere but ignored here
        case hash("--gui"):
        case hash("-gui"):
        case hash("--cli"):
        case hash("-cli"):
            // Ignore - handled elsewhere
        break;

        // "Regular args
        case hash("-a"):
        case hash("--apps"):
            sysout = QStringLiteral("~apps");
        break;

        case hash("-b"):
        case hash("--background"):
            /* Windows only - ignore for now */
        break;

        case hash("-c"):
        case hash("--config"):
            /* medley command line arg - but should not be in config file - ignore if so*/
            if(checkForDashOrEnd(arg1, arg2)) {
                if(!fromConfigFile
                    && ((arg2 == QStringLiteral("~wo")) || (arg2 == QStringLiteral("~write-only"))
                        || ((arg2 == QStringLiteral("~rw")) || (arg2 == QStringLiteral("~read-write")))
                       )
                  )
                    save_config = true;
                idx++;
            }
        break;

        case hash("-d"):
        case hash("--display"):
            if(checkForDashOrEnd(arg1, arg2)) {
                if(! ((arg2 == QStringLiteral("~d")) || (arg2 == QStringLiteral("~default")))) {
                    /* TODO: do some format checking on arg2 here */
                    display = arg2.value();
                }
                idx++;
            }
        break;

        case hash("-e"):
        case hash("-e-"):
        case hash("--interlisp"):
        case hash("--interlisp-"):
            if(arg1.back() != QChar('-')) interlisp_exec = true;
        break;

        case hash("-f"):
        case hash("--full"):
            sysout = QStringLiteral("~full");
            break;

        case hash("-g"):
        case hash("--geometry"):
            if(checkForDashOrEnd(arg1, arg2)) {
                if(! ((arg2 == QStringLiteral("~d")) || (arg2 == QStringLiteral("~default")))) {
                    /* TODO: do some format checking on arg2 here */
                    geometry = arg2.value();
                }
                idx++;
            }
        break;

        case hash("-h"):
        case hash("--help"):
            /* medley command line arg - but should not be in config file - ignore */
        break;

        case hash("-i"):
        case hash("--id"):
            if(checkForDashOrEnd(arg1, arg2)) {
                if(! ((arg2 == QStringLiteral("~d")) || (arg2 == QStringLiteral("~default")))) {
                    /* TODO: do some format checking on arg2 here */
                    id = arg2.value();
                }
                idx++;
            }
        break;

        case hash("-k"):
        case hash("--vmem"):
            if(checkForDashOrEnd(arg1, arg2)) {
                if(! ((arg2 == QStringLiteral("~d")) || (arg2 == QStringLiteral("~default")))) {
                    //checkFileWriteableOrCreatable(arg1, arg2);
                    vmem = arg2.value();
                }
                idx++;
            }
        break;

        case hash("-l"):
        case hash("--lisp"):
            sysout = QStringLiteral("~lisp");
        break;

        case hash("-m"):
        case hash("--mem"):
            if(checkForDashOrEnd(arg1, arg2)) {
                if(! ((arg2 == QStringLiteral("~d")) || (arg2 == QStringLiteral("~default")))) {
                    mem = arg2.value().toInt();
                }
                idx++;
            }
        break;

        case hash("-n"):
        case hash("-n-"):
        case hash("--noscroll"):
        case hash("--noscroll-"):
            if(arg1.back() != QChar('-')) noscroll = true;
        break;

        case hash("-o"):
        case hash("--greet"):
            if(checkForDashOrEnd(arg1, arg2)) {
                if(! ((arg2 == QStringLiteral("~n")) || (arg2 == QStringLiteral("~none")))) {
                    greet = QStringLiteral("~none");
                } else if (! ((arg2 == QStringLiteral("~d")) || (arg2 == QStringLiteral("~default")))) {
                    // checkFileReadable(arg1 arg2);
                    greet = arg2.value();
                }
                idx++;
            }
        break;

        case hash("-p"):
        case hash("--port"):
            /* Windows only - ignore for now */
        break;

        case hash("-r"):
        case hash("--resume"):
            sysout = QStringLiteral("~resume");
        break;

        case hash("-s"):
        case hash("--screensize"):
            if(checkForDashOrEnd(arg1, arg2)) {
                if(! ((arg2 == QStringLiteral("~d")) || (arg2 == QStringLiteral("~default")))) {
                    /* TODO: do some format checking on arg2 here */
                    screensize = arg2.value();
                }
                idx++;
            }
        break;

        case hash("-t"):
        case hash("--title"):
            if(checkForDashOrEnd(arg1, arg2)) {
                if(! ((arg2 == QStringLiteral("~d")) || (arg2 == QStringLiteral("~default")))) {
                    /* TODO: do some format checking on arg2 here */
                    title = arg2.value();
                }
                idx++;
            }
        break;

        case hash("-u"):
        case hash("--update"):
            /* Windows only - ignore for now */
        break;

        case hash("-v"):
        case hash("-v-"):
        case hash("-vnc"):
        case hash("-vnc-"):
            if(isWSL) {
                if(arg1.back() == QChar('-')) vnc = false;
                else vnc = true;
            }
        break;

        case hash("-w"):
        case hash("--wsl"):
            /* Windows only - ignore for now */
        break;

        case hash("-x"):
        case hash("--logindir"):
            if(checkForDashOrEnd(arg1, arg2)) {
                if(! ((arg2 == QStringLiteral("~d")) || (arg2 == QStringLiteral("~default")))) {
                    /* TODO: do some format checking on arg2 here */
                    logindir = arg2.value();
                }
                idx++;
            }
        break;

        case hash("-y"):
        case hash("--sysout"):
            if(checkForDashOrEnd(arg1, arg2)) {
                switch (hash(arg2.value().toStdString().c_str())) {

                case hash("~a"):
                case hash("~apps"):
                case hash("~~apps"):
                    sysout = QStringLiteral("~apps");
                break;

                case hash("~d"):
                case hash("~default"):
                case hash("~~default"):
                    sysout = QStringLiteral("~default");
                break;

                case hash("~f"):
                case hash("~full"):
                case hash("~~full"):
                    sysout = QStringLiteral("~full");
                break;

                case hash("~l"):
                case hash("~lisp"):
                case hash("~~lisp"):
                    sysout = QStringLiteral("~lisp");
                break;

                case hash("~r"):
                case hash("~resume"):
                case hash("~~resume"):
                    sysout = QStringLiteral("~resume");
                break;

                default:
                    sysout = arg2.value();
                break;

                }
                idx++;
            }
        break;

        case hash("-z"):
        case hash("--man"):
            /* medley command line arg - but should not be in config file - ignore */
        break;

        case hash("-draft"):
        case hash("--draft"):
            draft = "draft";
        break;


        default:
            /* Unknow argument - ignore */
        break;

        }
    }

}

void Config::toTextStream(QTextStream &out, QChar separator) {

    if(isWindows) {
        if(background.has_value() && background.value()) {
            out << "--background" << separator;
            out.flush();
        }
    }

    if(display.has_value()) {
        out << "--display " << display.value() << separator;
        out.flush();
    }

    if(interlisp_exec.has_value() && interlisp_exec.value()) {
        out << "--interlisp" << separator;
        out.flush();
    }

    if(geometry.has_value()) {
        out << "--geometry " << geometry.value() << separator;
        out.flush();
    }

    if(id.has_value()) {
        out << "--id \"" << id.value() << "\"" << separator;
        out.flush();
    }

    if(vmem.has_value()) {
        out << "--vmem \"" << vmem.value() << "\"" << separator;
        out.flush();
    }

    if(mem.has_value()) {
        out << "--mem " << mem.value() << separator;
        out.flush();
    }

    if(noscroll.has_value() && noscroll.value()) {
        out << "--noscroll" << separator;
        out.flush();
    }

    if(greet.has_value()) {
        out << "--greet \"" << greet.value() << "\"" << separator;
        out.flush();
    }

    if(isWindows) {
        if(port.has_value()) {
            out << "--port " << port.value() << separator;
            out.flush();
        }
    }

    if(screensize.has_value()) {
        out << "--screensize " << screensize.value() << separator;
        out.flush();
    }

    if(title.has_value()) {
        out << "--title \"" << title.value() << "\"" << separator;
        out.flush();
    }

    if(isWSL) {
        if(vnc.has_value() && vnc.value()) {
            out << "--vnc" << separator;
            out.flush();
        }
    }

    if(isWindows) {
        if(use_wsl.has_value() && use_wsl.value()) {
            out << "--wsl" << separator;
            out.flush();
        }
    }

    if(logindir.has_value()) {
        out << "--logindir \"" << logindir.value() << "\"" << separator;
        out.flush();
    }

    if(sysout.has_value()) {
        out << "--sysout \"" << sysout.value() << "\"" << separator;
        out.flush();
    }

}

bool Config::checkForDashOrEnd(QString arg1, std::optional<QString> arg2 ) {
    if(!arg2.has_value()) return false;
    QString arg2_v = arg2.value();
    if (arg2_v.isNull() || arg2_v.isEmpty() || arg2_v.at(0) == QChar('-'))
        return false;
    return true;
}

//
//  Check validity of the values in Config object
//

void Config::prepareConfigForRunMedley()
{
    {
        if(id.has_value()) {
            QString idv = id.value();
            if(idv == QStringLiteral("~."))
                id = medleyDir.dirName();
            else if(idv == QStringLiteral("~..")) {
                QDir tmp_qdir = QDir(medleyDir);
                tmp_qdir.cdUp();
                id = tmp_qdir.dirName();
            }
            id = id.value().remove(re_ExceptAlphaNum);
        }
    }
    {
        if(!logindir.has_value())
            logindir = defaultLoginDir.absolutePath();
        QString ldv = logindir.value();
        QFileInfo fi = QFileInfo(ldv);
        if(fi.exists()) {
            if(!fi.isDir())
                throw(QStringLiteral("Logindir (%1) exists, but it is not a directory (or a symbolic link to a directory).").arg(ldv));
            else if(!fi.isWritable())
                throw(QStringLiteral("Logindir (%1) exists but is not writable.").arg(ldv));
        }
        else if(!QDir().mkpath(ldv))
            throw(QStringLiteral("Logindir (%1) does not exists and unable to create the directory").arg(ldv));
    }

    {
        if(!greet.has_value() || (greet.value() == QStringLiteral("~default"))) {
            if(sysout.has_value() && (sysout == QStringLiteral("~apps")))
                greet = greetFileApps;
            else greet = greetFileDefault;
        } else {
            QString gv = greet.value();
            if(gv == QStringLiteral("~none"))
                greet = greetFileNoGreet;
            else {
                QFileInfo fi = QFileInfo(gv);
                if(!fi.exists()) {
                    throw(QStringLiteral("Greet (INIT) file (%1) does not exist").arg(gv));
                } else if (!fi.isFile()) {
                    throw(QStringLiteral("Greet (INIT) file (%1) exists, but is a directory or other non-regular file").arg(gv));
                } else if (!fi.isReadable()) {
                    throw(QStringLiteral("Greet (INIT) file (%1) exists, but is not readable").arg(gv));
                }
            }
        }
    }

    {
        if(!sysout.has_value())
            sysout = QStringLiteral("~full");
        QString sv = sysout.value();
        QString mdpath = medleyDir.absolutePath();
        if(sysoutTildes.contains(sv)) {
            if(sv == QStringLiteral("~apps"))
                    sysout = mdpath + "/loadups/apps.sysout" ;
            else if(sv == QStringLiteral("~full"))
                    sysout = mdpath + "/loadups/full.sysout";
            else if(sv == QStringLiteral("~lisp"))
                    sysout = mdpath + "/loadups/lisp.sysout";
            else if(sv == QStringLiteral("~resume"))
                sysout = QDir(logindir.value()).absolutePath()
                         + "/vmem/lisp"
                         + (id.has_value() ? ("_" + id.value()) : QStringLiteral(""))
                         + ".virtualmem";
        }
        QFileInfo fi = QFileInfo(sysout.value());
        if(! (fi.exists() && fi.isFile() && fi.isReadable()))
                throw(QStringLiteral("Sysout file (%1) either doesn't exist, is a directory, or is not readable.").arg(sysout.value()));
    }

    {
        if(!vmem.has_value()) {
            vmem = QProcessEnvironment::systemEnvironment().value(QStringLiteral("LDEDESTSYSOUT"));
            if (vmem.value().isNull())
                vmem = QDir(logindir.value()).absolutePath()
                         + "/vmem/lisp"
                         + (id.has_value() ? ("_" + id.value()) : QStringLiteral(""))
                         + ".virtualmem";
        }
        QString vmv = vmem.value();
        QFileInfo fi = QFileInfo(vmv);
        if(!fi.exists()) {
            QFile file = QFile(fi.absoluteFilePath());
            QDir dir = QDir(fi.absolutePath());
            if(!dir.exists() && !dir.mkpath(dir.absolutePath()))
                throw(QStringLiteral("Virtual memory file (%1) does not exist and cannot be created (1)").arg(vmv));
            if(file.open(QIODeviceBase::WriteOnly, QFileDevice::ReadOwner | QFileDevice::WriteOwner)) {
                file.close();
                file.remove();
            }
            else
                throw(QStringLiteral("Virtual memory file (%1) does not exist and cannot be created (2)").arg(vmv));
        }
        else if(!fi.isWritable()) {
            throw(QStringLiteral("Virtual memory file (%1) exists, but is not writable").arg(vmv));
        }
        else if(fi.isDir()) {
            throw(QStringLiteral("Virtual memory file (%1) exists, but is a directory").arg(vmv));
        }
    }

    figureOutGeometries(true);

}

//
//  Based on settings of Geometry, ScreenSize and NoScroll, figure out
//  what the Geometry and ScreenSize configs should be
//
struct Config::Geometries Config::figureOutGeometries(bool setConfig)
{
    int geo_width = DEFAULT_GEO_WIDTH;
    int geo_height = DEFAULT_GEO_HEIGHT;
    int scroll_width = (noscroll.has_value() && noscroll.value()) ? DEFAULT_SCROLLBAR_WIDTH : 0;
    int screensize_width = DEFAULT_SCREENSIZE_WIDTH - scroll_width;
    int screensize_height = DEFAULT_SCREENSIZE_HEIGHT - scroll_width;

    if(!geometry.has_value() && !screensize.has_value()) {
        if(setConfig) geometry = QStringLiteral("%1x%2").arg(geo_width).arg(geo_height);
    }

    if(geometry.has_value()) {
        if(geometry.value().contains(re_WxH)) {
            QStringList pair = geometry.value().split(re_xOrX);
            geo_width = pair.at(0).toInt();
            geo_height = pair.at(1).toInt();
        }
        else {
            geo_width = DEFAULT_GEO_WIDTH;
            geo_height = DEFAULT_GEO_HEIGHT;
        }
        if(setConfig) geometry = QStringLiteral("%1x%2").arg(geo_width).arg(geo_height);
    }

    if(screensize.has_value()) {
        if(screensize.value().contains(re_WxH)) {
            QStringList pair = screensize.value().split(re_xOrX);
            screensize_width = (((31 + pair.at(0).toInt()) / 32) * 32);
            screensize_height = pair.at(1).toInt();
        }
        else {
            screensize_width = DEFAULT_SCREENSIZE_WIDTH;
            screensize_height = DEFAULT_SCREENSIZE_HEIGHT;
        }
        if(setConfig) screensize = QStringLiteral("%1x%2").arg(screensize_width).arg(screensize_height);
    }

    if(!(screensize.has_value() && geometry.has_value())) {
        if(geometry.has_value()) {
            screensize_width = (((31+geo_width)/32)*32) - scroll_width;
            screensize_height = geo_height - scroll_width;
            if(setConfig) screensize = QStringLiteral("%1x%2").arg(screensize_width).arg(screensize_height);
        } else {
            geo_width = screensize_width + scroll_width;
            geo_height = screensize_height + scroll_width;
            if(setConfig) geometry = QStringLiteral("%1x%2").arg(geo_width).arg(geo_height);
        }
    }

    return Geometries(geo_width, geo_height, screensize_width, screensize_height);
}

