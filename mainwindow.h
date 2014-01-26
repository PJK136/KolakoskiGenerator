#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <qwt_plot_curve.h>
#include "generator.h"
#include <QTime>

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
    void updateOutput(char *output);
    void resetPlots();

signals:
    void generationAsked();

protected slots:
    void reset();
    void updateProgression(std::vector<unsigned long long>);

private:
    Ui::MainWindow *ui;
    bool m_generation;
    QThread *m_thread;
    Generator *m_generator;
    QVector<double> m_lettres_generees;
    QVector<QVector<double>> m_ratio;
    QVector<QwtPlotCurve*> m_curve_ratio;
    QVector<double> m_difference;
    QwtPlotCurve m_curve_difference;
    QVector<double> m_ratio_lecture;
    QwtPlotCurve m_curve_lecture;
    QVector<QPointF> m_counts;
    QwtPlotCurve m_curve_fonction;
    long long m_min;
    long long m_max;
    QwtPlotCurve m_curve_theory;
    QwtPlotCurve m_curve_min_lineaire;
    QwtPlotCurve m_curve_max_lineaire;
    QwtPlotCurve m_curve_min_affine;
    QwtPlotCurve m_curve_max_affine;
    QTime m_temps;
};

#endif // MAINWINDOW_H
