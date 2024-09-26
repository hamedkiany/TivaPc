#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->m_knoba,SIGNAL(valueChanged(double)),this, SLOT (on_any_knob_change(double)));
    connect(ui->m_knob_b,SIGNAL(valueChanged(double)),this, SLOT (on_any_knob_change(double)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_m_boton_clicked()
{
    ui->led->setChecked((!ui->led->isChecked()));
    qDebug() << "boton pushed";
}

void  MainWindow::on_any_knob_change(double value)
{
    double value_a = ui->m_knoba->value();
    double value_b = ui->m_knob_b->value();
    ui->manometer->setValue(2 * (value_a +1 ) * (value_b + 1));
    qDebug() << "chaneged knob" << value_a << "ddd" <<value_b;
}

