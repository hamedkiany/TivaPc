#ifndef MAINUSERGUI_H
#define MAINUSERGUI_H

#include <QWidget>
#include <QtSerialPort/qserialport.h>
#include <QMessageBox>
#include "tiva_remotelink.h"

//new 2
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <QImage>
#include <qledmatrix.h>
#include <QTime>
#include <QTimer>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPolarChart>
#include <QtCore/QDebug>

#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

QT_USE_NAMESPACE

#include <QLayoutItem>
//new

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
//end for new chart

namespace Ui {
class MainUserGUI;
}

class MainUserGUI : public QWidget
{
    Q_OBJECT
    
public: 
    explicit MainUserGUI(QWidget *parent = 0);
    ~MainUserGUI();
    
private slots:
    // slots privados asociados mediante "connect" en el constructor
    void cambiaLEDs();
    void cambiaACMs();
    void cambiaMode();
    void on_LeeSW();
    void tivaStatusChanged(int status,QString message);
    void messageReceived(uint8_t type, QByteArray datos);

    //Slots asociados por nombre
    void on_runButton_clicked();    
    void on_serialPortComboBox_currentIndexChanged(const QString &arg1);
    void on_pushButton_clicked();    
    void on_colorWheel_colorChanged(const QColor &arg1);
    void on_Knob_valueChanged(double value);
    void on_ADCButton_clicked();
    void on_pingButton_clicked();
    void on_frecuencia_valueChanged(double value);


//    void on_chartview_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

//    void on_Mi_chartview_curPosChanged(const QPolygonF &arg1);

//    void on_chart_curPosChanged(const QPolygonF &arg1);


    void on_frecuencia_sample_valueChanged(double value);

private:
    // funciones privadas
    void processError(const QString &s);
    void activateRunButton();

private:
    //Componentes privados
    Ui::MainUserGUI *ui;
    int transactionCount;
    QMessageBox ventanaPopUp;
    TivaRemoteLink tiva; //Objeto para gestionar la comunicacion de mensajes con el microcontrolador

    //SEMANA2: Para las graficas
    double xVal[1024]; //valores eje X
    double yVal[6][1024]; //valores ejes Y
    QwtPlotCurve *Channels[4]; //Curvas
    QwtPlotGrid  *m_Grid; //Cuadricula
    double valorknob1;
    // QImage *imagen;
    // double scaleX;
    // double scaleY;
    // int currentShift;
    // int maxShift;
    // QScatterSeries *series1;
    // QSplineSeries *series2;
    // QLineSeries *series3;
    // QLineSeries *series4;
    // QAreaSeries *series5;
    // QPolarChart *chart2;
    // QChart *chart3;
    // QTimer timer;


    // qreal m_step;
    // qreal m_x=0;
    // qreal m_y=5;
    // QSplineSeries *m_series;
    // QValueAxis *m_axisX;
    // QValueAxis *m_axisY;

    // QPieSeries *series6;


    //accelarcion: Para las graficas
    double xxVal[1024]; //valores eje X
    double yyVal[3][1024]; //valores ejes Y
    QwtPlotCurve *Channels2[4]; //Curvas
    QwtPlotGrid  *m_Grid2; //Cuadricula

    //velocidad: Para las graficas
    double r2Val[1024]; //valores eje X
    double xyzVal[3][1024]; //valores ejes Y
    QwtPlotCurve *Channels3[4]; //Curvas
    QwtPlotGrid  *m_Grid3; //Cuadricula


};

#endif // GUIPANEL_H
