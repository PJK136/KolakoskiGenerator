#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "generator.h"
#include "binarygenerator.h"
#include "twolettersgenerator.h"
#include "multilettersgenerator.h"
#include <QThread>
#include <QTimer>
#include <qwt_plot_curve.h>
#include <qwt_scale_engine.h>
#include <qwt_legend.h>
#include <QVector>
#include <QRegExp>
#include <sstream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_generator(0),
    m_locale(QLocale::French),
    m_generation(false)
{
    ui->setupUi(this);

    ui->lettres->setValidator(new QRegExpValidator(QRegExp("[1-9 ]+")));
    ui->lettres->setText("1 2");

    qRegisterMetaType<std::vector<unsigned long long>>("std::vector<unsigned long long>");
    m_thread = new QThread();
    m_thread->start();
    ui->progression->setAxisTitle(QwtPlot::xBottom, "Nombre de lettres");
    ui->progression->setAxisTitle(QwtPlot::yLeft, "Ratio");
    ui->progression->enableAxis(QwtPlot::yRight);
    ui->progression->setAxisTitle(QwtPlot::yRight, "Ratio taille lecture");
    ui->progression->insertLegend(new QwtLegend, QwtPlot::BottomLegend);
    ui->difference->setAxisTitle(QwtPlot::xBottom, "Nombre de lettres");
    ui->difference->setAxisTitle(QwtPlot::yLeft, "Différence");
    ui->difference->insertLegend(new QwtLegend, QwtPlot::BottomLegend);
    m_curve_lecture.setTitle("Différence lecture-suite");
    m_curve_lecture.setAxes(QwtPlot::xBottom, QwtPlot::yRight);
    m_curve_lecture.setRenderHint(QwtPlotItem::RenderAntialiased);
    m_curve_lecture.attach(ui->progression);
}

MainWindow::~MainWindow()
{
    if (m_generator)
        m_generator->stopGeneration();
    m_thread->quit();
    m_thread->wait(1000);
    for (QwtPlotCurve *item : m_curve_ratio)
        delete item;
    for (QwtPlotCurve *item : m_curve_difference)
        delete item;
    delete m_thread;
    if (m_generator)
        delete m_generator;
    delete ui;
}

Qt::GlobalColor getColor(unsigned int nb)
{
    switch(nb%14)
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
        return Qt::gray;
        break;
    case 7:
        return Qt::darkRed;
        break;
    case 8:
        return Qt::darkBlue;
        break;
    case 9:
        return Qt::darkMagenta;
        break;
    case 10:
        return Qt::darkYellow;
        break;
    case 11:
        return Qt::darkCyan;
        break;
    case 12:
        return Qt::darkGray;
        break;
    case 13:
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
        std::stringstream stream(ui->lettres->text().toStdString());
        m_lettres.clear();
        unsigned int lettre = 0;
        while (stream >> lettre)
        {
            if (lettre > 255)
            {
                QMessageBox::critical(this, "Lettre trop grande", "Une lettre ne peut pas dépasser 255 ... (Limitation technique)");
                return;
            }
            m_lettres.push_back(lettre);
        }

        if (m_lettres.empty())
        {
            QMessageBox::critical(this, "Pas de lettres ...", "Aucune lettre n'a été saisie pour la génération de la suite !");
            return;
        }
        else if (m_lettres.size() == 1)
        {
            QMessageBox::critical(this, "Pas assez de lettres ...", "Il faut au moins deux lettres différentes pour la génération de la suite !");
            return;
        }
        else if (m_lettres.size() > 255)
        {
            QMessageBox::critical(this, "Pas assez de lettres ...", "Il faut au moins deux lettres différentes pour la génération de la suite !");
            return;
        }

        for (unsigned int i = 0; i < m_lettres.size()-1; i++)
        {
            if (m_lettres[i] == m_lettres[i+1])
            {
                QMessageBox::critical(this, "Lettres identiques ...", "Il y a deux fois la même lettre à la suite dans la saisie !");
                return;
            }
        }

        if (m_lettres.front() == m_lettres.back())
        {
            QMessageBox::critical(this, "Lettres identiques ...", "La première lettre ne peut pas être identique à la dernière !");
            return;
        }

        m_generation = true;
        ui->generer->setText("Arrêter");

        ui->tailleLecture->setValue(0);

        if (m_generator)
            delete m_generator;

        if (m_lettres.size() == 2)
        {
            if (m_lettres[0] <= 2 && m_lettres[1] <= 2)
                m_generator = new BinaryGenerator();
            else
                m_generator = new TwoLettersGenerator();
            m_generator->setLimitLecture(static_cast<unsigned long long>(ui->limiteLecture->value())*8ull*1024ull*1024ull);
            ui->tailleLecture->setMaximum(ui->limiteLecture->value()*8);
        }
        else
        {
            m_generator = new MultiLettersGenerator();
            m_generator->setLimitLecture(static_cast<unsigned long long>(ui->limiteLecture->value())*1024ull*1024ull);
            ui->tailleLecture->setMaximum(ui->limiteLecture->value());
        }

        m_generator->moveToThread(m_thread);
        connect(this, SIGNAL(generationAsked()), m_generator, SLOT(startGeneration()));
        connect(m_generator, &Generator::finished, this, &MainWindow::reset);
        connect(m_generator, &Generator::finishedOutput, this, &MainWindow::updateOutput);
        connect(m_generator, &Generator::progression, this, &MainWindow::updateProgression);

        m_generator->init(m_lettres);

        m_ratio.clear();
        resetPlots();
        m_min = 0;
        m_max = 0;
        for (QwtPlotCurve *item : m_curve_ratio)
            delete item;

        for (QwtPlotCurve *item : m_curve_difference)
            delete item;

        m_curve_ratio.clear();
        m_curve_difference.clear();

        for (unsigned int i = 0; i < m_lettres.size(); i++)
        {
            m_ratio.push_back(QVector<double>());
            m_curve_ratio.push_back(new QwtPlotCurve(QString("Lettre ") + QString::number(m_lettres[i])));
            m_curve_ratio[i]->setPen(getColor(i));
            m_curve_ratio[i]->setRenderHint(QwtPlotItem::RenderAntialiased);
            m_curve_ratio[i]->attach(ui->progression);
        }

        for (unsigned int i = 0; i < m_lettres.size()-1; i++)
        {
            m_difference.push_back(QVector<double>());
            m_curve_difference.push_back(new QwtPlotCurve(QString("Lettre ") + QString::number(m_lettres[i+1]) + QString(" - ") + QString::number(m_lettres[i])));
            m_curve_difference[i]->setPen(getColor(i+1));
            m_curve_difference[i]->setRenderHint(QwtPlotItem::RenderAntialiased);
            m_curve_difference[i]->attach(ui->difference);
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
    m_generator->askProgression();
}

void MainWindow::updateProgression(std::vector<unsigned long long> counts)
{
    if (!m_generation)
        return;

    int elapsed = m_temps.elapsed();

    unsigned long long total = 0;
    for (unsigned int i = 0; i < counts.size()-1; i++)
        total += counts[i];

    m_lettres_generees.append(total);

    for (unsigned int i = 0; i < counts.size()-1; i++)
        m_ratio[i].append((double)counts[i]/total);

    for (unsigned int i = 0; i < counts.size()-2; i++)
    {
        m_difference[i].append((double)counts[i+1]-counts[i]);
        m_min = qMin(m_min, static_cast<long long>(m_difference[i].last()));
        m_max = qMax(m_max, static_cast<long long>(m_difference[i].last()));
    }

    if (counts.back())
        m_ratio_lecture.append((double)counts.back()/total);
    m_counts.append(QPointF(counts[0], counts[1]));

    for (unsigned int i = 0; i < counts.size()-1; i++)
        m_curve_ratio[i]->setSamples(m_lettres_generees, m_ratio[i]);

    for (unsigned int i = 0; i < counts.size()-2; i++)
        m_curve_difference[i]->setSamples(m_lettres_generees, m_difference[i]);

    m_curve_lecture.setSamples(m_lettres_generees, m_ratio_lecture);

    ui->progression->replot();
    ui->difference->replot();

    ui->tailleLecture->setValue(m_generator->getTailleLecture()/(1024*1024));

    if (elapsed)
        ui->vitesse->setText(QString("Vitesse : %1 L/s").arg(m_locale.toString(qFloor(((double)total/elapsed)*1000.))));
    else
        ui->vitesse->setText("Vitesse : N/A L/s");

    elapsed /= 1000;
    ui->temps->setText(QString("Temps : %1:%2:%3").arg(QString::number(elapsed/3600), 2, QLatin1Char('0'))
                                                  .arg(QString::number((elapsed/60)%60), 2, QLatin1Char('0'))
                                                  .arg(QString::number(elapsed%60), 2, QLatin1Char('0')));

    ui->min->setText(QString("Min : %1").arg(m_locale.toString(m_min)));
    ui->max->setText(QString("Max : %1").arg(m_locale.toString(m_max)));

    QString ratio = "Ratio :\n";
    for (unsigned int i = 0; i < counts.size()-1; i++)
        ratio += QString("L") + QString::number(m_lettres[i]) + QString(": ") + QString::number(m_ratio[i].last(), 'f', 14) + QString('\n');
    ui->ratio->setText(ratio);
    QTimer::singleShot(ui->rafraichissement->value(), this, SLOT(triggerUpdateProgression()));
}

void MainWindow::updateOutput(unsigned char *output)
{
    QString sortie = "";
    for (; *output; output++)
    {
        if (*output > 9 && *output < 36)
            sortie += *output - 10 + 65;
        else
            sortie += QString::number(*output);
    }

    ui->sortie->setPlainText(sortie);
}

void MainWindow::resetPlots()
{
    m_lettres_generees.clear();
    for (QVector<double> &item : m_ratio)
        item.clear();
    for (QVector<double> &item : m_difference)
        item.clear();
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
