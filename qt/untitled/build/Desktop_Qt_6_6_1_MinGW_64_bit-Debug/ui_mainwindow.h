/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "analogwidgets/led.h"
#include "analogwidgets/manometer.h"
#include "qwt_knob.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    Led *ledA;
    QPushButton *m_boton;
    ManoMeter *manometer;
    QwtKnob *m_knoba;
    QwtKnob *m_knob_b;
    QCheckBox *m_check;
    QPushButton *pushButton;
    Led *ledB;
    QCheckBox *checkBox1;
    QCheckBox *checkBox2;
    QCheckBox *checkBox3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        ledA = new Led(centralwidget);
        ledA->setObjectName("ledA");
        ledA->setGeometry(QRect(480, 70, 16, 16));
        m_boton = new QPushButton(centralwidget);
        m_boton->setObjectName("m_boton");
        m_boton->setGeometry(QRect(90, 70, 75, 24));
        manometer = new ManoMeter(centralwidget);
        manometer->setObjectName("manometer");
        manometer->setGeometry(QRect(630, 260, 100, 100));
        m_knoba = new QwtKnob(centralwidget);
        m_knoba->setObjectName("m_knoba");
        m_knoba->setGeometry(QRect(80, 330, 150, 150));
        m_knob_b = new QwtKnob(centralwidget);
        m_knob_b->setObjectName("m_knob_b");
        m_knob_b->setGeometry(QRect(330, 340, 150, 150));
        m_check = new QCheckBox(centralwidget);
        m_check->setObjectName("m_check");
        m_check->setGeometry(QRect(640, 460, 76, 20));
        m_check->setChecked(true);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(680, 130, 75, 24));
        ledB = new Led(centralwidget);
        ledB->setObjectName("ledB");
        ledB->setGeometry(QRect(480, 130, 16, 16));
        ledB->setColor(QColor(0, 85, 255));
        checkBox1 = new QCheckBox(centralwidget);
        checkBox1->setObjectName("checkBox1");
        checkBox1->setGeometry(QRect(80, 130, 91, 24));
        checkBox2 = new QCheckBox(centralwidget);
        checkBox2->setObjectName("checkBox2");
        checkBox2->setGeometry(QRect(80, 160, 91, 24));
        checkBox3 = new QCheckBox(centralwidget);
        checkBox3->setObjectName("checkBox3");
        checkBox3->setGeometry(QRect(80, 190, 91, 24));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton, &QPushButton::clicked, MainWindow, qOverload<>(&QMainWindow::close));
        QObject::connect(m_check, SIGNAL(clicked(bool)), manometer, SLOT(setVisible(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
#if QT_CONFIG(tooltip)
        ledA->setToolTip(QCoreApplication::translate("MainWindow", "Color Led component", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        ledA->setWhatsThis(QCoreApplication::translate("MainWindow", "Led indicator", nullptr));
#endif // QT_CONFIG(whatsthis)
        m_boton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
#if QT_CONFIG(tooltip)
        manometer->setToolTip(QCoreApplication::translate("MainWindow", "Shows the pressure", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        manometer->setWhatsThis(QCoreApplication::translate("MainWindow", "The bar meter widget displays the pressure attached to it", nullptr));
#endif // QT_CONFIG(whatsthis)
        m_check->setText(QCoreApplication::translate("MainWindow", "visible", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "exit", nullptr));
#if QT_CONFIG(tooltip)
        ledB->setToolTip(QCoreApplication::translate("MainWindow", "Color Led component", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        ledB->setWhatsThis(QCoreApplication::translate("MainWindow", "Led indicator", nullptr));
#endif // QT_CONFIG(whatsthis)
        checkBox1->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
        checkBox2->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
        checkBox3->setText(QCoreApplication::translate("MainWindow", "CheckBox", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
