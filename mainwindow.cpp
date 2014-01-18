#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "generator.h"
#include <QThread>
#include <QTimer>
#include <qwt_plot_curve.h>
#include <qwt_scale_engine.h>
#include <QVector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_generation(false)
{
    ui->setupUi(this);
    m_generator = new Generator();
    m_thread = new QThread();
    m_generator->moveToThread(m_thread);
    connect(this, &MainWindow::generationAsked, m_generator, &Generator::generer);
    connect(m_generator, &Generator::finished, this, &MainWindow::reset);
    m_thread->start();
    ui->progression->setAxisTitle(QwtPlot::xBottom, "Nombre de lettres");
    ui->progression->setAxisTitle(QwtPlot::yLeft, "Ratio");
    ui->progression->enableAxis(QwtPlot::yRight);
    ui->progression->setAxisTitle(QwtPlot::yRight, "Différence");
    m_curve_ratio[0].attach(ui->progression);
    m_curve_ratio[1].attach(ui->progression);
    m_curve_ratio[0].setPen(Qt::red);
    m_curve_ratio[1].setPen(Qt::blue);
    m_curve_difference.setAxes(QwtPlot::xBottom, QwtPlot::yRight);
    m_curve_difference.attach(ui->progression);
}

MainWindow::~MainWindow()
{
    m_generator->stopGeneration();
    m_thread->quit();
    m_thread->wait(1000);
    delete m_thread;
    delete m_generator;
    delete ui;
}

void MainWindow::toogleGeneration()
{
    if (!m_generation)
    {
        m_generation = true;
        ui->generer->setText("Arrêter");
        ui->tailleLecture->setValue(0);
        ui->tailleLecture->setMaximum(ui->limiteLecture->value()*8);
        m_generator->init();
        m_generator->setLimitLecture(static_cast<unsigned long long>(ui->limiteLecture->value())*8ull*1024ull*1024ull);
        emit generationAsked();
        updateProgression();
    }
    else
    {
        ui->generer->setDisabled(true);
        ui->generer->setText("Arrêt en cours ...");
        m_generation = false;
        m_generator->stopGeneration();
    }
}

void MainWindow::updateProgression()
{
    if (!m_generation)
        return;

    unsigned long long a = m_generator->getCount(0);
    unsigned long long b = m_generator->getCount(1);
    unsigned long long total = a + b;

    if (total >= 10000)
    {
        m_ratio[0].append(QPointF(total, (double)a/total));
        m_ratio[1].append(QPointF(total, (double)b/total));
        m_difference.append(QPointF(total, static_cast<double>(qAbs(b-a))/total));

        /*if (m_ratio[0].size() >= 100)
        {
            m_ratio[0].pop_front();
            m_ratio[1].pop_front();
            m_difference.pop_front();
        }*/

        m_curve_ratio[0].setSamples(m_ratio[0]);
        m_curve_ratio[1].setSamples(m_ratio[1]);
        m_curve_difference.setSamples(m_difference);
        ui->progression->replot();
    }

    ui->tailleLecture->setValue(m_generator->getTailleLecture()/(1024*1024));

    QTimer::singleShot(100, this, SLOT(updateProgression()));
}

void MainWindow::reset()
{
    ui->generer->setText("Générer");
    ui->generer->setEnabled(true);
    m_generation = false;
    ui->tailleLecture->setValue(0);
    m_ratio[0].clear();
    m_ratio[1].clear();
    m_difference.clear();
}
