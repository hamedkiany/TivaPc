// Clase GUIPANEL implementa las funciones a ejecutar cuando se realizan acciones en los componentes (widgets)
// de su interfaz gráfico asociado.

#include "guipanel.h"     // Cabecera de la clase
#include "ui_guipanel.h"  // Cabecera de componentes del interfaz gráfico

#include <QPainter>       // colores diferentes para los componentes
#include <QTimer>
#include <QGraphicsPixmapItem>

#include <qwt_dial_needle.h>
#include <qwt_round_scale_draw.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_div.h>
#include <qwt_scale_engine.h>
#include <qwt_compass.h>
#include <qwt_interval.h>

#if QT_VERSION < 0x040000
#include <QColorGroup>    // Cabeceras para definir espacios de
typedef QColorGroup Palette;
#else
typedef QPalette Palette;
#endif

GUIPanel::GUIPanel(QWidget *parent) :  // Inicializacion de variables
    QWidget(parent),
    ui(new Ui::GUIPanel)               // Indica que guipanel.ui es el interfaz grafico de la clase
{
    ui->setupUi(this);                // Conecta la clase con su interfaz gráfico.
    setWindowTitle(tr("Esferas tipo Compas")); // Título de la ventana



       // Inicializa componentes para los controles y mandos
    initYawCompass();  // Pinta y configura el componente del control de direccion (Yaw)
    initRollCompass(); // Pinta y configura el componente del control de alabeo (Roll)
    initPitchCompass(); // Pinta y configura el componente del control de angulo de ataque (Pitch)

    // Configura otros controles e indicadores del GUI

    // Deshabilita controles hasta que nos conectemos
    // Configuración inicial del indicadores varios
    initWidgets();
    disableWidgets();
    started = false;
    originalPixmap=(QPixmap) (ui->drone->pixmap());
    ui->swheel->setVisible(false);

    //ui->pitchLabel->move(200,100);
}

QPixmap GUIPanel::rotatePixmap(QPixmap thePixmax, int angle){
    QSize size = originalPixmap.size();
    QPixmap rotatedPixmap(size);
    rotatedPixmap.fill(QColor::fromRgb(0, 0, 0, 0)); //the new pixmap must be transparent.
    QPainter* p = new QPainter(&rotatedPixmap);
    p->translate(size.height()/2,size.height()/2);
    p->rotate(angle);
    p->translate(-size.height()/2,-size.height()/2);
    p->drawPixmap(0, 0, originalPixmap);
    p->end();
    delete p;
    return rotatedPixmap;
}

GUIPanel::~GUIPanel() // Destructor de la clase
{
    delete ui;   // Borra el interfaz gráfico asociado a la clase
}

// Deshabilita los widgets mientras no queramos que funcionen
void GUIPanel::disableWidgets(){
    // Esconde controles de PRUEBA (*QUITAR en la aplicacion -> solo de ejemplo)
    ui->yawDial->setVisible(false);
    ui->rollSlider->setVisible(false);
    ui->pitchSlider->setVisible(false);

    // Deshabilita controles e indicadores para que no funcionen hasta pulsar el boton de inicio
    ui->YawCompass->setEnabled(false);
    ui->RollCompass->setEnabled(false);
    ui->PitchCompass->setEnabled(false);
}

// Habilita los widgets para poder utilizarlos
void GUIPanel::enableWidgets(){
    // Muestra controles de PRUEBA (*QUITAR -> solo de ejemplo)
    ui->yawDial->setVisible(true);
    ui->rollSlider->setVisible(true);
    ui->pitchSlider->setVisible(true);

    // Habilita controles e indicadores
    ui->YawCompass->setEnabled(true);
    ui->RollCompass->setEnabled(true);
    ui->PitchCompass->setEnabled(true);

}

// Estado inicial de los widgets
void GUIPanel::initWidgets(){
    // Configuración inicial del indicadores varios

    ui->RollCompass->setValue(90); // Estado inicial del giroscopio de ROLL
    ui->rollSlider->setValue(90);  // Control de ROLL: Quitar en aplicacion final

    ui->YawCompass->setValue(0); // Estado inicial del giroscopo de PITCH
    ui->yawDial->setValue(180);  // Control de Pitch: Quitar en aplicación final

}


// Funciones SLOT que se crean automaticamente desde QTDesigner al activar una señal de un WIdget del interfaz gráfico
// Se suelen asociar a funciones auxiliares, en muchos caso, por comodidad.

// SLOT asociada a pulsación del botón RUN
void GUIPanel::on_runButton_clicked(){
    if(!started){
        enableWidgets();
        ui->runButton->setIcon(QIcon(QPixmap(":/img/EngineButtonStop.png")));
        started = true;
    }else{
        disableWidgets();
        ui->runButton->setIcon(QIcon(QPixmap(":/img/EngineButtonStart.png")));
        started = false;
    }
}

// Configuracion e inicialización de la esfera de "dirección" (YAW) del avion
void GUIPanel::initYawCompass(){
    // Color del fondo de la esfera
    // Define una Palette con los colores de los elementos de la esfera
    // (ver initRollCompass, por ejemplo)

    ui->YawCompass->setMode(QwtDial::RotateScale); // Modo rotacion escala
    //ui->YawCompass->setMode(QwtDial::RotateNeedle); // Modo rotacion aguja
    // Si quieres que aparezca una aguja tipo "flecha"
    //ui->YawCompass->setNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow));
    ui->YawCompass->setValue(0); // Apunta hacia al N al inicio

    // Haz una escala de etiquetas y ticks de 0 a 360º (en intervalos de 20 o 30º)
    // Tambien puedes colocar indicadores de "N","S","E," y "O", en los angulos
    // correspondientes a 0,90,180,y 270º
    // (ver initRollCompass, por ejemplo)

    // Muestra marcas/ticks, etiquetas, y "corona" interior
    // Haz las divisiones de la escala de forma similar a la mostrada
    // en initRollCompass.

    // Control con QDial (BORRAR en la aplicación final)
    ui->yawDial->setValue(180); // Valor inicial a 180; aqui el 0 es el Sur!!
    ui->yawDial->setNotchTarget(30); // Marcas cada 30º
}


// Configuración e inicialización de la esfera de "alabeo" (ROLL)
void GUIPanel::initRollCompass(){

    // Color del fondo de la esfera
    Palette colorGroup;
    colorGroup.setColor(Palette::Base, QColor(Qt::darkGray).darker(180));
    colorGroup.setColor(Palette::WindowText,QColor(Qt::white).darker(280));
    colorGroup.setColor(Palette::Text, Qt::white);
    ui->RollCompass->setPalette(colorGroup);

    // Aguja tipo "brujula"
    ui->RollCompass->setNeedle(new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::ThinStyle));
    ui->RollCompass->setMode(QwtDial::RotateNeedle); // Lo que gira es la aguja

    // Escala para la esfera
    // Etiquetas y ticks/marcas
    QMap<double, QString> map; // Indicamos que etiquetas queremos que salgan
    map.insert(90, "0");      // y donde
    map.insert(270, "0");
    map.insert(70, "1");
    map.insert(50, "2");
    map.insert(290, "1");
    map.insert(310, "2");
    QwtCompassScaleDraw *rollScDraw = new QwtCompassScaleDraw(map);
    // Longitud de las marcas
    rollScDraw->setTickLength(QwtScaleDiv::MinorTick,0);  // Solo queremos
    rollScDraw->setTickLength(QwtScaleDiv::MediumTick,0); // que aparezcan las
    rollScDraw->setTickLength(QwtScaleDiv::MajorTick,10);  // marcas grandes
    // Establecemos donde irán los Ticks (0 esta al "Norte" de la "brujula"
    QList< double > ticks;
    ticks.append(50.0);
    ticks.append(70.0);
    ticks.append(90.0);
    ticks.append(110.0);
    ticks.append(130.0);
    ticks.append(230.0);
    ticks.append(250.0);
    ticks.append(270.0);
    ticks.append(290.0); // no 290, porque si no, no sale..(no se porqué)
    ticks.append(310.0); // no 290, porque si no, no sale..(no se porqué)
    // Divisiones de la escala
    QwtScaleDiv myScDiv=rollScDraw->scaleDiv(); // Trabajamos con la escala

    myScDiv.setInterval(QwtInterval(0,360)); // Rango 0-360º
    // Solo aparecen ticks en la escala "mayor" (el resto está vacío)
    myScDiv.setTicks(QwtScaleDiv::MinorTick,QList<double>());
    myScDiv.setTicks(QwtScaleDiv::MediumTick,QList<double>());
    myScDiv.setTicks(QwtScaleDiv::MajorTick,ticks);
    // Indicamos que no aparezca el "circulo interior" pero si, las etiquetas
    // y las marcas
    rollScDraw->enableComponent(QwtAbstractScaleDraw::Labels,true);
    rollScDraw->enableComponent(QwtAbstractScaleDraw::Ticks,true);
    rollScDraw->enableComponent(QwtAbstractScaleDraw::Backbone,false);
    rollScDraw->setSpacing(10); // Distancia al centro de la corona de ticks
    // Asignamos el componente de escala al compas
    ui->RollCompass->setScaleDraw(rollScDraw);
    // .. y el componente de divisiones a la escala
    rollScDraw->setScaleDiv(myScDiv); //(si se hace en otro orden NO funciona)

    ui->RollCompass->setValue(90); // Posicion inicial (0 es el Norte)
    ui->RollCompass->setWrapping(false); // La aguja no puede superar los valores máximo o minimo

    //Esto de abajo permite que la etiqueta salga de color blanco
    ui->NoPitchLabel->setText("<font color='white'>m/s</font>");
}

// Configuracion e inicialización de la esfera de "angulo de ataque" (PITCH) del avion

void GUIPanel::initPitchCompass(){
    // Color del fondo de la esfera
    Palette colorGroup;
    colorGroup.setColor(Palette::Base, Qt::darkBlue);
    colorGroup.setColor(Palette::WindowText,
                        QColor(Qt::blue).darker(120));
    colorGroup.setColor(Palette::Text, Qt::white);
    ui->PitchCompass->setPalette(colorGroup);
    ui->PitchCompass->setMode(QwtDial::RotateNeedle); // Modo rotacion escala

    // Aguja tipo "rayo" // COMENTADO PARA USAR ROTACION DE PIXMAP
    //ui->PitchCompass->setNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow));
   /* ui->PitchCompass->setValue(90); // Aguja hacia al E al inicio
    ui->pitchSlider->setMinimum(45);
    ui->pitchSlider->setMaximum(135);*/

    // Escala para la esfera
    // Etiquetas y ticks/marcas
    QMap<double, QString> map; // Indicamos que etiquetas queremos que salgan
    QString label;
    label.asprintf("60");
    map.insert(30, label);
    label.asprintf("45");
    map.insert(45, label);
    label.asprintf("0");
    map.insert(90, label);
    label.asprintf("-45");
    map.insert(135, label);
    label.asprintf("-60");
    map.insert(150, label);

    label.asprintf("60");
    map.insert(330, label);
    label.asprintf("45");
    map.insert(315, label);
    label.asprintf("0");
    map.insert(293, label);
    label.asprintf("-45");
    map.insert(225, label);
    label.asprintf("-60");
    map.insert(210, label);
    QwtCompassScaleDraw *pitchScDraw = new QwtCompassScaleDraw(map);

    // Longitud de las marcas
    pitchScDraw ->setTickLength(QwtScaleDiv::MinorTick,0);  // Solo queremos
    pitchScDraw->setTickLength(QwtScaleDiv::MediumTick,0); // que aparezcan las
    pitchScDraw ->setTickLength(QwtScaleDiv::MajorTick,10);  // marcas grandes
    // Establecemos donde irán los Ticks (0 esta al "Norte" de la "brujula"
    QList< double > ticks;
    ticks.append(30.0);
    ticks.append(45.0);
    ticks.append(67.0);
    ticks.append(90.0);
    ticks.append(112.0);
    ticks.append(135.0);
    ticks.append(150.0);

    ticks.append(330.0);
    ticks.append(315.0);
    ticks.append(293.0);
    ticks.append(270.0);
    ticks.append(248.0);
    ticks.append(225.0);
    ticks.append(210.0);
    // Divisiones de la escala
    QwtScaleDiv myScDiv=pitchScDraw->scaleDiv(); // Trabajamos con la escala

    myScDiv.setInterval(QwtInterval(0,360)); // Rango 0-360º
    // Solo aparecen ticks en la escala "mayor" (el resto está vacío)
    myScDiv.setTicks(QwtScaleDiv::MinorTick,QList<double>());
    myScDiv.setTicks(QwtScaleDiv::MediumTick,QList<double>());
    myScDiv.setTicks(QwtScaleDiv::MajorTick,ticks);
    // Indicamos que aparezca el "circulo interior",etiquetas y  marcas
    pitchScDraw->enableComponent(QwtAbstractScaleDraw::Labels,true);
    pitchScDraw->enableComponent(QwtAbstractScaleDraw::Ticks,true);
    pitchScDraw->enableComponent(QwtAbstractScaleDraw::Backbone,true);
    pitchScDraw->setSpacing(8); // Distancia al centro de la corona de ticks
    // Asignamos el componente de escala al compas
    ui->PitchCompass->setScaleDraw(pitchScDraw);
    // .. y el componente de divisiones a la escala
    pitchScDraw->setScaleDiv(myScDiv); //(si se hace en otro orden NO funciona)

    //Esto de abajo permite que la etiqueta salga de color blanco
    ui->Angulo_Label->setText("<font color='white'>Angulo</font>");

    ui->PitchCompass->setReadOnly(true);
    ui->PitchCompass->setWrapping(false); // La aguja no puede superar los valores máximo o minimo
}

// Convierte un valor en la escala 0-4096 (centrado en 2048) a otro en la escala (min,max),
// centrado en (max-min)/2
unsigned short GUIPanel::convertScale(unsigned short value, unsigned short min, unsigned short max){
    float temp = ((float)value)/4096;
    temp = (temp * (max - min)) + min;
    return temp;
}

// Slot asociada a la variacion de "alabeo" (ROLL) --> en la aplicación final esto debe
// asociarse al comando recibido de TIVA actualizando el estado de alabeo, en funcion del
// potenciometro asociado
void GUIPanel::on_rollSlider_valueChanged(int value)
{
    ui->RollCompass->setValue((double)value);
}

void GUIPanel::on_pitchSlider_valueChanged(int value)
{
    //ui->PitchCompass->setValue((double)value); // Comentado para usar el pixmap
    if(ui->radioButton->isChecked())
             ui->swheel->setPixmap(rotatePixmap((ui->swheel->pixmap()),value));
        else
             ui->drone->setPixmap(rotatePixmap((ui->drone->pixmap()),value));
    ui->lcdNumber->display(value);
}

// Slot asociada a la variacion de "direccion" (YAW) --> en la aplicación final esto debe
// asociarse al comando recibido de TIVA actualizando el estado de dirección, en funcion del
// potenciometro asociado
void GUIPanel::on_yawDial_valueChanged(int value)
{
    // En el control por el dial, se le suma 180, porque el 0 esta en el N en la esfera, y en el
    // S en el dial de control.
    if(value < 180)
        ui->YawCompass->setValue(value + 180);
    else
        ui->YawCompass->setValue(value-180);
}

void GUIPanel::on_radioButton_2_pressed()
{
    ui->drone->setVisible(true);
    ui->swheel->setVisible(false);
    originalPixmap=(QPixmap) (ui->drone->pixmap());
    ui->pitchSlider->setMaximum(45);
    ui->pitchSlider->setMinimum(-45);

}

void GUIPanel::on_radioButton_pressed()
{
    ui->drone->setVisible(false);
    ui->swheel->setVisible(true);
    originalPixmap=(QPixmap) (ui->swheel->pixmap());
    ui->pitchSlider->setMaximum(90);
    ui->pitchSlider->setMinimum(-90);
}
