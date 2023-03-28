#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "apps.h"
#include "config.h"
#include <QMainWindow>
#include <QRegularExpression>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Application *app, Config *config, QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void ResumeCB_stateChanged(int arg1);
    void StartFromSysoutCB_stateChanged(int arg1);
    void AppsSysoutRB_toggled(bool checked);
    void InterlispExecCB_stateChanged(int arg1);
    void updateSysout();
    void CustomSysoutRB_toggled(bool checked);
    void CustomSysoutButton_clicked();
    void updateId();
    void NoScrollCB_stateChanged(int arg1);
    void GeoCB_stateChanged(int arg1);
    void updateGeometry();
    void updateWidthandHeightSBs();
    void ScreenSizeCB_stateChanged(int arg1);
    void updateScreenSize();
    void updateTitle();
    void updateDisplay();
    void updateMemory();
    void updateVmemFile();
    void VmemFileButton_clicked();
    void updateGreetFile();
    void GreetFileButton_clicked();
    void updateLoginDir();
    void LoginDirButton_clicked();
    void SaveButton_clicked();
    void ResetButton_clicked();
    void RestoreButton_clicked();

private:
    Ui::MainWindow *ui;
    Config *config;
    Application* app;

    void updateConfig(QCheckBox *CB, QLineEdit *LE, std::optional<QString> &configField);
    void updateConfig(QCheckBox *CB, QLineEdit *LE, QPushButton *BB, std::optional<QString> &configField);
    void updateConfig(QCheckBox *CB, QSpinBox *SB, std::optional<int> &configField);

    void connectUI();
    void resetUI();
    void configureUI();
    bool checkConfig();


};

#define UI_DEFAULT_TITLE DEFAULT_TITLE
#define UI_DEFAULT_DISPLAY DEFAULT_DISPLAY
#define UI_DEFAULT_ID DEFAULT_ID
#define UI_DEFAULT_GEO_WIDTH DEFAULT_GEO_WIDTH
#define UI_DEFAULT_GEO_HEIGHT DEFAULT_GEO_HEIGHT
#define UI_DEFAULT_SCREENSIZE_WIDTH DEFAULT_SCREENSIZE_WIDTH
#define UI_DEFAULT_SCREENSIZE_HEIGHT DEFAULT_SCREENSIZE_HEIGHT
#define UI_DEFAULT_MEMORY 256





#endif // MAINWINDOW_H
