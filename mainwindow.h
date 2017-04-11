#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "generator.h"
#include <QMainWindow>
#include <QThread>
#include <QTime>
#include <qwt_plot_curve.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void toogleGeneration();
    void triggerUpdateProgression();
    void updateOutput(unsigned char *output);
    void resetPlots();

signals:
    void generationAsked();

protected slots:
    void reset();
    void updateProgression(std::vector<unsigned long long>);

private:
    Ui::MainWindow *ui;
    QThread *m_thread;
    Generator *m_generator;
    std::vector<unsigned char> m_lettres;
    QVector<double> m_lettres_generees;
    QVector<QVector<double>> m_ratio;
    QVector<QwtPlotCurve*> m_curve_ratio;
    QVector<QVector<double>> m_difference;
    QVector<QwtPlotCurve*> m_curve_difference;
    QVector<double> m_ratio_lecture;
    QwtPlotCurve m_curve_lecture;
    QVector<QPointF> m_counts;
    long long m_min;
    long long m_max;
    QTime m_temps;
    QLocale m_locale;
    bool m_generation;
};

#endif // MAINWINDOW_H
