/********************************************************************************
** Form generated from reading UI file 'guipanel.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIPANEL_H
#define UI_GUIPANEL_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include "analogwidgets/counter.h"
#include "analogwidgets/led.h"
#include "analogwidgets/potentiometer.h"
#include "analogwidgets/thermometer.h"
#include "color_preview.hpp"
#include "color_wheel.hpp"
#include "qdigitalgauge.h"
#include "qhexedit.h"
#include "qledmatrix.h"
#include "qneedleindicator.h"
#include "qwt_knob.h"
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_GUIPanel
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QwtPlot *Grafica;
    QwtKnob *frecuencia;
    QTextBrowser *textBrowser;
    QWidget *tab_4;
    QHexEdit *hexEdit;
    QNeedleIndicator *qneedleindicator;
    QwtKnob *Knob_3;
    QWidget *tab_2;
    Led *led;
    PotentioMeter *dial;
    ThermoMeter *thermometer;
    color_widgets::ColorWheel *colorWheel;
    QwtKnob *Knob;
    QDigitalGauge *qdigitalgauge;
    QCheckBox *checkBox;
    color_widgets::ColorPreview *colorPreview;
    Counter *counter;
    QLedMatrix *ledMatrix;
    QWidget *tab_3;
    QChartView *chartview;
    QChartView *chartview_2;
    QChartView *chartview_3;
    QChartView *chartview_4;
    QwtKnob *Knob_2;
    QLabel *label;

    void setupUi(QWidget *GUIPanel)
    {
        if (GUIPanel->objectName().isEmpty())
            GUIPanel->setObjectName("GUIPanel");
        GUIPanel->resize(1101, 638);
        tabWidget = new QTabWidget(GUIPanel);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, 0, 1101, 641));
        tab = new QWidget();
        tab->setObjectName("tab");
        Grafica = new QwtPlot(tab);
        Grafica->setObjectName("Grafica");
        Grafica->setGeometry(QRect(40, 40, 941, 411));
        Grafica->setAutoFillBackground(false);
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        Grafica->setCanvasBackground(brush);
        Grafica->setAutoReplot(true);
        frecuencia = new QwtKnob(tab);
        frecuencia->setObjectName("frecuencia");
        frecuencia->setGeometry(QRect(70, 470, 131, 131));
        frecuencia->setUpperBound(5.000000000000000);
        textBrowser = new QTextBrowser(tab);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(320, 470, 711, 131));
        tabWidget->addTab(tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        hexEdit = new QHexEdit(tab_4);
        hexEdit->setObjectName("hexEdit");
        hexEdit->setGeometry(QRect(40, 10, 831, 281));
        hexEdit->setAsciiArea(true);
        hexEdit->setCursorPosition(Q_INT64_C(0));
        hexEdit->setData("");
        hexEdit->setReadOnly(false);
        qneedleindicator = new QNeedleIndicator(tab_4);
        qneedleindicator->setObjectName("qneedleindicator");
        qneedleindicator->setGeometry(QRect(630, 340, 200, 200));
        Knob_3 = new QwtKnob(tab_4);
        Knob_3->setObjectName("Knob_3");
        Knob_3->setGeometry(QRect(420, 370, 150, 150));
        Knob_3->setUpperBound(100.000000000000000);
        tabWidget->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        led = new Led(tab_2);
        led->setObjectName("led");
        led->setGeometry(QRect(780, 90, 91, 61));
        dial = new PotentioMeter(tab_2);
        dial->setObjectName("dial");
        dial->setGeometry(QRect(110, 70, 100, 100));
        thermometer = new ThermoMeter(tab_2);
        thermometer->setObjectName("thermometer");
        thermometer->setGeometry(QRect(290, 50, 40, 160));
        colorWheel = new color_widgets::ColorWheel(tab_2);
        colorWheel->setObjectName("colorWheel");
        colorWheel->setGeometry(QRect(510, 60, 100, 100));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(colorWheel->sizePolicy().hasHeightForWidth());
        colorWheel->setSizePolicy(sizePolicy);
        Knob = new QwtKnob(tab_2);
        Knob->setObjectName("Knob");
        Knob->setGeometry(QRect(40, 280, 150, 150));
        qdigitalgauge = new QDigitalGauge(tab_2);
        qdigitalgauge->setObjectName("qdigitalgauge");
        qdigitalgauge->setGeometry(QRect(240, 280, 161, 151));
        QFont font;
        font.setPointSize(8);
        qdigitalgauge->setFont(font);
        checkBox = new QCheckBox(tab_2);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(800, 170, 80, 23));
        checkBox->setChecked(true);
        colorPreview = new color_widgets::ColorPreview(tab_2);
        colorPreview->setObjectName("colorPreview");
        colorPreview->setGeometry(QRect(650, 90, 24, 24));
        counter = new Counter(tab_2);
        counter->setObjectName("counter");
        counter->setGeometry(QRect(80, 200, 100, 40));
        ledMatrix = new QLedMatrix(tab_2);
        ledMatrix->setObjectName("ledMatrix");
        ledMatrix->setGeometry(QRect(490, 300, 411, 100));
        ledMatrix->setBackgroundColor(QColor(0, 0, 0));
        ledMatrix->setBackgroundMode(Qt::OpaqueMode);
        ledMatrix->setDarkLedColor(QColor(34, 34, 34));
        ledMatrix->setProperty("rows", QVariant(16));
        ledMatrix->setProperty("columns", QVariant(64));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        chartview = new QChartView(tab_3);
        chartview->setObjectName("chartview");
        chartview->setGeometry(QRect(20, 40, 291, 261));
        chartview_2 = new QChartView(tab_3);
        chartview_2->setObjectName("chartview_2");
        chartview_2->setGeometry(QRect(10, 320, 871, 261));
        chartview_3 = new QChartView(tab_3);
        chartview_3->setObjectName("chartview_3");
        chartview_3->setGeometry(QRect(310, 40, 291, 261));
        chartview_4 = new QChartView(tab_3);
        chartview_4->setObjectName("chartview_4");
        chartview_4->setGeometry(QRect(600, 40, 291, 261));
        Knob_2 = new QwtKnob(tab_3);
        Knob_2->setObjectName("Knob_2");
        Knob_2->setGeometry(QRect(890, 110, 150, 150));
        label = new QLabel(tab_3);
        label->setObjectName("label");
        label->setGeometry(QRect(910, 240, 121, 71));
        label->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        label->setWordWrap(true);
        label->setMargin(1);
        tabWidget->addTab(tab_3, QString());

        retranslateUi(GUIPanel);
        QObject::connect(Knob, SIGNAL(valueChanged(double)), qdigitalgauge, SLOT(setValue(double)));
        QObject::connect(dial, SIGNAL(valueChanged(int)), thermometer, SLOT(setValue(int)));
        QObject::connect(checkBox, SIGNAL(toggled(bool)), led, SLOT(setChecked(bool)));
        QObject::connect(colorWheel, SIGNAL(colorChanged(QColor)), colorPreview, SLOT(setColor(QColor)));
        QObject::connect(dial, SIGNAL(valueChanged(int)), counter, SLOT(setValue(int)));
        QObject::connect(Knob_3, SIGNAL(valueChanged(double)), qneedleindicator, SLOT(setValue(double)));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(GUIPanel);
    } // setupUi

    void retranslateUi(QWidget *GUIPanel)
    {
        GUIPanel->setWindowTitle(QCoreApplication::translate("GUIPanel", "GUIPanel", nullptr));
        textBrowser->setHtml(QCoreApplication::translate("GUIPanel", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt;\">Este ejemplo muestra c\303\263mo pintar curvas en una gr\303\241fica de Qt. Las curvas que se pintan son unas sinusoides calculadas seg\303\272n una f\303\263rmula. El periodo de las sinusoides se puede cambiar girando la rosca (para demostrar c\303\263mo se repintan las gr\303\241ficas). Esto es s\303\255mplemente un ejemplo, las curvas"
                        " que hay que pintar al realizar la pr\303\241ctica son las muestras obtenidas con el conversor anal\303\263gico digital por la Tiva y mandadas hacia Qt. Se pintar\303\241 una curva por cada canal de entrada (4 en total). La gr\303\241fica se actualizar\303\241 conforme vayan llegando nuevas muestras, descartando (borrando) las muestras m\303\241s antiguas. Cuando lleguen muestras nuevas no s\303\263lo se pintar\303\241n las que hayan llegado, sino tambi\303\251n las anteriores. Cada curva pintada contendr\303\241 al menos las \303\272ltimas 1000 muestras. Al arrancar la funcionalidad, las muestras estar\303\241n inicializadas a 0 y se ir\303\241n actualizando con los valores que vayan llegando. </span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("GUIPanel", "Qwt Plot", nullptr));
#if QT_CONFIG(tooltip)
        hexEdit->setToolTip(QCoreApplication::translate("GUIPanel", "QHexEditWidget", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        hexEdit->setWhatsThis(QCoreApplication::translate("GUIPanel", "QHexEdit widget allow to edit the data in hex view.", nullptr));
#endif // QT_CONFIG(whatsthis)
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("GUIPanel", "Page", nullptr));
#if QT_CONFIG(tooltip)
        led->setToolTip(QCoreApplication::translate("GUIPanel", "Color Led component", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        led->setWhatsThis(QCoreApplication::translate("GUIPanel", "Led indicator", nullptr));
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(tooltip)
        dial->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        dial->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
#if QT_CONFIG(tooltip)
        thermometer->setToolTip(QCoreApplication::translate("GUIPanel", "Shows the pressure", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        thermometer->setWhatsThis(QCoreApplication::translate("GUIPanel", "The bar meter widget displays the pressure attached to it", nullptr));
#endif // QT_CONFIG(whatsthis)
        checkBox->setText(QCoreApplication::translate("GUIPanel", "Enciende", nullptr));
#if QT_CONFIG(tooltip)
        counter->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(whatsthis)
        counter->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("GUIPanel", "Widgets Varios", nullptr));
        label->setText(QCoreApplication::translate("GUIPanel", "La porcion de Joe cambia al soltar la rosca", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("GUIPanel", "QCharts", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GUIPanel: public Ui_GUIPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIPANEL_H
