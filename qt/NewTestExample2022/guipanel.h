#ifndef GUIPANEL_H
#define GUIPANEL_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <QImage>
#include <qledmatrix.h>

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

namespace Ui {
class GUIPanel;
}

class GUIPanel : public QWidget
{
    Q_OBJECT
    
public:    
    explicit GUIPanel(QWidget *parent = 0);
    ~GUIPanel();
    
private slots:    
    void on_frecuencia_valueChanged(double value);
    void shiftMatrix();
    void on_Knob_2_sliderReleased();

private:
    Ui::GUIPanel *ui;
    int transactionCount;

    //SEMANA2: Para las graficas
    double xVal[1024]; //valores eje X
    double yVal[4][1024]; //valores ejes Y
    QwtPlotCurve *Channels[4]; //Curvas
    QwtPlotGrid  *m_Grid; //Cuadricula
    QImage *imagen;
    double scaleX;
    double scaleY;
    int currentShift;
    int maxShift;
    QScatterSeries *series1;
    QSplineSeries *series2;
    QLineSeries *series3;
    QLineSeries *series4;
    QAreaSeries *series5;
    QPolarChart *chart2;
    QChart *chart3;
    QTimer timer;


    qreal m_step;
    qreal m_x=0;
    qreal m_y=5;
    QSplineSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;

    QPieSeries *series6;
};

#endif // GUIPANEL_H
