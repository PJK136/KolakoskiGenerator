#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <qwt_plot_curve.h>
#include "generator.h"

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
    void updateProgression();

signals:
    void generationAsked();

protected slots:
    void reset();

private:
    Ui::MainWindow *ui;
    bool m_generation;
    QThread *m_thread;
    Generator *m_generator;
    QVector<QPointF> m_ratio[2];
    QwtPlotCurve m_curve_ratio[2];
    QVector<QPointF> m_difference;
    QwtPlotCurve m_curve_difference;
};

#endif // MAINWINDOW_H
