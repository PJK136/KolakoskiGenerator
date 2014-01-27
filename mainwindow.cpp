#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "generator.h"
#include "binarygenerator.h"
#include "twolettersgenerator.h"
#include <QThread>
#include <QTimer>
#include <qwt_plot_curve.h>
#include <qwt_scale_engine.h>
#include <QVector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_generation(false),
    m_generator(0)
{
    ui->setupUi(this);
    qRegisterMetaType<std::vector<unsigned long long>>("std::vector<unsigned long long>");
    m_thread = new QThread();
    m_thread->start();
    ui->progression->setAxisTitle(QwtPlot::xBottom, "Nombre de lettres");
    ui->progression->setAxisTitle(QwtPlot::yLeft, "Ratio");
    ui->progression->enableAxis(QwtPlot::yRight);
    ui->progression->setAxisTitle(QwtPlot::yRight, "Ratio taille lecture");
    ui->difference->setAxisTitle(QwtPlot::xBottom, "Nombre de lettres");
    ui->difference->setAxisTitle(QwtPlot::yLeft, "Différence");
    ui->fonction->setAxisTitle(QwtPlot::xBottom, "Nombre de lettre 1");
    ui->fonction->setAxisTitle(QwtPlot::yLeft, "Nombre de lettre 2");
    m_curve_lecture.setAxes(QwtPlot::xBottom, QwtPlot::yRight);

    m_curve_fonction.setPen(Qt::blue);
    m_curve_theory.setPen(Qt::green);
    m_curve_min_lineaire.setPen(Qt::red);
    m_curve_max_lineaire.setPen(Qt::red);
    m_curve_min_affine.setPen(Qt::cyan);
    m_curve_max_affine.setPen(Qt::cyan);

    m_curve_lecture.setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve_difference.setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve_fonction.setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve_theory.setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve_min_lineaire.setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve_max_lineaire.setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve_min_affine.setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve_max_affine.setRenderHint(QwtPlotItem::RenderAntialiased);

    m_curve_lecture.attach(ui->progression);
    m_curve_difference.attach(ui->difference);
    m_curve_fonction.attach(ui->fonction);
    m_curve_theory.attach(ui->fonction);
    m_curve_min_lineaire.attach(ui->fonction);
    m_curve_max_lineaire.attach(ui->fonction);
    m_curve_min_affine.attach(ui->fonction);
    m_curve_max_affine.attach(ui->fonction);
}

MainWindow::~MainWindow()
{
    if (m_generator)
        m_generator->stopGeneration();
    m_thread->quit();
    m_thread->wait(1000);
    for (QwtPlotCurve *item : m_curve_ratio)
        delete item;
    delete m_thread;
    if (m_generator)
        delete m_generator;
    delete ui;
}

Qt::GlobalColor getColor(unsigned int nb)
{
    switch(nb%7)
    {
    case 0:
        return Qt::red;
        break;
    case 1:
        return Qt::blue;
        break;
    case 2:
        return Qt::green;
        break;
    case 3:
        return Qt::magenta;
        break;
    case 4:
        return Qt::yellow;
        break;
    case 5:
        return Qt::cyan;
        break;
    case 6:
        return Qt::black;
        break;
    default:
        return Qt::black;
        break;
    }
}

void MainWindow::toogleGeneration()
{
    if (!m_generation)
    {
        m_generation = true;
        ui->generer->setText("Arrêter");

        ui->tailleLecture->setValue(0);
        ui->tailleLecture->setMaximum(ui->limiteLecture->value()*8);

        if (m_generator)
            delete m_generator;

        if (ui->lettre_1->value() <= 2 && ui->lettre_2->value() <= 2)
            m_generator = new BinaryGenerator();
        else
            m_generator = new TwoLettersGenerator();

        m_generator->moveToThread(m_thread);
        connect(this, SIGNAL(generationAsked()), m_generator, SLOT(startGeneration()));
        connect(m_generator, &Generator::finished, this, &MainWindow::reset);
        connect(m_generator, &Generator::finishedOutput, this, &MainWindow::updateOutput);
        connect(m_generator, &Generator::progression, this, &MainWindow::updateProgression);

        m_generator->init(std::vector<unsigned char>({ui->lettre_1->value(),ui->lettre_2->value()}));
        m_generator->setLimitLecture(static_cast<unsigned long long>(ui->limiteLecture->value())*8ull*1024ull*1024ull);

        m_ratio.clear();
        resetPlots();
        m_min = 0;
        m_max = 0;
        for (QwtPlotCurve *item : m_curve_ratio)
            delete item;

        m_curve_ratio.clear();

        for (int i = 0; i < ui->nombreLettres->value(); i++)
        {
            m_ratio.push_back(QVector<double>());
            m_curve_ratio.push_back(new QwtPlotCurve(QString("Lettre ") + QString::number(i)));
            m_curve_ratio[i]->setPen(getColor(i));
            m_curve_ratio[i]->setRenderHint(QwtPlotItem::RenderAntialiased);
            m_curve_ratio[i]->attach(ui->progression);
        }

        m_temps.start();
        emit generationAsked();
    }
    else
    {
        ui->generer->setDisabled(true);
        ui->generer->setText("Arrêt en cours ...");
        m_generation = false;
        m_generator->stopGeneration();
    }
}

void MainWindow::triggerUpdateProgression()
{
    m_generator->getProgression();
}

void MainWindow::updateProgression(std::vector<unsigned long long> counts)
{
    if (!m_generation)
        return;

    unsigned long long total = 0;
    for (unsigned int i = 0; i < counts.size()-1; i++)
        total += counts[i];

    m_min = qMin(m_min, static_cast<long long>(counts[1]-counts[0]));
    m_max = qMax(m_max, static_cast<long long>(counts[1]-counts[0]));

    m_lettres_generees.append(total);

    for (unsigned int i = 0; i < counts.size()-1; i++)
        m_ratio[i].append((double)counts[i]/total);

    m_ratio_lecture.append((double)counts.back()/total);
    m_difference.append((double)counts[1]-counts[0]);
    m_counts.append(QPointF(counts[0], counts[1]));

    for (unsigned int i = 0; i < counts.size()-1; i++)
        m_curve_ratio[i]->setSamples(m_lettres_generees, m_ratio[i]);

    m_curve_lecture.setSamples(m_lettres_generees, m_ratio_lecture);
    m_curve_difference.setSamples(m_lettres_generees, m_difference);

    /*m_curve_fonction.setSamples(m_counts);
    m_curve_theory.setSamples({QPointF(m_counts.first().x(),m_counts.first().x()), QPointF(counts[0], counts[0])});
    m_curve_min_lineaire.setSamples({QPointF(0,0), QPointF(counts[0], counts[0]+m_min)});
    m_curve_max_lineaire.setSamples({QPointF(0,0), QPointF(counts[0], counts[0]+m_max)});
    m_curve_min_affine.setSamples({QPointF(m_counts.first().x(),m_counts.first().x()+m_min), QPointF(counts[0], counts[0]+m_min)});
    m_curve_max_affine.setSamples({QPointF(m_counts.first().x(),m_counts.first().x()+m_max), QPointF(counts[0], counts[0]+m_max)});*/

    ui->progression->replot();
    ui->difference->replot();
    ui->fonction->replot();

    ui->tailleLecture->setValue(m_generator->getTailleLecture()/(1024*1024));
    int elapsed = m_temps.elapsed()/1000;
    ui->temps->setText(QString("Temps : %1:%2:%3").arg(QString::number(elapsed/3600), 2, QLatin1Char('0'))
                                                  .arg(QString::number((elapsed/60)%60), 2, QLatin1Char('0'))
                                                  .arg(QString::number(elapsed%60), 2, QLatin1Char('0')));
    if (elapsed)
        ui->vitesse->setText(QString("Vitesse : %1 L/s").arg(QLocale(QLocale::French).toString(total/elapsed)));
    QTimer::singleShot(ui->rafraichissement->value(), this, SLOT(triggerUpdateProgression()));
}

void MainWindow::updateOutput(unsigned char *output)
{
    QString sortie = "";
    for (; *output; output++)
    {
        sortie += QString::number(*output);
    }

    ui->sortie->setPlainText(sortie);
}

void MainWindow::resetPlots()
{
    m_lettres_generees.clear();
    for (QVector<double> &item : m_ratio)
        item.clear();
    m_difference.clear();
    m_ratio_lecture.clear();
    m_counts.clear();
}

void MainWindow::reset()
{
    ui->generer->setText("Générer");
    ui->generer->setEnabled(true);
    m_generation = false;
    ui->tailleLecture->setValue(0);
    resetPlots();
}
