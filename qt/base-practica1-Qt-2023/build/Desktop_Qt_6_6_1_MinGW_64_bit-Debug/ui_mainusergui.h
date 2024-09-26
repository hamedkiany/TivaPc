/********************************************************************************
** Form generated from reading UI file 'mainusergui.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINUSERGUI_H
#define UI_MAINUSERGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "LED.h"
#include "analogwidgets/led.h"
#include "analogwidgets/thermometer.h"
#include "color_wheel.hpp"
#include "qwt_knob.h"
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_MainUserGUI
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QSplitter *splitter_2;
    QCheckBox *rojo;
    QCheckBox *verde;
    QCheckBox *azul;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QwtKnob *Knob;
    QLabel *label_2;
    color_widgets::ColorWheel *colorWheel;
    QCheckBox *Mode_PWM_check;
    Led *led1;
    Led *led4;
    QCheckBox *LeeSW_check;
    QWidget *tab_2;
    QPushButton *ADCButton;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QLCDNumber *lcdCh1;
    QLCDNumber *lcdCh3;
    QLCDNumber *lcdCh2;
    QLCDNumber *lcdCh4;
    ThermoMeter *thermometer;
    QWidget *tab_3;
    LED *led8;
    QwtPlot *qwtPlot;
    QwtKnob *frecuencia;
    QWidget *tab_4;
    QCheckBox *ACMGPIO4;
    QCheckBox *ACMGPIO5;
    QCheckBox *ACMGPIO6;
    QCheckBox *ACMGPIO7;
    Led *led_GPIO0;
    Led *led_GPIO1;
    Led *led_GPIO2;
    Led *led_GPIO3;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QWidget *tab_5;
    QwtKnob *frecuencia_sample;
    QwtPlot *qwtPlot_acc;
    QwtPlot *qwtPlot_velo;
    QSplitter *splitter_4;
    QSplitter *splitter;
    QLabel *serialPortLabel;
    QComboBox *serialPortComboBox;
    QSplitter *splitter_3;
    QPushButton *runButton;
    QPushButton *pingButton;
    QSplitter *splitter_5;
    QPushButton *pushButton;
    QLabel *statusLabel;

    void setupUi(QWidget *MainUserGUI)
    {
        if (MainUserGUI->objectName().isEmpty())
            MainUserGUI->setObjectName("MainUserGUI");
        MainUserGUI->resize(936, 847);
        tabWidget = new QTabWidget(MainUserGUI);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, 40, 941, 761));
        tab = new QWidget();
        tab->setObjectName("tab");
        splitter_2 = new QSplitter(tab);
        splitter_2->setObjectName("splitter_2");
        splitter_2->setGeometry(QRect(60, 80, 97, 54));
        splitter_2->setOrientation(Qt::Vertical);
        rojo = new QCheckBox(splitter_2);
        rojo->setObjectName("rojo");
        splitter_2->addWidget(rojo);
        verde = new QCheckBox(splitter_2);
        verde->setObjectName("verde");
        splitter_2->addWidget(verde);
        azul = new QCheckBox(splitter_2);
        azul->setObjectName("azul");
        splitter_2->addWidget(azul);
        layoutWidget = new QWidget(tab);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(360, 40, 171, 243));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        Knob = new QwtKnob(layoutWidget);
        Knob->setObjectName("Knob");
        Knob->setUpperBound(1.000000000000000);
        Knob->setValue(0.500000000000000);
        Knob->setSingleSteps(0u);
        Knob->setPageSteps(10u);

        gridLayout->addWidget(Knob, 1, 1, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        colorWheel = new color_widgets::ColorWheel(layoutWidget);
        colorWheel->setObjectName("colorWheel");
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(colorWheel->sizePolicy().hasHeightForWidth());
        colorWheel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(colorWheel, 0, 1, 1, 1);

        Mode_PWM_check = new QCheckBox(tab);
        Mode_PWM_check->setObjectName("Mode_PWM_check");
        Mode_PWM_check->setGeometry(QRect(60, 170, 111, 24));
        led1 = new Led(tab);
        led1->setObjectName("led1");
        led1->setGeometry(QRect(610, 70, 16, 16));
        led1->setChecked(true);
        led4 = new Led(tab);
        led4->setObjectName("led4");
        led4->setGeometry(QRect(610, 120, 16, 16));
        led4->setChecked(true);
        led4->setColor(QColor(1, 14, 255));
        LeeSW_check = new QCheckBox(tab);
        LeeSW_check->setObjectName("LeeSW_check");
        LeeSW_check->setGeometry(QRect(60, 210, 91, 24));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        ADCButton = new QPushButton(tab_2);
        ADCButton->setObjectName("ADCButton");
        ADCButton->setGeometry(QRect(260, 250, 75, 23));
        layoutWidget1 = new QWidget(tab_2);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(160, 30, 271, 131));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        lcdCh1 = new QLCDNumber(layoutWidget1);
        lcdCh1->setObjectName("lcdCh1");
        lcdCh1->setFrameShape(QFrame::Box);
        lcdCh1->setFrameShadow(QFrame::Plain);
        lcdCh1->setSmallDecimalPoint(false);
        lcdCh1->setDigitCount(5);
        lcdCh1->setSegmentStyle(QLCDNumber::Flat);
        lcdCh1->setProperty("value", QVariant(0.000000000000000));

        gridLayout_2->addWidget(lcdCh1, 0, 0, 1, 1);

        lcdCh3 = new QLCDNumber(layoutWidget1);
        lcdCh3->setObjectName("lcdCh3");
        lcdCh3->setFrameShape(QFrame::Box);
        lcdCh3->setFrameShadow(QFrame::Plain);
        lcdCh3->setSmallDecimalPoint(false);
        lcdCh3->setDigitCount(5);
        lcdCh3->setSegmentStyle(QLCDNumber::Flat);
        lcdCh3->setProperty("value", QVariant(0.000000000000000));

        gridLayout_2->addWidget(lcdCh3, 0, 1, 1, 1);

        lcdCh2 = new QLCDNumber(layoutWidget1);
        lcdCh2->setObjectName("lcdCh2");
        lcdCh2->setFrameShape(QFrame::Box);
        lcdCh2->setFrameShadow(QFrame::Plain);
        lcdCh2->setSmallDecimalPoint(false);
        lcdCh2->setDigitCount(5);
        lcdCh2->setSegmentStyle(QLCDNumber::Flat);
        lcdCh2->setProperty("value", QVariant(0.000000000000000));

        gridLayout_2->addWidget(lcdCh2, 1, 0, 1, 1);

        lcdCh4 = new QLCDNumber(layoutWidget1);
        lcdCh4->setObjectName("lcdCh4");
        lcdCh4->setFrameShape(QFrame::Box);
        lcdCh4->setFrameShadow(QFrame::Plain);
        lcdCh4->setSmallDecimalPoint(false);
        lcdCh4->setDigitCount(5);
        lcdCh4->setSegmentStyle(QLCDNumber::Flat);
        lcdCh4->setProperty("value", QVariant(0.000000000000000));

        gridLayout_2->addWidget(lcdCh4, 1, 1, 1, 1);

        thermometer = new ThermoMeter(tab_2);
        thermometer->setObjectName("thermometer");
        thermometer->setGeometry(QRect(499, 50, 71, 251));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        led8 = new LED(tab_3);
        led8->setObjectName("led8");
        led8->setGeometry(QRect(90, 210, 50, 40));
        qwtPlot = new QwtPlot(tab_3);
        qwtPlot->setObjectName("qwtPlot");
        qwtPlot->setGeometry(QRect(230, 50, 400, 227));
        frecuencia = new QwtKnob(tab_3);
        frecuencia->setObjectName("frecuencia");
        frecuencia->setGeometry(QRect(30, 50, 131, 131));
        frecuencia->setUpperBound(5.000000000000000);
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        ACMGPIO4 = new QCheckBox(tab_4);
        ACMGPIO4->setObjectName("ACMGPIO4");
        ACMGPIO4->setGeometry(QRect(40, 70, 91, 24));
        ACMGPIO5 = new QCheckBox(tab_4);
        ACMGPIO5->setObjectName("ACMGPIO5");
        ACMGPIO5->setGeometry(QRect(150, 70, 91, 24));
        ACMGPIO6 = new QCheckBox(tab_4);
        ACMGPIO6->setObjectName("ACMGPIO6");
        ACMGPIO6->setGeometry(QRect(250, 70, 91, 24));
        ACMGPIO7 = new QCheckBox(tab_4);
        ACMGPIO7->setObjectName("ACMGPIO7");
        ACMGPIO7->setGeometry(QRect(350, 70, 91, 24));
        led_GPIO0 = new Led(tab_4);
        led_GPIO0->setObjectName("led_GPIO0");
        led_GPIO0->setGeometry(QRect(120, 200, 16, 16));
        led_GPIO0->setChecked(false);
        led_GPIO1 = new Led(tab_4);
        led_GPIO1->setObjectName("led_GPIO1");
        led_GPIO1->setGeometry(QRect(220, 200, 16, 16));
        led_GPIO1->setChecked(false);
        led_GPIO2 = new Led(tab_4);
        led_GPIO2->setObjectName("led_GPIO2");
        led_GPIO2->setGeometry(QRect(320, 200, 16, 16));
        led_GPIO2->setChecked(false);
        led_GPIO3 = new Led(tab_4);
        led_GPIO3->setObjectName("led_GPIO3");
        led_GPIO3->setGeometry(QRect(420, 200, 16, 16));
        led_GPIO3->setChecked(false);
        label_3 = new QLabel(tab_4);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(90, 220, 63, 20));
        label_4 = new QLabel(tab_4);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(190, 220, 63, 20));
        label_5 = new QLabel(tab_4);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(290, 220, 63, 20));
        label_6 = new QLabel(tab_4);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(390, 220, 63, 20));
        tabWidget->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName("tab_5");
        frecuencia_sample = new QwtKnob(tab_5);
        frecuencia_sample->setObjectName("frecuencia_sample");
        frecuencia_sample->setGeometry(QRect(150, 460, 131, 131));
        frecuencia_sample->setLowerBound(5.000000000000000);
        frecuencia_sample->setUpperBound(20.000000000000000);
        qwtPlot_acc = new QwtPlot(tab_5);
        qwtPlot_acc->setObjectName("qwtPlot_acc");
        qwtPlot_acc->setGeometry(QRect(450, 90, 400, 227));
        qwtPlot_velo = new QwtPlot(tab_5);
        qwtPlot_velo->setObjectName("qwtPlot_velo");
        qwtPlot_velo->setGeometry(QRect(450, 410, 400, 227));
        tabWidget->addTab(tab_5, QString());
        splitter_4 = new QSplitter(MainUserGUI);
        splitter_4->setObjectName("splitter_4");
        splitter_4->setGeometry(QRect(20, 10, 641, 19));
        splitter_4->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(splitter_4);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        serialPortLabel = new QLabel(splitter);
        serialPortLabel->setObjectName("serialPortLabel");
        serialPortLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        splitter->addWidget(serialPortLabel);
        serialPortComboBox = new QComboBox(splitter);
        serialPortComboBox->setObjectName("serialPortComboBox");
        splitter->addWidget(serialPortComboBox);
        splitter_4->addWidget(splitter);
        splitter_3 = new QSplitter(splitter_4);
        splitter_3->setObjectName("splitter_3");
        splitter_3->setOrientation(Qt::Horizontal);
        runButton = new QPushButton(splitter_3);
        runButton->setObjectName("runButton");
        splitter_3->addWidget(runButton);
        pingButton = new QPushButton(splitter_3);
        pingButton->setObjectName("pingButton");
        splitter_3->addWidget(pingButton);
        splitter_4->addWidget(splitter_3);
        splitter_5 = new QSplitter(MainUserGUI);
        splitter_5->setObjectName("splitter_5");
        splitter_5->setGeometry(QRect(10, 810, 691, 18));
        splitter_5->setOrientation(Qt::Horizontal);
        pushButton = new QPushButton(splitter_5);
        pushButton->setObjectName("pushButton");
        splitter_5->addWidget(pushButton);
        statusLabel = new QLabel(splitter_5);
        statusLabel->setObjectName("statusLabel");
        splitter_5->addWidget(statusLabel);
        QWidget::setTabOrder(serialPortComboBox, pingButton);
        QWidget::setTabOrder(pingButton, runButton);

        retranslateUi(MainUserGUI);

        tabWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MainUserGUI);
    } // setupUi

    void retranslateUi(QWidget *MainUserGUI)
    {
        MainUserGUI->setWindowTitle(QCoreApplication::translate("MainUserGUI", "GUIPanel", nullptr));
        rojo->setText(QCoreApplication::translate("MainUserGUI", "Rojo", nullptr));
        verde->setText(QCoreApplication::translate("MainUserGUI", "Verde", nullptr));
        azul->setText(QCoreApplication::translate("MainUserGUI", "Azul", nullptr));
        label->setText(QCoreApplication::translate("MainUserGUI", "Color", nullptr));
        label_2->setText(QCoreApplication::translate("MainUserGUI", "Brillo", nullptr));
        Mode_PWM_check->setText(QCoreApplication::translate("MainUserGUI", "PWM Mode", nullptr));
#if QT_CONFIG(tooltip)
        led1->setToolTip(QCoreApplication::translate("MainUserGUI", "Color Led component", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        led1->setWhatsThis(QCoreApplication::translate("MainUserGUI", "Led indicator", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(tooltip)
        led4->setToolTip(QCoreApplication::translate("MainUserGUI", "Color Led component", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        led4->setWhatsThis(QCoreApplication::translate("MainUserGUI", "Led indicator", nullptr));
#endif // QT_CONFIG(whatsthis)
        LeeSW_check->setText(QCoreApplication::translate("MainUserGUI", "LeeSW", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainUserGUI", "LEDs", nullptr));
        ADCButton->setText(QCoreApplication::translate("MainUserGUI", "LeeADC", nullptr));
#if QT_CONFIG(tooltip)
        thermometer->setToolTip(QCoreApplication::translate("MainUserGUI", "Shows the pressure", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        thermometer->setWhatsThis(QCoreApplication::translate("MainUserGUI", "The bar meter widget displays the pressure attached to it", nullptr));
#endif // QT_CONFIG(whatsthis)
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainUserGUI", "ADC", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainUserGUI", "Otro tab", nullptr));
        ACMGPIO4->setText(QCoreApplication::translate("MainUserGUI", "GPIO4", nullptr));
        ACMGPIO5->setText(QCoreApplication::translate("MainUserGUI", "GPIO5", nullptr));
        ACMGPIO6->setText(QCoreApplication::translate("MainUserGUI", "GPIO6", nullptr));
        ACMGPIO7->setText(QCoreApplication::translate("MainUserGUI", "GPIO7", nullptr));
#if QT_CONFIG(tooltip)
        led_GPIO0->setToolTip(QCoreApplication::translate("MainUserGUI", "Color Led component", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        led_GPIO0->setWhatsThis(QCoreApplication::translate("MainUserGUI", "Led indicator", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(tooltip)
        led_GPIO1->setToolTip(QCoreApplication::translate("MainUserGUI", "Color Led component", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        led_GPIO1->setWhatsThis(QCoreApplication::translate("MainUserGUI", "Led indicator", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(tooltip)
        led_GPIO2->setToolTip(QCoreApplication::translate("MainUserGUI", "Color Led component", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        led_GPIO2->setWhatsThis(QCoreApplication::translate("MainUserGUI", "Led indicator", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(tooltip)
        led_GPIO3->setToolTip(QCoreApplication::translate("MainUserGUI", "Color Led component", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        led_GPIO3->setWhatsThis(QCoreApplication::translate("MainUserGUI", "Led indicator", nullptr));
#endif // QT_CONFIG(whatsthis)
        label_3->setText(QCoreApplication::translate("MainUserGUI", "GPIO0", nullptr));
        label_4->setText(QCoreApplication::translate("MainUserGUI", "GPIO1", nullptr));
        label_5->setText(QCoreApplication::translate("MainUserGUI", "GPIO2", nullptr));
        label_6->setText(QCoreApplication::translate("MainUserGUI", "GPIO3", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainUserGUI", "P-1-3-1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QCoreApplication::translate("MainUserGUI", "P-1-3-5", nullptr));
        serialPortLabel->setText(QCoreApplication::translate("MainUserGUI", "Puerto Serie:", nullptr));
        runButton->setText(QCoreApplication::translate("MainUserGUI", "Inicio", nullptr));
        pingButton->setText(QCoreApplication::translate("MainUserGUI", "Ping", nullptr));
        pushButton->setText(QCoreApplication::translate("MainUserGUI", "Estado:", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainUserGUI", "Detenido", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainUserGUI: public Ui_MainUserGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUSERGUI_H
