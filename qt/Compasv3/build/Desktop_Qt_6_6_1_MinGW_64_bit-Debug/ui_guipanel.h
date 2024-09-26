/********************************************************************************
** Form generated from reading UI file 'guipanel.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIPANEL_H
#define UI_GUIPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDial>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QWidget>
#include "qwt_compass.h"
#include "qwt_dial.h"

QT_BEGIN_NAMESPACE

class Ui_GUIPanel
{
public:
    QPushButton *runButton;
    QSplitter *splitter_2;
    QwtCompass *YawCompass;
    QLabel *label_2;
    QDial *yawDial;
    QLabel *headingLabel;
    QwtCompass *RollCompass;
    QLabel *RollLabel;
    QLabel *NoPitchLabel;
    QSlider *rollSlider;
    QwtCompass *PitchCompass;
    QLabel *label_3;
    QLabel *PitchLabel;
    QLabel *drone;
    QSlider *pitchSlider;
    QLCDNumber *lcdNumber;
    QLabel *RollLabel_3;
    QLabel *RollLabel_4;
    QLabel *PitchLabel2;
    QLabel *PitchLabel3;
    QLabel *Angulo_Label;
    QLabel *swheel;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;

    void setupUi(QWidget *GUIPanel)
    {
        if (GUIPanel->objectName().isEmpty())
            GUIPanel->setObjectName("GUIPanel");
        GUIPanel->resize(732, 395);
        runButton = new QPushButton(GUIPanel);
        runButton->setObjectName("runButton");
        runButton->setGeometry(QRect(640, 110, 64, 65));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/EngineButtonStart.png"), QSize(), QIcon::Normal, QIcon::Off);
        runButton->setIcon(icon);
        runButton->setIconSize(QSize(64, 64));
        runButton->setFlat(true);
        splitter_2 = new QSplitter(GUIPanel);
        splitter_2->setObjectName("splitter_2");
        splitter_2->setGeometry(QRect(10, 360, 271, 27));
        splitter_2->setOrientation(Qt::Horizontal);
        YawCompass = new QwtCompass(GUIPanel);
        YawCompass->setObjectName("YawCompass");
        YawCompass->setGeometry(QRect(40, 187, 191, 171));
        QFont font;
        font.setPointSize(10);
        YawCompass->setFont(font);
        YawCompass->setLineWidth(4);
        YawCompass->setFrameShadow(QwtDial::Sunken);
        label_2 = new QLabel(GUIPanel);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(110, 250, 51, 51));
        label_2->setMaximumSize(QSize(16777215, 16777215));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/img/Drone-Top.png")));
        label_2->setScaledContents(true);
        yawDial = new QDial(GUIPanel);
        yawDial->setObjectName("yawDial");
        yawDial->setGeometry(QRect(214, 303, 50, 64));
        yawDial->setMinimum(0);
        yawDial->setMaximum(360);
        yawDial->setValue(180);
        yawDial->setInvertedAppearance(false);
        yawDial->setWrapping(true);
        yawDial->setNotchesVisible(true);
        headingLabel = new QLabel(GUIPanel);
        headingLabel->setObjectName("headingLabel");
        headingLabel->setGeometry(QRect(100, 160, 71, 20));
        QFont font1;
        font1.setBold(true);
        headingLabel->setFont(font1);
        RollCompass = new QwtCompass(GUIPanel);
        RollCompass->setObjectName("RollCompass");
        RollCompass->setGeometry(QRect(277, 184, 191, 171));
        RollCompass->setLineWidth(4);
        RollLabel = new QLabel(GUIPanel);
        RollLabel->setObjectName("RollLabel");
        RollLabel->setGeometry(QRect(350, 160, 41, 20));
        RollLabel->setFont(font1);
        NoPitchLabel = new QLabel(GUIPanel);
        NoPitchLabel->setObjectName("NoPitchLabel");
        NoPitchLabel->setGeometry(QRect(360, 320, 41, 20));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Ubuntu Condensed")});
        font2.setPointSize(10);
        NoPitchLabel->setFont(font2);
        rollSlider = new QSlider(GUIPanel);
        rollSlider->setObjectName("rollSlider");
        rollSlider->setGeometry(QRect(320, 370, 110, 27));
        rollSlider->setMinimum(50);
        rollSlider->setMaximum(130);
        rollSlider->setValue(90);
        rollSlider->setOrientation(Qt::Horizontal);
        PitchCompass = new QwtCompass(GUIPanel);
        PitchCompass->setObjectName("PitchCompass");
        PitchCompass->setGeometry(QRect(502, 186, 191, 171));
        QFont font3;
        font3.setPointSize(8);
        PitchCompass->setFont(font3);
        PitchCompass->setLineWidth(4);
        label_3 = new QLabel(GUIPanel);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(327, 253, 91, 32));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/img/DronePerfil.png")));
        label_3->setScaledContents(true);
        PitchLabel = new QLabel(GUIPanel);
        PitchLabel->setObjectName("PitchLabel");
        PitchLabel->setGeometry(QRect(570, 161, 51, 20));
        PitchLabel->setFont(font1);
        drone = new QLabel(GUIPanel);
        drone->setObjectName("drone");
        drone->setGeometry(QRect(560, 236, 72, 75));
        drone->setPixmap(QPixmap(QString::fromUtf8(":/img/PITCH.png")));
        drone->setScaledContents(true);
        pitchSlider = new QSlider(GUIPanel);
        pitchSlider->setObjectName("pitchSlider");
        pitchSlider->setGeometry(QRect(699, 232, 20, 91));
        pitchSlider->setMinimum(-45);
        pitchSlider->setMaximum(45);
        pitchSlider->setValue(0);
        pitchSlider->setOrientation(Qt::Vertical);
        lcdNumber = new QLCDNumber(GUIPanel);
        lcdNumber->setObjectName("lcdNumber");
        lcdNumber->setGeometry(QRect(515, 96, 64, 23));
        RollLabel_3 = new QLabel(GUIPanel);
        RollLabel_3->setObjectName("RollLabel_3");
        RollLabel_3->setGeometry(QRect(270, 260, 16, 20));
        RollLabel_3->setFont(font1);
        RollLabel_4 = new QLabel(GUIPanel);
        RollLabel_4->setObjectName("RollLabel_4");
        RollLabel_4->setGeometry(QRect(460, 260, 16, 20));
        RollLabel_4->setFont(font1);
        PitchLabel2 = new QLabel(GUIPanel);
        PitchLabel2->setObjectName("PitchLabel2");
        PitchLabel2->setGeometry(QRect(490, 260, 16, 20));
        PitchLabel2->setFont(font1);
        PitchLabel3 = new QLabel(GUIPanel);
        PitchLabel3->setObjectName("PitchLabel3");
        PitchLabel3->setGeometry(QRect(680, 260, 16, 20));
        PitchLabel3->setFont(font1);
        Angulo_Label = new QLabel(GUIPanel);
        Angulo_Label->setObjectName("Angulo_Label");
        Angulo_Label->setGeometry(QRect(570, 280, 61, 20));
        Angulo_Label->setFont(font2);
        swheel = new QLabel(GUIPanel);
        swheel->setObjectName("swheel");
        swheel->setGeometry(QRect(520, 210, 151, 131));
        swheel->setPixmap(QPixmap(QString::fromUtf8(":/img/Volante.png")));
        swheel->setScaledContents(true);
        radioButton = new QRadioButton(GUIPanel);
        radioButton->setObjectName("radioButton");
        radioButton->setGeometry(QRect(450, 130, 82, 17));
        radioButton->setChecked(false);
        radioButton->setAutoExclusive(true);
        radioButton_2 = new QRadioButton(GUIPanel);
        radioButton_2->setObjectName("radioButton_2");
        radioButton_2->setGeometry(QRect(450, 150, 82, 17));
        radioButton_2->setChecked(true);
        YawCompass->raise();
        runButton->raise();
        splitter_2->raise();
        label_2->raise();
        yawDial->raise();
        headingLabel->raise();
        RollCompass->raise();
        RollLabel->raise();
        NoPitchLabel->raise();
        rollSlider->raise();
        PitchCompass->raise();
        label_3->raise();
        PitchLabel->raise();
        drone->raise();
        pitchSlider->raise();
        lcdNumber->raise();
        RollLabel_3->raise();
        RollLabel_4->raise();
        PitchLabel2->raise();
        PitchLabel3->raise();
        Angulo_Label->raise();
        swheel->raise();
        radioButton->raise();
        radioButton_2->raise();

        retranslateUi(GUIPanel);

        QMetaObject::connectSlotsByName(GUIPanel);
    } // setupUi

    void retranslateUi(QWidget *GUIPanel)
    {
        GUIPanel->setWindowTitle(QCoreApplication::translate("GUIPanel", "GUIPanel", nullptr));
        runButton->setText(QString());
        label_2->setText(QString());
        headingLabel->setText(QCoreApplication::translate("GUIPanel", "HEADING", nullptr));
        RollLabel->setText(QCoreApplication::translate("GUIPanel", "N - S", nullptr));
        NoPitchLabel->setText(QCoreApplication::translate("GUIPanel", "m/s", nullptr));
        label_3->setText(QString());
        PitchLabel->setText(QCoreApplication::translate("GUIPanel", "W - E", nullptr));
        drone->setText(QString());
        RollLabel_3->setText(QCoreApplication::translate("GUIPanel", "N", nullptr));
        RollLabel_4->setText(QCoreApplication::translate("GUIPanel", "S", nullptr));
        PitchLabel2->setText(QCoreApplication::translate("GUIPanel", "W", nullptr));
        PitchLabel3->setText(QCoreApplication::translate("GUIPanel", "E", nullptr));
        Angulo_Label->setText(QCoreApplication::translate("GUIPanel", " Angulo ", nullptr));
        swheel->setText(QString());
        radioButton->setText(QCoreApplication::translate("GUIPanel", "Volante", nullptr));
        radioButton_2->setText(QCoreApplication::translate("GUIPanel", "Avion", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GUIPanel: public Ui_GUIPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIPANEL_H
