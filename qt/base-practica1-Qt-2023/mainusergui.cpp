#include "mainusergui.h"
#include "ui_mainusergui.h"
#include <QSerialPort>      // Comunicacion por el puerto serie
#include <QSerialPortInfo>  // Comunicacion por el puerto serie
#include <QMessageBox>      // Se deben incluir cabeceras a los componentes que se vayan a crear en la clase
#include <QDebug>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_graphic.h>
//for new chart

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
//end for new chart

// y que no estén incluidos en el interfaz gráfico. En este caso, la ventana de PopUp <QMessageBox>
// que se muestra al recibir un PING de respuesta

#include<stdint.h>      // Cabecera para usar tipos de enteros con tamaño
#include<stdbool.h>     // Cabecera para usar booleanos
int counter = 0;
int j=0,jj=0,tj=0;
MainUserGUI::MainUserGUI(QWidget *parent) :  // Constructor de la clase
    QWidget(parent),
    ui(new Ui::MainUserGUI)               // Indica que guipanel.ui es el interfaz grafico de la clase
  , transactionCount(0)
{
    ui->setupUi(this);                // Conecta la clase con su interfaz gráfico.
    setWindowTitle(tr("Interfaz de Control TIVA")); // Título de la ventana

    // Inicializa la lista de puertos serie
    ui->serialPortComboBox->clear(); // Vacía de componentes la comboBox
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        // La descripción realiza un FILTRADO que  nos permite que SOLO aparezcan los interfaces tipo USB serial con el identificador de fabricante
        // y producto de la TIVA
        // NOTA!!: SI QUIERES REUTILIZAR ESTE CODIGO PARA OTRO CONVERSOR USB-Serie, cambia los valores VENDOR y PRODUCT por los correspondientes
        // o cambia la condicion por "if (true) para listar todos los puertos serie"
        if ((info.vendorIdentifier()==0x1CBE) && (info.productIdentifier()==0x0002))
        {
            ui->serialPortComboBox->addItem(info.portName());
        }
    }

    //Inicializa algunos controles
    ui->serialPortComboBox->setFocus();   // Componente del GUI seleccionado de inicio
    ui->pingButton->setEnabled(false);    // Se deshabilita el botón de ping del interfaz gráfico, hasta que

    //Inicializa la ventana de PopUp que se muestra cuando llega la respuesta al PING
    ventanaPopUp.setIcon(QMessageBox::Information);
    ventanaPopUp.setText(tr("Status: RESPUESTA A PING RECIBIDA")); //Este es el texto que muestra la ventana
    ventanaPopUp.setStandardButtons(QMessageBox::Ok);
    ventanaPopUp.setWindowTitle(tr("Evento"));
    ventanaPopUp.setParent(this,Qt::Popup);


    //Semana 2. Inicializacion GRAFICA
    ui->qwtPlot->setTitle("Sinusoides"); // Titulo de la grafica
    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, "Tiempo"); // Etiqueta eje X de coordenadas
    ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, "Valor");    // Etiqueta eje Y de coordenadas
    //ui->Grafica->axisAutoScale(true); // Con Autoescala
    ui->qwtPlot->setAxisScale(QwtPlot::yLeft, 0, 3.3); // Escala fija
    ui->qwtPlot->setAxisScale(QwtPlot::xBottom,0,1024.0);

    // Formateo de la curva
    for(int i=0;i<6;i++){
        Channels[i] = new QwtPlotCurve();
        Channels[i]->attach(ui->qwtPlot);
    }

    //Inicializacion de los valores básicos
    for(int i=0;i<1024;i++){
        xVal[i]=i;
        yVal[0][i]=3.3*(sin((double)i*2.0*3.14159/1024.0)+1.0)/2.0;
        yVal[1][i]=3.3*(sin((double)i*4.0*3.14159/1024.0)+1.0)/2.0;
        yVal[2][i]=3.3*(sin((double)i*8.0*3.14159/1024.0)+1.0)/2.0;
        yVal[3][i]=3.3*(sin((double)i*16.0*3.14159/1024.0)+1.0)/2.0;
        yVal[4][i]=3.3*(sin((double)i*16.0*3.14159/1024.0)+1.0)/2.0;
        yVal[5][i]=3.3*(sin((double)i*16.0*3.14159/1024.0)+1.0)/2.0;

    }

    Channels[0]->setRawSamples(xVal,yVal[0],1024);
    Channels[1]->setRawSamples(xVal,yVal[1],1024);
    Channels[2]->setRawSamples(xVal,yVal[2],1024);
    Channels[3]->setRawSamples(xVal,yVal[3],1024);
    Channels[4]->setRawSamples(xVal,yVal[4],1024);
    Channels[5]->setRawSamples(xVal,yVal[5],1024);

    Channels[0]->setPen(QPen(Qt::red)); // Color de la curva
    Channels[1]->setPen(QPen(Qt::cyan));
    Channels[2]->setPen(QPen(Qt::yellow));
    Channels[3]->setPen(QPen(Qt::green));
    Channels[4]->setPen(QPen(Qt::blue));
    Channels[5]->setPen(QPen(Qt::green));

    m_Grid = new QwtPlotGrid();     // Rejilla de puntos
    m_Grid->attach(ui->qwtPlot);    // que se asocia al objeto QwtPl
    ui->qwtPlot->setAutoReplot(false); //Desactiva el autoreplot (mejora la eficiencia)
    //Semana 2. FIN Inicializacion GRAFICA

    //accelarcion. Inicializacion GRAFICA
    ui->qwtPlot_acc->setTitle("Acceleracion"); // Titulo de la grafica
    ui->qwtPlot_acc->setAxisTitle(QwtPlot::xBottom, "Tiempo"); // Etiqueta eje X de coordenadas
    ui->qwtPlot_acc->setAxisTitle(QwtPlot::yLeft, "Valor");    // Etiqueta eje Y de coordenadas
    //ui->Grafica->axisAutoScale(true); // Con Autoescala
    ui->qwtPlot_acc->setAxisScale(QwtPlot::yLeft, 0, 3.3); // Escala fija
    ui->qwtPlot_acc->setAxisScale(QwtPlot::xBottom,0,1024.0);

    // Formateo de la curva
    for(int i=0;i<3;i++){
        Channels2[i] = new QwtPlotCurve();
        Channels2[i]->attach(ui->qwtPlot_acc);
    }

    //Inicializacion de los valores básicos
    for(int i=0;i<1024;i++){
        xxVal[i]=i;
        yyVal[0][i]=3.3*(sin((double)i*2.0*3.14159/1024.0)+1.0)/2.0;
        yyVal[1][i]=3.3*(sin((double)i*4.0*3.14159/1024.0)+1.0)/2.0;
        yyVal[2][i]=3.3*(sin((double)i*8.0*3.14159/1024.0)+1.0)/2.0;

    }

    Channels2[0]->setRawSamples(xxVal,yyVal[0],1024);
    Channels2[1]->setRawSamples(xxVal,yyVal[1],1024);
    Channels2[2]->setRawSamples(xxVal,yyVal[2],1024);

    Channels2[0]->setPen(QPen(Qt::red)); // Color de la curva
    Channels2[1]->setPen(QPen(Qt::cyan));
    Channels2[2]->setPen(QPen(Qt::yellow));

    m_Grid2 = new QwtPlotGrid();     // Rejilla de puntos
    m_Grid2->attach(ui->qwtPlot_acc);    // que se asocia al objeto QwtPl
    ui->qwtPlot_acc->setAutoReplot(false); //Desactiva el autoreplot (mejora la eficiencia)
    //accelarcion . FIN Inicializacion GRAFICA


    //velocidad. Inicializacion GRAFICA
    ui->qwtPlot_velo->setTitle("Velocidad"); // Titulo de la grafica
    ui->qwtPlot_velo->setAxisTitle(QwtPlot::xBottom, "Tiempo"); // Etiqueta eje X de coordenadas
    ui->qwtPlot_velo->setAxisTitle(QwtPlot::yLeft, "Valor");    // Etiqueta eje Y de coordenadas
    //ui->Grafica->axisAutoScale(true); // Con Autoescala
    ui->qwtPlot_velo->setAxisScale(QwtPlot::yLeft, 0, 3.3); // Escala fija
    ui->qwtPlot_velo->setAxisScale(QwtPlot::xBottom,0,1024.0);

    // Formateo de la curva
    for(int k=0;k<3;k++){
        Channels3[k] = new QwtPlotCurve();
        Channels3[k]->attach(ui->qwtPlot_velo);
    }

    //Inicializacion de los valores básicos
    for(int k=0;k<1024;k++){
        r2Val[k]=k;
        xyzVal[0][k]=3.3*(sin((double)k*2.0*3.14159/1024.0)+1.0)/2.0;
        xyzVal[1][k]=3.3*(sin((double)k*4.0*3.14159/1024.0)+1.0)/2.0;
        xyzVal[2][k]=3.3*(sin((double)k*8.0*3.14159/1024.0)+1.0)/2.0;

    }

    Channels3[0]->setRawSamples(r2Val,xyzVal[0],1024);
    Channels3[1]->setRawSamples(r2Val,xyzVal[1],1024);
    Channels3[2]->setRawSamples(r2Val,xyzVal[2],1024);

    Channels3[0]->setPen(QPen(Qt::red)); // Color de la curva
    Channels3[1]->setPen(QPen(Qt::cyan));
    Channels3[2]->setPen(QPen(Qt::yellow));

    m_Grid3 = new QwtPlotGrid();     // Rejilla de puntos
    m_Grid3->attach(ui->qwtPlot_velo);    // que se asocia al objeto QwtPl
    ui->qwtPlot_velo->setAutoReplot(false); //Desactiva el autoreplot (mejora la eficiencia)
    //velocidad . FIN Inicializacion GRAFICA


    //Conexion de signals de los widgets del interfaz con slots propios de este objeto
    connect(ui->rojo,SIGNAL(toggled(bool)),this,SLOT(cambiaLEDs()));
    connect(ui->verde,SIGNAL(toggled(bool)),this,SLOT(cambiaLEDs()));
    connect(ui->azul,SIGNAL(toggled(bool)),this,SLOT(cambiaLEDs()));
    connect(ui->ACMGPIO4,SIGNAL(toggled(bool)),this,SLOT(cambiaACMs()));
    connect(ui->ACMGPIO5,SIGNAL(toggled(bool)),this,SLOT(cambiaACMs()));
    connect(ui->ACMGPIO6,SIGNAL(toggled(bool)),this,SLOT(cambiaACMs()));
    connect(ui->ACMGPIO7,SIGNAL(toggled(bool)),this,SLOT(cambiaACMs()));
    connect(ui->Mode_PWM_check,SIGNAL(toggled(bool)),this,SLOT(cambiaMode()));
    connect(ui->colorWheel,SIGNAL(toggled(bool)),this,SLOT(on_colorWheel_colorChanged(const QColor &arg1)));
    connect(ui->Knob,SIGNAL(toggled(bool)),this,SLOT(on_Knob_valueChanged(double value)));
    connect(ui->LeeSW_check,SIGNAL(toggled(bool)),this,SLOT(on_LeeSW()));
    //connect(ui->chart,SIGNAL(toggled(bool)),this,SLOT(on_chart_curPosChanged(const QPolygonF &arg1));


    //Conectamos Slots del objeto "Tiva" con Slots de nuestra aplicacion (o con widgets)
    connect(&tiva,SIGNAL(statusChanged(int,QString)),this,SLOT(tivaStatusChanged(int,QString)));
    connect(&tiva,SIGNAL(messageReceived(uint8_t,QByteArray)),this,SLOT(messageReceived(uint8_t,QByteArray)));



}

MainUserGUI::~MainUserGUI() // Destructor de la clase
{
    delete ui;   // Borra el interfaz gráfico asociado a la clase
}

void MainUserGUI::on_serialPortComboBox_currentIndexChanged(const QString &arg1)
{
    activateRunButton();
}

// Funcion auxiliar de procesamiento de errores de comunicación
void MainUserGUI::processError(const QString &s)
{
    activateRunButton(); // Activa el botón RUN
    // Muestra en la etiqueta de estado la razón del error (notese la forma de pasar argumentos a la cadena de texto)
    ui->statusLabel->setText(tr("Status: Not running, %1.").arg(s));
}

// Funcion de habilitacion del boton de inicio/conexion
void MainUserGUI::activateRunButton()
{
    ui->runButton->setEnabled(true);
}


// SLOT asociada a pulsación del botón RUN
void MainUserGUI::on_runButton_clicked()
{
    //Intenta arrancar la comunicacion con la TIVA
    tiva.startRemoteLink( ui->serialPortComboBox->currentText());
}



//Slots Asociado al boton que limpia los mensajes del interfaz
void MainUserGUI::on_pushButton_clicked()
{
    ui->statusLabel->setText(tr(""));
}

void MainUserGUI::on_colorWheel_colorChanged(const QColor &arg1)
{
    //Poner aqui el codigo para pedirle al objeto "tiva" que envie el mensaje correspondiente
    MESSAGE_LED_Color_PARAMETER parameter;
    parameter.leds.red = arg1.red();
    parameter.leds.blue = arg1.blue();
    parameter.leds.green = arg1.green();
    qDebug() << "on_colorWheel_colorChanged  " << parameter.leds.red << "valor red" <<arg1.red() ;
    tiva.sendMessage(MESSAGE_LED_Color,QByteArray::fromRawData((char *)&parameter,sizeof(parameter)));
}

void MainUserGUI::on_Knob_valueChanged(double value)
{
    //Este mensaje se envia "mal" intencionadamente (no se corrsponde con lo que el microcontrolador pretende recibir)
    qDebug() << "on_Knob_valueChanged " << value;
    MESSAGE_LED_PWM_BRIGHTNESS_PARAMETER parameter;
    parameter.rIntensity = float(value);
    tiva.sendMessage(MESSAGE_LED_PWM_BRIGHTNESS,QByteArray::fromRawData((char *)&parameter,sizeof(parameter)));
}

void MainUserGUI::on_ADCButton_clicked()
{
//    qDebug() << "on_ADCButton_clicked"  ;
    tiva.sendMessage(MESSAGE_ADC_SAMPLE,NULL,0);
}

void MainUserGUI::on_LeeSW(void)
{
    MESSAGE_SW_PARAMETER parameter;

    parameter.sw.state = ui->LeeSW_check->isChecked();
    //qDebug() << "state value" << parameter.sw.state ;
    tiva.sendMessage(MESSAGE_SW,QByteArray::fromRawData((char *)&parameter,sizeof(parameter)));
}

void MainUserGUI::on_pingButton_clicked()
{
    tiva.sendMessage(MESSAGE_PING,NULL,0);
}

void MainUserGUI::cambiaLEDs(void)
{

    MESSAGE_LED_GPIO_PARAMETER parameter;

    parameter.leds.red=ui->rojo->isChecked();
    parameter.leds.green=ui->verde->isChecked();
    parameter.leds.blue=ui->azul->isChecked();

    tiva.sendMessage(MESSAGE_LED_GPIO,QByteArray::fromRawData((char *)&parameter,sizeof(parameter)));
}


// void MainUserGUI::cambiaACMs(void)
// {

// }
void MainUserGUI::cambiaMode(void)
{

    MESSAGE_Mode_PARAMETER parameter;

    parameter.Mode = ui->Mode_PWM_check->isChecked();
    tiva.sendMessage(MESSAGE_Mode_Cambio,QByteArray::fromRawData((char *)&parameter,sizeof(parameter)));
}






//**** Slot asociado a la recepción de mensajes desde la TIVA ********/
//Está conectado a una señale generada por el objeto TIVA de clase QTivaRPC (se conecta en el constructor de la ventana, más arriba en este fichero))
//Se pueden añadir los que casos que quieran para completar la aplicación

void MainUserGUI::messageReceived(uint8_t message_type, QByteArray datos)
{
    switch(message_type) // Segun el comando tengo que hacer cosas distintas
    {
        /** A PARTIR AQUI ES DONDE SE DEBEN AÑADIR NUEVAS RESPUESTAS ANTE LOS COMANDOS QUE SE ENVIEN DESDE LA TIVA **/
        case MESSAGE_PING:
        {   //Recepcion de la respuesta al ping desde la TIVA
            // Algunos comandos no tiene parametros --> No se extraen
            ventanaPopUp.setStyleSheet("background-color: lightgrey");
            ventanaPopUp.setModal(true); //CAMBIO: Se sustituye la llamada a exec(...) por estas dos.
            ventanaPopUp.show();
        }
        break;

        case MESSAGE_SW:
        {   //Recepcion de la respuesta al ping desde la TIVA
            // Algunos comandos no tiene parametros --> No se extraen
            MESSAGE_SW_PARAMETER parametro;
            if (check_and_extract_command_param(datos.data(), datos.size(), &parametro, sizeof(parametro))>0)
            {
                if(parametro.sw.number == 1)
                    ui->led1->setChecked((!ui->led1->isChecked()));
                if(parametro.sw.number == 2)
                    ui->led4->setChecked((!ui->led4->isChecked()));
            }
            else
                qDebug("fallo en  botton");
        }
        break;

        case MESSAGE_REJECTED:
        {   //Recepcion del mensaje MESSAGE_REJECTED (La tiva ha rechazado un mensaje que le enviamos previamente)
            MESSAGE_REJECTED_PARAMETER parametro;
            if (check_and_extract_command_param(datos.data(), datos.size(), &parametro, sizeof(parametro))>0)
            {
                // Muestra en una etiqueta (statuslabel) del GUI el mensaje
            ui->statusLabel->setText(tr("Status: Comando rechazado,%1").arg(parametro.command));
            }
            else
            {
                ui->statusLabel->setText(tr("Status: MSG %1, recibí %2 B y esperaba %3").arg(message_type).arg(datos.size()).arg(sizeof(parametro)));
            }
        }
        break;

        case MESSAGE_ACM_INT:
        {    // Este caso trata la recepcion de datos del ADC desde la TIVA
            MESSAGE_ACM_INT_PARAMETER parametro;
            qDebug() << "case MESSAGE_ADC_INT \r\n";
            if (check_and_extract_command_param(datos.data(), datos.size(), &parametro, sizeof(parametro))>0)
            {
                ui->led_GPIO0->setChecked(parametro.GPacm.GP0);
                ui->led_GPIO1->setChecked(parametro.GPacm.GP1);
                ui->led_GPIO2->setChecked(parametro.GPacm.GP2);
                ui->led_GPIO3->setChecked(parametro.GPacm.GP3);
            }
            else
            {   //Si el tamanho de los datos no es correcto emito la senhal statusChanged(...) para reportar un error
                ui->statusLabel->setText(tr("Status: MSG %1, recibí %2 B y esperaba %3").arg(message_type).arg(datos.size()).arg(sizeof(parametro)));
            }

        }
        break;

        case MESSAGE_ADC_SAMPLE:
        {    // Este caso trata la recepcion de datos del ADC desde la TIVA
            MESSAGE_ADC_SAMPLE_PARAMETER parametro;
   //         qDebug() << "case MESSAGE_ADC_SAMPLE \r\n";
            if (check_and_extract_command_param(datos.data(), datos.size(), &parametro, sizeof(parametro))>0)
            {
                ui->lcdCh1->display(((double)parametro.chan1)*3.3/4096.0);
                ui->lcdCh2->display(((double)parametro.chan2)*3.3/4096.0);
                ui->lcdCh3->display(((double)parametro.chan3)*3.3/4096.0);
                ui->lcdCh4->display((147.5 - (((double)parametro.chan4)* 75 * 3.3) /4096));//*3.3/4096.0);
    //            ui->lcdCh5->display(((double)parametro.chan4)*3.3/4096.0);
    //            ui->lcdCh6->display(((double)parametro.chan6)*3.3/4096.0);
   //         qDebug() << "case MESSAGE_ADC_SAMPLE dentro if \r\n";
            }
            else
            {   //Si el tamanho de los datos no es correcto emito la senhal statusChanged(...) para reportar un error
                ui->statusLabel->setText(tr("Status: MSG %1, recibí %2 B y esperaba %3").arg(message_type).arg(datos.size()).arg(sizeof(parametro)));
            }

        }
        break;
        case MESSAGE_ADC_LIVE:
        {

            MESSAGE_ADC_LIVE_PARAMETER parametro;
            if (check_and_extract_command_param(datos.data(), datos.size(), &parametro, sizeof(parametro))>0)
            {
                //ui->qwtPlot->detachItems(QwtPlotItem::Rtti_PlotCurve, true);
                // yyVal[0][j] = parametro.x;
                // yyVal[1][j] = parametro.y;
                // yyVal[2][j] = parametro.z;
                if (tj < 1024) {
                        xVal[tj]=tj;
                        //Los valores "constantes" deberian definirse como etiquetas por "estética"
                        yVal[0][tj]=3.3*(sin((double)tj*2.0*(valorknob1+1.0)*3.14159/1024.0)+1.0)/2.0;
                        yVal[1][tj]=3.3*(sin((double)tj*4.0*(valorknob1+1.0)*3.14159/1024.0)+1.0)/2.0;
                        yVal[2][tj]=3.3*(sin((double)tj*8.0*(valorknob1+1.0)*3.14159/1024.0)+1.0)/2.0;
                        yVal[3][tj]=3.3*(sin((double)tj*16.0*(valorknob1+1.0)*3.14159/1024.0)+1.0)/2.0;
                        yVal[4][tj]=3.3*(sin((double)tj*24.0*(valorknob1+1.0)*3.14159/1024.0)+1.0)/2.0;
                        yVal[5][tj]=3.3*(sin((double)tj*32.0*(valorknob1+1.0)*3.14159/1024.0)+1.0)/2.0;

                    ++tj;
                } else {
                    // Shift values to the left by 20 positions
                    for (int i = 0; i < 1024 - 200; ++i) {
                        yVal[0][i] = yVal[0][i + 200];
                        yVal[1][i] = yVal[1][i + 200];
                        yVal[2][i] = yVal[2][i + 200];
                        yVal[3][i] = yVal[3][i + 200];
                        yVal[4][i] = yVal[4][i + 200];
                        yVal[5][i] = yVal[5][i + 200];

                    }
                    tj = 1024 - 200 + 1;
                }
                ui->qwtPlot->replot();

                ui->led8->toggleState();

            }
        }
        break;

        case MESSAGE_B160_ACC:
        {

            MESSAGE_B160_ACC_PARAMETER parametro;
            if (check_and_extract_command_param(datos.data(), datos.size(), &parametro, sizeof(parametro))>0)
            {
                //ui->qwtPlot->detachItems(QwtPlotItem::Rtti_PlotCurve, true);
                // yyVal[0][j] = parametro.x;
                // yyVal[1][j] = parametro.y;
                // yyVal[2][j] = parametro.z;
                if (j < 1024) {
                    xxVal[j]=j;
                    yyVal[0][j] = parametro.x;
                    yyVal[1][j] = parametro.y;
                    yyVal[2][j] = parametro.z;
                    ++j;
                } else {
                    // Shift values to the left by 20 positions
                    for (int i = 0; i < 1024 - 20; ++i) {
                        yyVal[0][i] = yyVal[0][i + 20];
                        yyVal[1][i] = yyVal[1][i + 20];
                        yyVal[2][i] = yyVal[2][i + 20];
                    }
                    j = 1024 - 20 + 1;
                }
                ui->qwtPlot_acc->replot();



            }
            // qDebug() << "case MESSAGE_B160_ACC_PARAMETER" << parametro.x << " " << parametro.y << " " <<"\r\n" ;
        }
        break;
        case MESSAGE_B160_VELO:
        {
            MESSAGE_B160_VELO_PARAMETER parametro;
            if (check_and_extract_command_param(datos.data(), datos.size(), &parametro, sizeof(parametro))>0)
            {
                if (jj < 1024) {
                    r2Val[jj]=jj;
                    xyzVal[0][jj] = parametro.x;
                    xyzVal[1][jj] = parametro.y;
                    xyzVal[2][jj] = parametro.z;
                    ++jj;
                } else {
                    // Shift values to the left by 20 positions
                    for (int ii = 0; ii < 1024 - 20; ++ii) {
                        xyzVal[0][ii] = yyVal[0][ii + 20];
                        xyzVal[1][ii] = yyVal[1][ii + 20];
                        xyzVal[2][ii] = yyVal[2][ii + 20];
                    }
                    jj = 1024 - 20 + 1;
                }
                ui->qwtPlot_velo->replot();
            }
         qDebug() << "case MESSAGE_B160_VELO_PARAMETER " << parametro.x << "\r\n" ;
        }
        break;

        default:
            //Notifico que ha llegado un tipo de mensaje desconocido
            ui->statusLabel->setText(tr("Status: Recibido mensaje desconocido %1").arg(message_type));
        break; //Innecesario

        }
}

//Este Slot lo hemos conectado con la señal statusChange de la TIVA, que se activa cuando
//El puerto se conecta o se desconecta y cuando se producen determinados errores.
//Esta función lo que hace es procesar dichos eventos
void MainUserGUI::tivaStatusChanged(int status,QString message)
{
    switch (status)
    {
        case TivaRemoteLink::TivaConnected:

            //Caso conectado..
            // Deshabilito el boton de conectar
            ui->runButton->setEnabled(false);

            // Se indica que se ha realizado la conexión en la etiqueta 'statusLabel'
            ui->statusLabel->setText(tr("Ejecucion, conectado al puerto %1.").arg(ui->serialPortComboBox->currentText()));

            //   Y se habilitan los controles deshabilitados
            ui->pingButton->setEnabled(true);

        break;

        case TivaRemoteLink::TivaIsDisconnected:
            //Caso desconectado..
            // Rehabilito el boton de conectar
             ui->runButton->setEnabled(false);
             ui->statusLabel->setText(message);
        break;
        case TivaRemoteLink::FragmentedPacketError:
        case TivaRemoteLink::CRCorStuffError:
            //Errores detectados en la recepcion de paquetes
            ui->statusLabel->setText(message);
        break;
        default:
            //Otros errores (por ejemplo, abriendo el puerto)
            processError(tiva.getLastErrorMessage());
    }
}




void MainUserGUI::on_frecuencia_valueChanged(double value)
{
    //int i;
    valorknob1 = value;
    //Recalcula los valores de la grafica
    for(int i=0;i<1024;i++){
        //xVal[i]=i;
        //Los valores "constantes" deberian definirse como etiquetas por "estética"
        yVal[0][i]=3.3*(sin((double)i*2.0*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
        yVal[1][i]=3.3*(sin((double)i*4.0*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
        yVal[2][i]=3.3*(sin((double)i*8.0*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
        yVal[3][i]=3.3*(sin((double)i*16.0*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
        yVal[4][i]=3.3*(sin((double)i*8.0*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
        yVal[5][i]=3.3*(sin((double)i*16.0*(value+1.0)*3.14159/1024.0)+1.0)/2.0;

    }

  ui->qwtPlot->replot(); //Refresca la grafica una vez actualizados los valores
}




void MainUserGUI::cambiaACMs(void)
{

    MESSAGE_ACM_GPIO_PARAMETER parameter;

    parameter.GPacm.GP4=ui->ACMGPIO4->isChecked();
    parameter.GPacm.GP5=ui->ACMGPIO5->isChecked();
    parameter.GPacm.GP6=ui->ACMGPIO6->isChecked();
    parameter.GPacm.GP7=ui->ACMGPIO7->isChecked();


    tiva.sendMessage(MESSAGE_ACM_GPIO,QByteArray::fromRawData((char *)&parameter,sizeof(parameter)));
    qDebug() << "case MESSAGE_ACM_GPIO " << "\r\n" ;

}



void MainUserGUI::on_frecuencia_sample_valueChanged(double value)
{
    //Este mensaje se envia "mal" intencionadamente (no se corrsponde con lo que el microcontrolador pretende recibir)
    qDebug() << "on_frecuencia_sample_valueChanged " << value;
    MESSAGE_B160_FRQ_PARAMETER parameter;
    parameter.frq = float(value);
    tiva.sendMessage(MESSAGE_B160_FRQ,QByteArray::fromRawData((char *)&parameter,sizeof(parameter)));
}

