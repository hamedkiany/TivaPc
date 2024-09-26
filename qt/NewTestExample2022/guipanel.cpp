#include "guipanel.h"
#include "ui_guipanel.h"
#include <QMessageBox>      // Se deben incluir cabeceras a los componentes que se vayan a crear en la clase
// y que no estén incluidos en el interfaz gráfico. En este caso, la ventana de PopUp <QMessageBox>
// que se muestra al recibir un PING de respuesta

#include<stdint.h>      // Cabecera para usar tipos de enteros con tamaño
#include<stdbool.h>     // Cabecera para usar booleanos


#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>


#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPolarChart>
#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>



QT_USE_NAMESPACE

GUIPanel::GUIPanel(QWidget *parent) :  // Constructor de la clase
    QWidget(parent),
    ui(new Ui::GUIPanel)               // Indica que guipanel.ui es el interfaz grafico de la clase
  , transactionCount(0)
{
    ui->setupUi(this);                // Conecta la clase con su interfaz gráfico.
    setWindowTitle(tr("Ejemplo Plot")); // Título de la ventana


    //Semana 2. Inicializacion GRAFICA
    ui->Grafica->setTitle("Sinusoides"); // Titulo de la grafica
    ui->Grafica->setAxisTitle(QwtPlot::xBottom, "Tiempo"); // Etiqueta eje X de coordenadas
    ui->Grafica->setAxisTitle(QwtPlot::yLeft, "Valor");    // Etiqueta eje Y de coordenadas
    //ui->Grafica->axisAutoScale(true); // Con Autoescala
    ui->Grafica->setAxisScale(QwtPlot::yLeft, 0, 3.3); // Escala fija
    ui->Grafica->setAxisScale(QwtPlot::xBottom,0,1024.0);

    // Formateo de la curva
    for(int i=0;i<4;i++){
    Channels[i] = new QwtPlotCurve();
    Channels[i]->attach(ui->Grafica);
    }

    //Inicializacion de los valores básicos
    for(int i=0;i<1024;i++){
            xVal[i]=i;
            yVal[0][i]=3.3*(sin((double)i*2.0*3.14159/1024.0)+1.0)/2.0;
            yVal[1][i]=3.3*(sin((double)i*4.0*3.14159/1024.0)+1.0)/2.0;
            yVal[2][i]=3.3*(sin((double)i*8.0*3.14159/1024.0)+1.0)/2.0;
            yVal[3][i]=3.3*(sin((double)i*16.0*3.14159/1024.0)+1.0)/2.0;
    }
    Channels[0]->setRawSamples(xVal,yVal[0],1024);
    Channels[1]->setRawSamples(xVal,yVal[1],1024);
    Channels[2]->setRawSamples(xVal,yVal[2],1024);
    Channels[3]->setRawSamples(xVal,yVal[3],1024);

    Channels[0]->setPen(QPen(Qt::red)); // Color de la curva
    Channels[1]->setPen(QPen(Qt::cyan));
    Channels[2]->setPen(QPen(Qt::yellow));
    Channels[3]->setPen(QPen(Qt::green));
    m_Grid = new QwtPlotGrid();     // Rejilla de puntos
    m_Grid->attach(ui->Grafica);    // que se asocia al objeto QwtPl
    ui->Grafica->setAutoReplot(false); //Desactiva el autoreplot (mejora la eficiencia)
    //Semana 2. FIN Inicializacion GRAFICA

    //Para matriz de leds
     imagen = new QImage(":/HelloWorld.png");

     currentShift=-ui->ledMatrix->columnCount();

     //QTimer *timer2 = new QTimer(ui->grafica);
     connect(&timer, SIGNAL(timeout()), this, SLOT(shiftMatrix())); // Enlazamos la llegada del timer a su cuenta (50ms) con la
                                                                  // ejecución de la función "drawValue"
     timer.start(50); // El timer cuenta eventos periódicos de 50ms


     QBarSet *set0 = new QBarSet("Jane");
        QBarSet *set1 = new QBarSet("John");
        QBarSet *set2 = new QBarSet("Axel");
        QBarSet *set3 = new QBarSet("Mary");
        QBarSet *set4 = new QBarSet("Samantha");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6;
        *set1 << 5 << 0 << 0 << 4 << 0 << 7;
        *set2 << 3 << 5 << 8 << 13 << 8 << 5;
        *set3 << 5 << 6 << 7 << 3 << 4 << 5;
        *set4 << 9 << 7 << 5 << 3 << 1 << 2;
    //![1]

    //![2]
        QBarSeries *series = new QBarSeries();
        series->append(set0);
        series->append(set1);
        series->append(set2);
        series->append(set3);
        series->append(set4);

    //![2]

    //![3]
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Simple barchart example");
        chart->setAnimationOptions(QChart::SeriesAnimations);
    //![3]

    //![4]
        QStringList categories;
        categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        axisY->setRange(0,15);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
    //![4]

    //![5]
        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);
    //![5]
        ui->chartview->setChart(chart);
        ui->chartview->setRenderHint(QPainter::Antialiasing);

        //Polar

        const qreal angularMin = -100;
            const qreal angularMax = 100;

            const qreal radialMin = -100;
            const qreal radialMax = 100;

            series1 = new QScatterSeries();
            series1->setName("scatter");
            for (int i = angularMin; i <= angularMax; i += 10)
                series1->append(i, (i / radialMax) * radialMax + 8.0);

            series2 = new QSplineSeries();
            series2->setName("spline");
            for (int i = angularMin; i <= angularMax; i += 10)
                series2->append(i, (i / radialMax) * radialMax);

            series3 = new QLineSeries();
            series3->setName("star outer");
            qreal ad = (angularMax - angularMin) / 8;
            qreal rd = (radialMax - radialMin) / 3 * 1.3;
            series3->append(angularMin, radialMax);
            series3->append(angularMin + ad*1, radialMin + rd);
            series3->append(angularMin + ad*2, radialMax);
            series3->append(angularMin + ad*3, radialMin + rd);
            series3->append(angularMin + ad*4, radialMax);
            series3->append(angularMin + ad*5, radialMin + rd);
            series3->append(angularMin + ad*6, radialMax);
            series3->append(angularMin + ad*7, radialMin + rd);
            series3->append(angularMin + ad*8, radialMax);

            series4 = new QLineSeries();
            series4->setName("star inner");
            ad = (angularMax - angularMin) / 8;
            rd = (radialMax - radialMin) / 3;
            series4->append(angularMin, radialMax);
            series4->append(angularMin + ad*1, radialMin + rd);
            series4->append(angularMin + ad*2, radialMax);
            series4->append(angularMin + ad*3, radialMin + rd);
            series4->append(angularMin + ad*4, radialMax);
            series4->append(angularMin + ad*5, radialMin + rd);
            series4->append(angularMin + ad*6, radialMax);
            series4->append(angularMin + ad*7, radialMin + rd);
            series4->append(angularMin + ad*8, radialMax);

            series5 = new QAreaSeries();
            series5->setName("star area");
            series5->setUpperSeries(series3);
            series5->setLowerSeries(series4);
            series5->setOpacity(0.5);

            //![1]
            chart2 = new QPolarChart();
            //![1]
            chart2->addSeries(series1);
            chart2->addSeries(series2);
            chart2->addSeries(series3);
            chart2->addSeries(series4);
            chart2->addSeries(series5);

            chart2->setTitle("Polar graph");

            //![2]
            QValueAxis *angularAxis = new QValueAxis();
            angularAxis->setTickCount(9); // First and last ticks are co-located on 0/360 angle.
            angularAxis->setLabelFormat("%.1f");
            angularAxis->setShadesVisible(true);
            angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
            chart2->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);

            QValueAxis *radialAxis = new QValueAxis();
            radialAxis->setTickCount(9);
            radialAxis->setLabelFormat("%d");
            chart2->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
            //![2]

            series1->attachAxis(radialAxis);
            series1->attachAxis(angularAxis);
            series2->attachAxis(radialAxis);
            series2->attachAxis(angularAxis);
            series3->attachAxis(radialAxis);
            series3->attachAxis(angularAxis);
            series4->attachAxis(radialAxis);
            series4->attachAxis(angularAxis);
            series5->attachAxis(radialAxis);
            series5->attachAxis(angularAxis);

            radialAxis->setRange(radialMin, radialMax);
            angularAxis->setRange(angularMin, angularMax);

            ui->chartview_3->setChart(chart2);
            ui->chartview_3->setRenderHint(QPainter::Antialiasing);


            //Spline

            chart3 = new QChart;
            chart3->setTitle("Dynamic spline chart");
            chart3->legend()->hide();
            //chart3->setAnimationOptions(QChart::AllAnimations);

            m_series=new QSplineSeries;
            QPen green(Qt::red);
            green.setWidth(3);
            m_series->setPen(green);
            QStringList m_titles;
            m_axisX=new QValueAxis();
            m_axisY=new QValueAxis();

            m_x=0;
            m_y=5;
            m_series->append(m_x, m_y);
            chart3->addSeries(m_series);
            chart3->addAxis(m_axisX,Qt::AlignBottom);
            chart3->addAxis(m_axisY,Qt::AlignLeft);
            m_series->attachAxis(m_axisX);
            m_series->attachAxis(m_axisY);
            m_axisX->setTickCount(5);
            m_axisX->setRange(0, 100);
            m_axisY->setRange(-5, 10);

            ui->chartview_2->setChart(chart3);
            ui->chartview_2->setRenderHint(QPainter::Antialiasing);


            //Pie chart
            series6 = new QPieSeries();
                series6->append("Jane", 1);
                series6->append("Joe", 2);
                series6->append("Andy", 3);
                series6->append("Barbara", 4);
                series6->append("Axel", 5);
            //![1]

            //![2]
                QPieSlice *slice = series6->slices().at(1);
                slice->setExploded();
                slice->setLabelVisible();
                slice->setPen(QPen(Qt::darkGreen, 2));
                slice->setBrush(Qt::green);
            //![2]

            //![3]
                QChart *chart4 = new QChart();
                chart4->addSeries(series6);
                chart4->setTitle("Simple piechart example");
                chart4->legend()->hide();
                chart4->setAnimationOptions(QChart::AllAnimations);
            //![3]

            //![4]
                ui->chartview_4->setChart(chart4);
                ui->chartview_4->setRenderHint(QPainter::Antialiasing);
            //![4]



}



GUIPanel::~GUIPanel() // Destructor de la clase
{
    delete ui;   // Borra el interfaz gráfico asociado a la clase
}

void GUIPanel::shiftMatrix()
{

    //LED MATRIX
    if (currentShift>ui->ledMatrix->columnCount())
    {
        currentShift=-ui->ledMatrix->columnCount();
    }
    for(int row=0; row < ui->ledMatrix->rowCount(); ++row)
    {
        for(int col=0; col < ui->ledMatrix->columnCount(); ++col)
        {
            int checkcol=col+currentShift;
            // Funciona porque se evalua en orden...
            if ((checkcol>=0) && (checkcol<ui->ledMatrix->columnCount())&&(imagen->pixel(checkcol, row) == 0xFFFFFFFF))
            {
                // White pixels will be shown as red LEDs
                ui->ledMatrix->setColorAt(row, col, QLedMatrix::Red);
            }
            else
            {
                // Other pixels shown as dark LEDs
                ui->ledMatrix->setColorAt(row, col, QLedMatrix::NoColor);
            }
        }
    }
    currentShift++;


    //Spline

        qreal x = chart3->plotArea().width() / 100.0;
        //qreal y = (m_axisX->max() - m_axisX->min()) / m_axisX->tickCount();
            m_x += 1;
            m_y = QRandomGenerator::global()->bounded(5) - 2.5;
            m_series->append(m_x, m_y);
            if (m_x>100)
            {
                m_series->remove(0);
                if ((int)m_x%50==1) chart3->scroll(50.0*x,0);
                //m_axisX->setRange(0, 100);
                       //chart3->scroll(x, 0);
            }
}


//SEMANA2: Slot asociado a la rosca "frecuencia"
void GUIPanel::on_frecuencia_valueChanged(double value)
{
    int i;

    //Recalcula los valores de la grafica
    for(int i=0;i<1024;i++){
            //xVal[i]=i;
        //Los valores "constantes" deberian definirse como etiquetas por "estética"
            yVal[0][i]=3.3*(sin((double)i*2.0*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
            yVal[1][i]=3.3*(sin((double)i*4.0*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
            yVal[2][i]=3.3*(sin((double)i*8.0*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
            yVal[3][i]=3.3*(sin((double)i*16.0*(value+1.0)*3.14159/1024.0)+1.0)/2.0;
    }
    ui->Grafica->replot(); //Refresca la grafica una vez actualizados los valores
}

void GUIPanel::on_Knob_2_sliderReleased()
{
    QPieSlice *slice = series6->slices().at(1);
    slice->setValue(ui->Knob_2->value());
}
