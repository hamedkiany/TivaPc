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
    ui->ledA->setChecked((!ui->ledA->isChecked()));
    qDebug() << "boton pushed";
}

void  MainWindow::on_any_knob_change(double value)
{
    double value_a = ui->m_knoba->value();
    //double value_b = ui->m_knob_b->value();
    //ui->manometer->setValue(2 * (value_a +1 ) * (value_b + 1));
    if (value_a > 5)
        ui->ledB->setChecked(true);
    else
        ui->ledB->setChecked(false);


    qDebug() << "chaneged knob" << value_a ;
}


void MainWindow::on_checkBox1_stateChanged(int arg1)
{
    if(ui->checkBox1->isChecked() and ui->checkBox2->isChecked() and ui->checkBox3->isChecked())
        ui->ledA->setChecked(true);
    else
        ui->ledA->setChecked(false);
        //qDebug() << "check box 1" ;
}


void MainWindow::on_checkBox2_stateChanged(int arg1)
{
    if(ui->checkBox2->isChecked()  and ui->checkBox2->isChecked() and ui->checkBox3->isChecked())
        ui->ledA->setChecked(true);
    else
        ui->ledA->setChecked(false);
    //qDebug() << "check box 1" ;

}


void MainWindow::on_checkBox3_stateChanged(int arg1)
{
    if(ui->checkBox3->isChecked() and ui->checkBox2->isChecked()and ui->checkBox3->isChecked())
        ui->ledA->setChecked(true);
    else
        ui->ledA->setChecked(false);
    //qDebug() << "check box 1" ;

}

