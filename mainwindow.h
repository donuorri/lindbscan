/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** mainwindow.h
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QProgressBar>
#include <QElapsedTimer>

#include "common.h"
#include "gridgraphicsitem.h"
#include "pointsloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void logMessage(const QString &msg);

private slots:
    void on_clusteringProgress(int progress, int maximum);
    void on_clusteringCompleted();
    void on_runButton_clicked();
    void on_drawGridCheckBox_toggled(bool checked);
    void on_aboutButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    GridGraphicsItem* gridItem;
    QProgressBar* pBar;
    PointsLoader loader;
    qreal epsilon;
    QElapsedTimer timer;

    QList<Point*>* points;

    void runClusteringBenchmark();
};

#endif // MAINWINDOW_H
