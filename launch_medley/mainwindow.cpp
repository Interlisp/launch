#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "configfile.h"
#include <iostream>
#include "apps.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>

MainWindow::MainWindow(Application *app, Config *config, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , config(config)
    , app(app)
{
    ui->setupUi(this);
    connectUI();
    resetUI();
    configureUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//
//  Connect up the UI: link signals (e.g., clicked, toggled, stateChanged) from the various widgets
//         on MainWindow to slots (methods) on theMainwindowobject that handle these signals
//
void MainWindow::connectUI()
{
    connect(ui->ResumeCB,           SIGNAL(stateChanged(int)), this, SLOT(ResumeCB_stateChanged(int)));
    connect(ui->StartFromSysoutCB,  SIGNAL(stateChanged(int)), this, SLOT(StartFromSysoutCB_stateChanged(int)));
    connect(ui->AppsSysoutRB,       SIGNAL(toggled(bool)),     this, SLOT(AppsSysoutRB_toggled(bool)));
    connect(ui->InterlispExecCB,    SIGNAL(stateChanged(int)), this, SLOT(InterlispExecCB_stateChanged(int)));
    connect(ui->FullSysoutRB,       SIGNAL(toggled(bool)),     this, SLOT(updateSysout()));
    connect(ui->LispSysoutRB,       SIGNAL(toggled(bool)),     this, SLOT(updateSysout()));
    connect(ui->CustomSysoutRB,     SIGNAL(toggled(bool)),     this, SLOT(CustomSysoutRB_toggled(bool)));
    connect(ui->CustomSysoutButton, SIGNAL(clicked()),         this, SLOT(CustomSysoutButton_clicked()));
    connect(ui->IdCB,               SIGNAL(stateChanged(int)), this, SLOT(updateId()));
    connect(ui->IdLE,               SIGNAL(editingFinished()), this, SLOT(updateId()));
    connect(ui->NoScrollCB,         SIGNAL(stateChanged(int)), this, SLOT(NoScrollCB_stateChanged(int)));
    connect(ui->GeoCB,              SIGNAL(stateChanged(int)), this, SLOT(GeoCB_stateChanged(int)));
    connect(ui->GeoWidthSB,         SIGNAL(editingFinished()), this, SLOT(updateGeometry()));
    connect(ui->GeoHeightSB,        SIGNAL(editingFinished()), this, SLOT(updateGeometry()));
    connect(ui->ScreenSizeCB,       SIGNAL(stateChanged(int)), this, SLOT(ScreenSizeCB_stateChanged(int)));
    connect(ui->ScreenSizeWidthSB,  SIGNAL(editingFinished()), this, SLOT(updateScreenSize()));
    connect(ui->ScreenSizeHeightSB, SIGNAL(editingFinished()), this, SLOT(updateScreenSize()));
    connect(ui->TitleCB,            SIGNAL(stateChanged(int)), this, SLOT(updateTitle()));
    connect(ui->TitleLE,            SIGNAL(editingFinished()), this, SLOT(updateTitle()));
    connect(ui->DisplayCB,          SIGNAL(stateChanged(int)), this, SLOT(updateDisplay()));
    connect(ui->DisplayLE,          SIGNAL(editingFinished()), this, SLOT(updateDisplay()));
    connect(ui->MemoryCB,           SIGNAL(stateChanged(int)), this, SLOT(updateMemory()));
    connect(ui->MemorySB,           SIGNAL(editingFinished()), this, SLOT(updateMemory()));
    connect(ui->VmemFileCB,         SIGNAL(stateChanged(int)), this, SLOT(updateVmemFile()));
    connect(ui->VmemFileLE,         SIGNAL(editingFinished()), this, SLOT(updateVmemFile()));
    connect(ui->VmemFileButton,     SIGNAL(clicked()),         this, SLOT(VmemFileButton_clicked()));
    connect(ui->GreetFileCB,        SIGNAL(stateChanged(int)), this, SLOT(updateGreetFile()));
    connect(ui->GreetFileLE,        SIGNAL(editingFinished()), this, SLOT(updateGreetFile()));
    connect(ui->GreetFileButton,    SIGNAL(clicked()),         this, SLOT(GreetFileButton_clicked()));
    connect(ui->LoginDirCB,         SIGNAL(stateChanged(int)), this, SLOT(updateLoginDir()));
    connect(ui->LoginDirLE,         SIGNAL(editingFinished()), this, SLOT(updateLoginDir()));
    connect(ui->LoginDirButton,     SIGNAL(clicked()),         this, SLOT(ResetButton_clicked()));
    connect(ui->ResetButton,        SIGNAL(clicked()),         this, SLOT(ResetButton_clicked()));
    connect(ui->SaveButton,         SIGNAL(clicked()),         this, SLOT(SaveButton_clicked()));
    connect(ui->RestoreButton,      SIGNAL(clicked()),         this, SLOT(RestoreButton_clicked()));

}

//
//   Reset the ui to its default (unconfigured) state
//

void MainWindow::resetUI() {

    Config* save_config = config;
    config = new Config();

    QList<QCheckBox *> CBs = findChildren<QCheckBox *>();
    for(auto cb : CBs) {
        cb->setChecked(false);
    }
    ui->ResumeCB->setChecked(true);
    ui->InterlispExecCB->setDisabled(true);

    QList<QRadioButton *> RBs = findChildren<QRadioButton *>();
    for(auto rb : RBs) {
        rb->setChecked(false);
    }
    ui->FullSysoutRB->setChecked(true);

    QList<QLineEdit *> LEs = findChildren<QLineEdit *>();
    for(auto le : LEs) {
        le->setText(QString());
        le->setDisabled(true);
    }
    ui->TitleLE->setText(UI_DEFAULT_TITLE);
    ui->DisplayLE->setText(UI_DEFAULT_DISPLAY);
    //ui->IdLE->setText(UI_DEFAULT_ID);

    QList<QSpinBox *> SBs = findChildren<QSpinBox *>();
    for(auto sb : SBs) {
        QList<QLineEdit *>LEs = sb->findChildren<QLineEdit *>();
        for(auto le : LEs) le->setDisabled(false);
        sb->setDisabled(true);
    }

    ui->GeoWidthSB->setValue(UI_DEFAULT_GEO_WIDTH);
    ui->GeoHeightSB->setValue(UI_DEFAULT_GEO_HEIGHT);
    ui->ScreenSizeWidthSB->setValue(UI_DEFAULT_SCREENSIZE_WIDTH);
    ui->ScreenSizeHeightSB->setValue(UI_DEFAULT_SCREENSIZE_HEIGHT);
    ui->MemorySB->setValue(UI_DEFAULT_MEMORY);

    QList<QPushButton *> PBs = findChildren<QPushButton *>();
    for(auto pb : PBs) {
        if(pb->text() == QStringLiteral("Browse"))
            pb->setDisabled(true);
    }

    delete config;
    config = save_config;
}

//
//   Configure the ui according to the current Config object
//
void MainWindow::configureUI() {

    if(config->interlisp_exec.has_value() && config->interlisp_exec.value()) {
        ui->InterlispExecCB->setChecked(true);
    }

    if(config->sysout.has_value()) {
        QString sysout = config->sysout.value();
        if ((sysout == QStringLiteral("~resume")) || (sysout == QStringLiteral("~default")))
            ui->ResumeCB->setChecked(true);
        else if (sysout == QStringLiteral("~apps")) {
            ui->StartFromSysoutCB->setChecked(true);
            ui->AppsSysoutRB->setChecked(true);
        }
        else if (sysout == QStringLiteral("~full")) {
            ui->StartFromSysoutCB->setChecked(true);
            ui->FullSysoutRB->setChecked(true);
        }
        else if (sysout == QStringLiteral("~lisp")) {
            ui->StartFromSysoutCB->setChecked(true);
            ui->LispSysoutRB->setChecked(true);
        }
        else {
            ui->SysoutPathLE->setText(config->sysout.value());
            ui->StartFromSysoutCB->setChecked(true);
            ui->CustomSysoutRB->setChecked(true);
        };
    }
    else {
        ui->ResumeCB->setChecked(true);
    }

    if(config->id.has_value()) {
        ui->IdLE->setText(config->id.value());
        ui->IdCB->setChecked(true);
    }

    if(config->noscroll.has_value() && config->noscroll.value()) {
        ui->NoScrollCB->setChecked(true);
    }

    {
        int width = UI_DEFAULT_GEO_WIDTH, height = UI_DEFAULT_GEO_HEIGHT;
        bool has_value = config->geometry.has_value();
        if(has_value && config->geometry.value().contains(Config::re_WxH)) {
            QStringList pair = config->geometry.value().split(Config::re_xOrX);
            width = pair.at(0).toInt();
            height = pair.at(1).toInt();
         }
        ui->GeoWidthSB->setValue(width);
        ui->GeoHeightSB->setValue(height);
        ui->GeoCB->setChecked(has_value);
    }


    {
        int width = UI_DEFAULT_SCREENSIZE_WIDTH, height = UI_DEFAULT_SCREENSIZE_HEIGHT;
        bool has_value = config->screensize.has_value();
        if(has_value && config->screensize.value().contains(Config::re_WxH)) {
            QStringList pair = config->screensize.value().split(Config::re_xOrX);
            width = pair.at(0).toInt();
            height = pair.at(1).toInt();
        }
        ui->ScreenSizeWidthSB->setValue(width);
        ui->ScreenSizeHeightSB->setValue(height);
        ui->ScreenSizeCB->setChecked(has_value);
    }

    if(config->title.has_value()) {
        ui->TitleLE->setText(config->title.value());
        ui->TitleCB->setChecked(true);
    }
    else ui->TitleLE->setText(UI_DEFAULT_TITLE);

    if(config->display.has_value()) {
        ui->DisplayLE->setText(config->display.value());
        ui->DisplayCB->setChecked(true);
    }
    else ui->DisplayLE->setText(UI_DEFAULT_DISPLAY);

    if(config->mem.has_value()) {
        ui->MemorySB->setValue(config->mem.value());
        ui->MemoryCB->setChecked(true);
    }
    else ui->MemorySB->setValue(UI_DEFAULT_MEMORY);

    if(config->vmem.has_value()) {
        ui->VmemFileLE->setText(config->vmem.value());
        ui->VmemFileCB->setChecked(true);
    }

    if(config->greet.has_value()) {
        ui->GreetFileLE->setText(config->greet.value());
        ui->GreetFileCB->setChecked(true);
    }

    if(config->logindir.has_value()) {
        ui->LoginDirLE->setText(config->logindir.value());
        ui->LoginDirCB->setChecked(true);
    }
    else ui->LoginDirLE->setText(app->defaultLoginDir.absolutePath());
}


//
//  Slots (methods) to handle all of the signals comong from the various
//  MainWindow UI Widgets. These slots handle the various constraints in settings
//  between the various widgets.
//

void MainWindow::ResumeCB_stateChanged(int arg1)
{
    bool is_checked = ui->ResumeCB->isChecked();
    ui->StartFromSysoutCB->setChecked(!is_checked);
    ui->AppsSysoutRB->setDisabled(is_checked);
    if (ui->AppsSysoutRB->isChecked()) {
        ui->InterlispExecCB->setDisabled(is_checked);
    }
    ui->FullSysoutRB->setDisabled(is_checked);
    ui->LispSysoutRB->setDisabled(is_checked);
    ui->CustomSysoutRB->setDisabled(is_checked);
    if (ui->CustomSysoutRB->isChecked()) {
        ui->SysoutPathLE->setDisabled(is_checked);
    }
    ui->CustomSysoutButton->setDisabled(is_checked);
    if(is_checked) config->sysout = QStringLiteral("~resume");
}

void MainWindow::StartFromSysoutCB_stateChanged(int arg1)
{
    bool is_checked = ui->StartFromSysoutCB->isChecked();
    ui->ResumeCB->setChecked(!is_checked);
    if(is_checked) updateSysout();
}

void MainWindow::AppsSysoutRB_toggled(bool checked)
{
    ui->InterlispExecCB->setDisabled(!checked);
    updateSysout();
}

void MainWindow::InterlispExecCB_stateChanged(int arg1) {
    bool is_checked = ui->InterlispExecCB->isChecked();
    config->interlisp_exec = is_checked;
};

void MainWindow::updateSysout()
{
    if(ui->AppsSysoutRB->isChecked()) {
        config->sysout = QStringLiteral("~apps");
        if(ui->InterlispExecCB->isChecked())
            config->interlisp_exec = true;
        else config->interlisp_exec.reset();
    }

    if(ui->FullSysoutRB->isChecked())
        config->sysout = QStringLiteral("~full");

    if(ui->LispSysoutRB->isChecked())
        config->sysout = QStringLiteral("~lisp");

    if(ui->CustomSysoutRB->isChecked()) {
        QString t = ui->SysoutPathLE->text();
        if(!t.isNull() && !t.isEmpty())
            config->sysout = ui->SysoutPathLE->text();
    }
}

void MainWindow::CustomSysoutRB_toggled(bool checked)
{
    ui->SysoutPathLE->setDisabled(!checked);
    ui->CustomSysoutButton->setDisabled(!checked);
    updateSysout();
}

void MainWindow::CustomSysoutButton_clicked()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Select Sysout File"), "/home", tr("*.sysout"));
    if(!fileName.isEmpty()) ui->SysoutPathLE->setText(fileName);
    updateSysout();
}

void MainWindow::updateId()
{
    updateConfig(ui->IdCB, ui->IdLE, config->id);
}

void MainWindow::NoScrollCB_stateChanged(int arg1)
{
    if(ui->NoScrollCB->isChecked())
        config->noscroll = true;
    else
        config->noscroll.reset();
    updateGeometry();
    updateScreenSize();
}

void MainWindow::GeoCB_stateChanged(int arg1)
{
    bool is_checked = ui->GeoCB->isChecked();
    ui->GeoWidthSB->setDisabled(!is_checked);
    ui->GeoHeightSB->setDisabled(!is_checked);
    ui->GeoHeightLabel->setDisabled(!is_checked);
    ui->GeoWidthLabel->setDisabled(!is_checked);
    updateGeometry();
}

void MainWindow::updateGeometry()
{
    if(ui->GeoCB->isChecked())
        config->geometry =
            QString::number(ui->GeoWidthSB->value())
            + QStringLiteral("x")
            + QString::number(ui->GeoHeightSB->value())
            ;
    else config->geometry.reset();
}

void MainWindow::updateWidthandHeightSBs()
{
    Config::Geometries geometries = config->figureOutGeometries();
    ui->GeoWidthSB->setValue(geometries.gw);
    ui->GeoHeightSB->setValue(geometries.gh);
    ui->ScreenSizeWidthSB->setValue(geometries.sw);
    ui->ScreenSizeHeightSB->setValue(geometries.sh);
}

void MainWindow::ScreenSizeCB_stateChanged(int arg1)
{
    bool is_checked = ui->ScreenSizeCB->isChecked();
    ui->ScreenSizeWidthSB->setDisabled(!is_checked);
    ui->ScreenSizeHeightSB->setDisabled(!is_checked);
    ui->ScreenSizeHeightLabel->setDisabled(!is_checked);
    ui->ScreenSizeWidthLabel->setDisabled(!is_checked);
    updateScreenSize();
}

void MainWindow::updateScreenSize()
{
    if(ui->ScreenSizeCB->isChecked())
        config->screensize =
            QString::number(ui->ScreenSizeWidthSB->value())
            + QStringLiteral("x")
            + QString::number(ui->ScreenSizeHeightSB->value())
            ;
    else config->screensize.reset();
}

void MainWindow::updateTitle()
{
    updateConfig(ui->TitleCB, ui->TitleLE, config->title);
}

void MainWindow::updateDisplay()
{
    updateConfig(ui->DisplayCB, ui->DisplayLE, config->display);
}

void MainWindow::updateMemory()
{
    updateConfig(ui->MemoryCB, ui->MemorySB, config->mem);
}

void MainWindow::updateVmemFile()
{
    updateConfig(ui->VmemFileCB, ui->VmemFileLE, ui->VmemFileButton, config->vmem);
}

void MainWindow::VmemFileButton_clicked()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Select Vmem File"), "/home", tr("*.vmem"));
    if(!fileName.isEmpty()) {
        ui->VmemFileLE->setText(fileName);
        updateConfig(ui->VmemFileCB, ui->VmemFileLE, ui->VmemFileButton, config->vmem);
    }
}

void MainWindow::updateGreetFile()
{
    updateConfig(ui->GreetFileCB, ui->GreetFileLE, ui->GreetFileButton, config->greet);
}

void MainWindow::GreetFileButton_clicked()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Select Greet (INIT) File"), "/home", tr("*"));
    if(!fileName.isEmpty()) {
        ui->GreetFileLE->setText(fileName);
        updateConfig(ui->GreetFileCB, ui->GreetFileLE, ui->GreetFileButton, config->greet);
    }
}

void MainWindow::updateLoginDir()
{
    updateConfig(ui->LoginDirCB, ui->LoginDirLE, ui->LoginDirButton, config->logindir);
}

void MainWindow::LoginDirButton_clicked()
{
    QString dir =
        QFileDialog::getExistingDirectory(
            this,
            tr("Select directory to serve as LOGINDIR"),
            "/home",
            QFileDialog::ShowDirsOnly
            | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty()) {
        ui->LoginDirLE->setText(dir);
        updateConfig(ui->LoginDirCB, ui->LoginDirLE, ui->LoginDirButton, config->logindir);
    }
}

void MainWindow::SaveButton_clicked()
{
    ConfigFile().writeConfig(config);
}


void MainWindow::ResetButton_clicked()
{
    Config *old_config = config;
    config = new Config();
    resetUI();
    delete old_config;
}

void MainWindow::RestoreButton_clicked()
{
    QStringList *argList = ConfigFile().readConfig();
    delete config;
    config = new Config(argList);
    delete argList;
    resetUI();
    configureUI();
}


//
//  Helper methods for slots
//
void MainWindow::updateConfig(QCheckBox *CB, QLineEdit *LE, std::optional<QString> &configField)
{
    updateConfig(CB, LE, nullptr, configField);
}

void MainWindow::updateConfig(QCheckBox *CB, QLineEdit *LE, QPushButton *BB, std::optional<QString> &configField)
{
    bool is_checked = CB->isChecked();
    LE->setDisabled(!is_checked);
    if(BB != nullptr) BB->setDisabled(!is_checked);
    QString t = LE->text();
    if(is_checked && !t.isNull() && !t.isEmpty())
        configField = t;
    else configField.reset();
}

void MainWindow::updateConfig(QCheckBox *CB, QSpinBox *SB, std::optional<int> &configField)
{
    bool is_checked = CB->isChecked();
    SB->setDisabled(!is_checked);
    int v = SB->value();
    if(is_checked)
        configField = v;
    else configField.reset();
}





