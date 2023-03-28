/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionTest1;
    QAction *actionTest2;
    QAction *actionTest3;
    QWidget *centralwidget;
    QGroupBox *StartFromGB;
    QCheckBox *ResumeCB;
    QCheckBox *StartFromSysoutCB;
    QWidget *layoutWidget;
    QVBoxLayout *VerticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *AppsSysoutRB;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *InterlispExecCB;
    QRadioButton *FullSysoutRB;
    QRadioButton *LispSysoutRB;
    QRadioButton *CustomSysoutRB;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *CustomSysoutButton;
    QLineEdit *SysoutPathLE;
    QCheckBox *IdCB;
    QLineEdit *IdLE;
    QCheckBox *NoScrollCB;
    QCheckBox *GeoCB;
    QSpinBox *GeoWidthSB;
    QLabel *GeoWidthLabel;
    QLabel *GeoHeightLabel;
    QSpinBox *GeoHeightSB;
    QLabel *ScreenSizeHeightLabel;
    QCheckBox *ScreenSizeCB;
    QSpinBox *ScreenSizeHeightSB;
    QLabel *ScreenSizeWidthLabel;
    QSpinBox *ScreenSizeWidthSB;
    QCheckBox *TitleCB;
    QLineEdit *TitleLE;
    QCheckBox *DisplayCB;
    QLineEdit *DisplayLE;
    QCheckBox *MemoryCB;
    QSpinBox *MemorySB;
    QLineEdit *VmemFileLE;
    QPushButton *VmemFileButton;
    QLineEdit *GreetFileLE;
    QPushButton *GreetFileButton;
    QLineEdit *LoginDirLE;
    QPushButton *LoginDirButton;
    QCheckBox *VmemFileCB;
    QCheckBox *GreetFileCB;
    QCheckBox *LoginDirCB;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *ResetButton;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *RestoreButton;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *SaveButton;
    QSpacerItem *horizontalSpacer_2;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *RunButton1;
    QSpacerItem *horizontalSpacer_6;
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_7;
    QLabel *MainLabel;
    QSpacerItem *horizontalSpacer_8;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 768);
        actionTest1 = new QAction(MainWindow);
        actionTest1->setObjectName("actionTest1");
        actionTest1->setCheckable(true);
        actionTest2 = new QAction(MainWindow);
        actionTest2->setObjectName("actionTest2");
        actionTest2->setCheckable(true);
        actionTest3 = new QAction(MainWindow);
        actionTest3->setObjectName("actionTest3");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        StartFromGB = new QGroupBox(centralwidget);
        StartFromGB->setObjectName("StartFromGB");
        StartFromGB->setGeometry(QRect(60, 117, 681, 231));
        ResumeCB = new QCheckBox(StartFromGB);
        ResumeCB->setObjectName("ResumeCB");
        ResumeCB->setGeometry(QRect(60, 30, 191, 24));
        ResumeCB->setChecked(false);
        StartFromSysoutCB = new QCheckBox(StartFromGB);
        StartFromSysoutCB->setObjectName("StartFromSysoutCB");
        StartFromSysoutCB->setGeometry(QRect(61, 62, 140, 24));
        StartFromSysoutCB->setChecked(true);
        layoutWidget = new QWidget(StartFromGB);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(80, 87, 318, 115));
        VerticalLayout = new QVBoxLayout(layoutWidget);
        VerticalLayout->setObjectName("VerticalLayout");
        VerticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        VerticalLayout->setContentsMargins(0, 0, 0, 11);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        AppsSysoutRB = new QRadioButton(layoutWidget);
        AppsSysoutRB->setObjectName("AppsSysoutRB");
        AppsSysoutRB->setMaximumSize(QSize(16777215, 20));
        AppsSysoutRB->setChecked(false);

        horizontalLayout_2->addWidget(AppsSysoutRB);

        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        InterlispExecCB = new QCheckBox(layoutWidget);
        InterlispExecCB->setObjectName("InterlispExecCB");
        InterlispExecCB->setEnabled(false);
        InterlispExecCB->setMaximumSize(QSize(16777215, 20));

        horizontalLayout_2->addWidget(InterlispExecCB);


        VerticalLayout->addLayout(horizontalLayout_2);

        FullSysoutRB = new QRadioButton(layoutWidget);
        FullSysoutRB->setObjectName("FullSysoutRB");
        FullSysoutRB->setMaximumSize(QSize(16777215, 20));
        FullSysoutRB->setChecked(true);

        VerticalLayout->addWidget(FullSysoutRB);

        LispSysoutRB = new QRadioButton(layoutWidget);
        LispSysoutRB->setObjectName("LispSysoutRB");
        LispSysoutRB->setMaximumSize(QSize(16777215, 20));

        VerticalLayout->addWidget(LispSysoutRB);

        CustomSysoutRB = new QRadioButton(layoutWidget);
        CustomSysoutRB->setObjectName("CustomSysoutRB");
        CustomSysoutRB->setMaximumSize(QSize(16777215, 20));

        VerticalLayout->addWidget(CustomSysoutRB);

        layoutWidget1 = new QWidget(StartFromGB);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(120, 197, 551, 31));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        CustomSysoutButton = new QPushButton(layoutWidget1);
        CustomSysoutButton->setObjectName("CustomSysoutButton");
        CustomSysoutButton->setEnabled(false);

        horizontalLayout_5->addWidget(CustomSysoutButton);

        SysoutPathLE = new QLineEdit(layoutWidget1);
        SysoutPathLE->setObjectName("SysoutPathLE");
        SysoutPathLE->setEnabled(false);
        SysoutPathLE->setReadOnly(true);

        horizontalLayout_5->addWidget(SysoutPathLE);

        IdCB = new QCheckBox(centralwidget);
        IdCB->setObjectName("IdCB");
        IdCB->setGeometry(QRect(80, 360, 51, 24));
        IdLE = new QLineEdit(centralwidget);
        IdLE->setObjectName("IdLE");
        IdLE->setEnabled(false);
        IdLE->setGeometry(QRect(140, 360, 161, 25));
        NoScrollCB = new QCheckBox(centralwidget);
        NoScrollCB->setObjectName("NoScrollCB");
        NoScrollCB->setGeometry(QRect(80, 390, 141, 24));
        GeoCB = new QCheckBox(centralwidget);
        GeoCB->setObjectName("GeoCB");
        GeoCB->setGeometry(QRect(80, 420, 121, 24));
        GeoWidthSB = new QSpinBox(centralwidget);
        GeoWidthSB->setObjectName("GeoWidthSB");
        GeoWidthSB->setEnabled(false);
        GeoWidthSB->setGeometry(QRect(290, 420, 71, 25));
        GeoWidthSB->setAlignment(Qt::AlignCenter);
        GeoWidthSB->setReadOnly(false);
        GeoWidthSB->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        GeoWidthSB->setAccelerated(true);
        GeoWidthSB->setSuffix(QString::fromUtf8(""));
        GeoWidthSB->setMaximum(9999);
        GeoWidthSB->setValue(1440);
        GeoWidthLabel = new QLabel(centralwidget);
        GeoWidthLabel->setObjectName("GeoWidthLabel");
        GeoWidthLabel->setEnabled(false);
        GeoWidthLabel->setGeometry(QRect(240, 420, 51, 20));
        GeoHeightLabel = new QLabel(centralwidget);
        GeoHeightLabel->setObjectName("GeoHeightLabel");
        GeoHeightLabel->setEnabled(false);
        GeoHeightLabel->setGeometry(QRect(400, 420, 51, 20));
        GeoHeightSB = new QSpinBox(centralwidget);
        GeoHeightSB->setObjectName("GeoHeightSB");
        GeoHeightSB->setEnabled(false);
        GeoHeightSB->setGeometry(QRect(460, 420, 71, 25));
        GeoHeightSB->setAlignment(Qt::AlignCenter);
        GeoHeightSB->setMaximum(9999);
        GeoHeightSB->setValue(900);
        ScreenSizeHeightLabel = new QLabel(centralwidget);
        ScreenSizeHeightLabel->setObjectName("ScreenSizeHeightLabel");
        ScreenSizeHeightLabel->setEnabled(false);
        ScreenSizeHeightLabel->setGeometry(QRect(400, 450, 51, 20));
        ScreenSizeCB = new QCheckBox(centralwidget);
        ScreenSizeCB->setObjectName("ScreenSizeCB");
        ScreenSizeCB->setGeometry(QRect(80, 450, 151, 24));
        ScreenSizeHeightSB = new QSpinBox(centralwidget);
        ScreenSizeHeightSB->setObjectName("ScreenSizeHeightSB");
        ScreenSizeHeightSB->setEnabled(false);
        ScreenSizeHeightSB->setGeometry(QRect(460, 450, 71, 25));
        ScreenSizeHeightSB->setAlignment(Qt::AlignCenter);
        ScreenSizeHeightSB->setMinimum(1);
        ScreenSizeHeightSB->setMaximum(9999);
        ScreenSizeHeightSB->setValue(900);
        ScreenSizeWidthLabel = new QLabel(centralwidget);
        ScreenSizeWidthLabel->setObjectName("ScreenSizeWidthLabel");
        ScreenSizeWidthLabel->setEnabled(false);
        ScreenSizeWidthLabel->setGeometry(QRect(240, 450, 51, 20));
        ScreenSizeWidthSB = new QSpinBox(centralwidget);
        ScreenSizeWidthSB->setObjectName("ScreenSizeWidthSB");
        ScreenSizeWidthSB->setEnabled(false);
        ScreenSizeWidthSB->setGeometry(QRect(290, 450, 71, 25));
        ScreenSizeWidthSB->setAlignment(Qt::AlignCenter);
        ScreenSizeWidthSB->setMinimum(32);
        ScreenSizeWidthSB->setMaximum(9999);
        ScreenSizeWidthSB->setSingleStep(32);
        ScreenSizeWidthSB->setValue(1440);
        TitleCB = new QCheckBox(centralwidget);
        TitleCB->setObjectName("TitleCB");
        TitleCB->setGeometry(QRect(80, 480, 121, 24));
        TitleLE = new QLineEdit(centralwidget);
        TitleLE->setObjectName("TitleLE");
        TitleLE->setEnabled(false);
        TitleLE->setGeometry(QRect(200, 480, 401, 25));
        DisplayCB = new QCheckBox(centralwidget);
        DisplayCB->setObjectName("DisplayCB");
        DisplayCB->setGeometry(QRect(80, 510, 81, 24));
        DisplayLE = new QLineEdit(centralwidget);
        DisplayLE->setObjectName("DisplayLE");
        DisplayLE->setEnabled(false);
        DisplayLE->setGeometry(QRect(190, 510, 161, 25));
        MemoryCB = new QCheckBox(centralwidget);
        MemoryCB->setObjectName("MemoryCB");
        MemoryCB->setGeometry(QRect(80, 540, 171, 24));
        MemorySB = new QSpinBox(centralwidget);
        MemorySB->setObjectName("MemorySB");
        MemorySB->setEnabled(false);
        MemorySB->setGeometry(QRect(250, 540, 91, 25));
        MemorySB->setAlignment(Qt::AlignCenter);
        MemorySB->setMinimum(32);
        MemorySB->setMaximum(256);
        MemorySB->setSingleStep(16);
        MemorySB->setValue(256);
        VmemFileLE = new QLineEdit(centralwidget);
        VmemFileLE->setObjectName("VmemFileLE");
        VmemFileLE->setEnabled(false);
        VmemFileLE->setGeometry(QRect(280, 570, 451, 25));
        VmemFileLE->setReadOnly(true);
        VmemFileButton = new QPushButton(centralwidget);
        VmemFileButton->setObjectName("VmemFileButton");
        VmemFileButton->setEnabled(false);
        VmemFileButton->setGeometry(QRect(180, 570, 93, 29));
        GreetFileLE = new QLineEdit(centralwidget);
        GreetFileLE->setObjectName("GreetFileLE");
        GreetFileLE->setEnabled(false);
        GreetFileLE->setGeometry(QRect(280, 610, 451, 25));
        GreetFileLE->setReadOnly(true);
        GreetFileButton = new QPushButton(centralwidget);
        GreetFileButton->setObjectName("GreetFileButton");
        GreetFileButton->setEnabled(false);
        GreetFileButton->setGeometry(QRect(180, 610, 93, 29));
        LoginDirLE = new QLineEdit(centralwidget);
        LoginDirLE->setObjectName("LoginDirLE");
        LoginDirLE->setEnabled(false);
        LoginDirLE->setGeometry(QRect(280, 650, 451, 25));
        LoginDirLE->setReadOnly(true);
        LoginDirButton = new QPushButton(centralwidget);
        LoginDirButton->setObjectName("LoginDirButton");
        LoginDirButton->setEnabled(false);
        LoginDirButton->setGeometry(QRect(180, 650, 93, 29));
        VmemFileCB = new QCheckBox(centralwidget);
        VmemFileCB->setObjectName("VmemFileCB");
        VmemFileCB->setGeometry(QRect(80, 570, 101, 24));
        GreetFileCB = new QCheckBox(centralwidget);
        GreetFileCB->setObjectName("GreetFileCB");
        GreetFileCB->setGeometry(QRect(80, 610, 81, 24));
        LoginDirCB = new QCheckBox(centralwidget);
        LoginDirCB->setObjectName("LoginDirCB");
        LoginDirCB->setGeometry(QRect(80, 650, 88, 24));
        layoutWidget2 = new QWidget(centralwidget);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(0, 710, 801, 31));
        horizontalLayout = new QHBoxLayout(layoutWidget2);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ResetButton = new QPushButton(layoutWidget2);
        ResetButton->setObjectName("ResetButton");

        horizontalLayout->addWidget(ResetButton);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_9);

        RestoreButton = new QPushButton(layoutWidget2);
        RestoreButton->setObjectName("RestoreButton");

        horizontalLayout->addWidget(RestoreButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        SaveButton = new QPushButton(layoutWidget2);
        SaveButton->setObjectName("SaveButton");

        horizontalLayout->addWidget(SaveButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        layoutWidget3 = new QWidget(centralwidget);
        layoutWidget3->setObjectName("layoutWidget3");
        layoutWidget3->setGeometry(QRect(0, 70, 801, 38));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        RunButton1 = new QPushButton(layoutWidget3);
        RunButton1->setObjectName("RunButton1");
        QFont font;
        font.setPointSize(12);
        RunButton1->setFont(font);

        horizontalLayout_3->addWidget(RunButton1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        layoutWidget4 = new QWidget(centralwidget);
        layoutWidget4->setObjectName("layoutWidget4");
        layoutWidget4->setGeometry(QRect(0, 20, 801, 38));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        MainLabel = new QLabel(layoutWidget4);
        MainLabel->setObjectName("MainLabel");
        QFont font1;
        font1.setPointSize(16);
        MainLabel->setFont(font1);
        MainLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(MainLabel);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Medley Interlisp", nullptr));
        actionTest1->setText(QCoreApplication::translate("MainWindow", "Test1", nullptr));
        actionTest2->setText(QCoreApplication::translate("MainWindow", "Test2", nullptr));
        actionTest3->setText(QCoreApplication::translate("MainWindow", "Test3", nullptr));
        StartFromGB->setTitle(QCoreApplication::translate("MainWindow", "Start From", nullptr));
        ResumeCB->setText(QCoreApplication::translate("MainWindow", "Resume previous session", nullptr));
        StartFromSysoutCB->setText(QCoreApplication::translate("MainWindow", "Start From Sysout", nullptr));
        AppsSysoutRB->setText(QCoreApplication::translate("MainWindow", "Apps.sysout", nullptr));
        InterlispExecCB->setText(QCoreApplication::translate("MainWindow", "Start with Interlisp Exec", nullptr));
        FullSysoutRB->setText(QCoreApplication::translate("MainWindow", "Full.sysout", nullptr));
        LispSysoutRB->setText(QCoreApplication::translate("MainWindow", "Lisp.sysout", nullptr));
        CustomSysoutRB->setText(QCoreApplication::translate("MainWindow", "Custom Sysout", nullptr));
        CustomSysoutButton->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        IdCB->setText(QCoreApplication::translate("MainWindow", "ID:", nullptr));
        IdLE->setText(QCoreApplication::translate("MainWindow", "default", nullptr));
        NoScrollCB->setText(QCoreApplication::translate("MainWindow", "No scroll bars", nullptr));
        GeoCB->setText(QCoreApplication::translate("MainWindow", " Window size:", nullptr));
        GeoWidthLabel->setText(QCoreApplication::translate("MainWindow", "Width:", nullptr));
        GeoHeightLabel->setText(QCoreApplication::translate("MainWindow", "Height:", nullptr));
        ScreenSizeHeightLabel->setText(QCoreApplication::translate("MainWindow", "Height:", nullptr));
        ScreenSizeCB->setText(QCoreApplication::translate("MainWindow", "Medley Screensize", nullptr));
        ScreenSizeWidthLabel->setText(QCoreApplication::translate("MainWindow", "Width:", nullptr));
        TitleCB->setText(QCoreApplication::translate("MainWindow", "Window Title", nullptr));
        TitleLE->setText(QCoreApplication::translate("MainWindow", "Medley Interlisp", nullptr));
        DisplayCB->setText(QCoreApplication::translate("MainWindow", "Display", nullptr));
        DisplayLE->setText(QCoreApplication::translate("MainWindow", ":0", nullptr));
        MemoryCB->setText(QCoreApplication::translate("MainWindow", "Virtual Memory Size: ", nullptr));
        MemorySB->setSuffix(QCoreApplication::translate("MainWindow", "M", nullptr));
        VmemFileButton->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        GreetFileButton->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        LoginDirButton->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        VmemFileCB->setText(QCoreApplication::translate("MainWindow", "VMEM File", nullptr));
        GreetFileCB->setText(QCoreApplication::translate("MainWindow", "INIT File", nullptr));
        LoginDirCB->setText(QCoreApplication::translate("MainWindow", "LOGINDIR", nullptr));
        ResetButton->setText(QCoreApplication::translate("MainWindow", "Reset Configuration", nullptr));
        RestoreButton->setText(QCoreApplication::translate("MainWindow", "Restore Configuration", nullptr));
        SaveButton->setText(QCoreApplication::translate("MainWindow", "Save Configuration", nullptr));
        RunButton1->setText(QCoreApplication::translate("MainWindow", "Run Medley", nullptr));
        MainLabel->setText(QCoreApplication::translate("MainWindow", "Medley Interlisp", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
