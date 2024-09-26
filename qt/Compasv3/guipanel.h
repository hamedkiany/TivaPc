#ifndef GUIPANEL_H
#define GUIPANEL_H

// Cabecera de la clase de clase GUIPANEL.

#include <QWidget>
#include <QSerialPort> // Ahora QSerialPort esta integrado en Qt5.3
#include <qwt_dial_needle.h>
#include <qwt_analog_clock.h>
#include <QTime>

namespace Ui {
class GUIPanel;
}

class GUIPanel : public QWidget
{
    Q_OBJECT

public:
    //GUIPanel(QWidget *parent = 0);
    explicit GUIPanel(QWidget *parent = 0);
    ~GUIPanel(); // Da problemas

// Estas funciones las añade automaticamente QTCreator
private slots:
    void on_runButton_clicked();
    void on_rollSlider_valueChanged(int value);

    void on_pitchSlider_valueChanged(int value);

    void on_yawDial_valueChanged(int value);

    void on_radioButton_pressed();

    void on_radioButton_2_pressed();

private: // funciones privadas. Las debe añadir el programador
    QPixmap rotatePixmap(const QPixmap thePixmax, int angle);
    void disableWidgets();
    void enableWidgets();
    void initWidgets();
    void initYawCompass();
    void initRollCompass();
    void initPitchCompass();
    unsigned short convertScale(unsigned short value, unsigned short min, unsigned short max);
private:  // Variables privadas; excepto ui, las debe añadir el programador
    Ui::GUIPanel *ui;
    bool started;
    QPixmap originalPixmap;
};

#endif // GUIPANEL_H
