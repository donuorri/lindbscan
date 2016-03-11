/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** mainwindow.cpp
**
**
** Author: Sergio Monteleone    sergio.monteleone@unipa.it
** Author: Gabriella Giordano   gabriella.giordano@unipa.it
** -------------------------------------------------------------------------*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QList>
#include <QHash>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <qmath.h>
#include <QFileDialog>
#include <QElapsedTimer>
#include <QGraphicsEllipseItem>
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <QDialogButtonBox>

#include "common.h"
#include "clustergraphicsitem.h"
#include "noisegraphicsitem.h"

#include "FFDBC.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gridItem = NULL;

    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    // load test datasets
    QDirIterator it("./datasets/", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString itemPath = it.next();
        if(itemPath.endsWith(".") || itemPath.endsWith(".."))
            continue;

        QString label = itemPath;
        ui->inputDSCombobox->addItem(label.remove("./datasets/"), itemPath);
    }

    ui->inputDSCombobox->setCurrentIndex(0);

    // progress bar
    pBar = new QProgressBar(this);
    ui->statusBar->addWidget(pBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::on_clusteringProgress
 * \param progress
 * \param maximum
 */
void MainWindow::on_clusteringProgress(int progress, int maximum)
{
    pBar->setMaximum(maximum);
    pBar->setValue(progress);
}

/*!
 * \brief MainWindow::runClusteringBenchmark
 */
void MainWindow::runClusteringBenchmark()
{
    // Reset progressbar
    pBar->setValue(0);
    ui->settingsDockWidget->setEnabled(false);

    QString fileName = ui->inputDSCombobox->itemData(ui->inputDSCombobox->currentIndex()).toString();
    points = loader.loadFromFile(fileName);

    if(points)
    {
        FFDBC* ffdbcThread = NULL;

        ffdbcThread = new FFDBC(ui->minptsSpinBox->value(), ui->gammaSpinBox->value(), points, this);

        connect(ffdbcThread, SIGNAL(clusteringProgress(int,int)), this, SLOT(on_clusteringProgress(int,int)));
        connect(ffdbcThread, SIGNAL(clusteringCompleted()), this, SLOT(on_clusteringCompleted()));

        timer.start();

        ffdbcThread->start();
    }
}

/*!
 * \brief MainWindow::drawDataset
 */
void MainWindow::on_clusteringCompleted()
{
    qint64 elapsedTimeInMs = timer.elapsed();

    qDebug() << QString::number(points->size()) << " points processed in " << QString::number(elapsedTimeInMs) << " ms.";

    FFDBC* cThread = qobject_cast<FFDBC*>(sender());

    if(!cThread) return;

    QList<Cluster *> clusters = cThread->getClusters();
    qreal _gamma = cThread->getGamma();

    ui->settingsDockWidget->setEnabled(true);

    // calculate performance indices
    if(ui->indexesCheckBox->checkState() == Qt::Checked)
    {
        // calculate entropy and purity
        qreal entropy = 0.0f;
        qreal purity = 0.0f;
        for (int i=0; i<clusters.size(); i++)
        {
            QList<Point*> cluster = clusters.at(i)->Points;
            QHash<QString, int> classes;

            for (int j=0; j<cluster.size(); j++)
            {
                QString key = cluster.at(j)->desiredClusterID;
                if (classes.contains(key))
                    classes.insert(key, classes[key] + 1);
                else
                    classes.insert(key, 1);
            }

            QList<int> vals = classes.values();
            qreal entropy_i = 0.0f; // entropy value for the i-th cluster
            for (int k=0; k<vals.size(); k++)
            {
                qreal probability = vals.at(k) / (qreal)cluster.size();
                entropy_i += probability * (-1.0) * (qLn(probability) / qLn(2)) ; // prob * log_2 prob
            }
            entropy += entropy_i * cluster.size();

            // for each cluster found get the maximum value
            // of items that matches the same class.
            qSort(vals.begin(), vals.end());
            purity += (vals.last());
        }
        entropy = entropy / (qreal)points->size();
        purity = purity / (qreal)points->size();

        int a=0, b=0, c=0, d=0;

        for(int i=0; i < points->size(); i++)
        {
            for(int k=(i+1); k < points->size(); k++)
            {
                Point* p1 = points->at(i);
                Point* p2 = points->at(k);

                if (!p1->desiredClusterID.isEmpty() && !p2->desiredClusterID.isEmpty())
                {
                    bool bothInMyCluster = (p1->clusterID == p2->clusterID);
                    bool bothInDesiredCluster = (p1->desiredClusterID == p2->desiredClusterID);

                    if(bothInMyCluster && bothInDesiredCluster) a++;
                    if(bothInMyCluster && !bothInDesiredCluster) b++;
                    if(!bothInMyCluster && bothInDesiredCluster) c++;
                    if(!bothInMyCluster && !bothInDesiredCluster) d++;
                }
            }
        }

        float N = points->size();
        float M = (N * (N - 1)) / 2.0f;

        float rand = (a + d) / M;
        float precision = a / (float)(a + b);
        float recall = a / (float)(a + c);
        float fscore = 2.0f * (precision * recall) / (precision + recall);
        float jaccard = a / (float)(a + b + c);
        float folkes = a / qSqrt((qreal)(a + b) * (qreal)(a + c));

        QStringList indexesMessage;
        indexesMessage << "<table cellspacing=\"10\">";
        indexesMessage << "<col align=\"left\">";
        indexesMessage << "<col align=\"right\">";
        indexesMessage << "<tr><th>Index</th><th>Score</th></tr>";
        indexesMessage << "<tr><td>Precision</td><td>" << QString::number(precision, 'f', 4)<< "</td></tr>";
        indexesMessage << "<tr><td>Recall</td><td>" << QString::number(recall, 'f', 4) << "</td></tr>";
        indexesMessage << "<tr><td>F-score</td><td>" << QString::number(fscore, 'f', 4) << "</td></tr>";
        indexesMessage << "<tr><td>Rand statistics</td><td>" << QString::number(rand, 'f', 4) << "</td></tr>";
        indexesMessage << "<tr><td>Jaccard</td><td>" << QString::number(jaccard, 'f', 4) << "</td></tr>";
        indexesMessage << "<tr><td>Folkes & Mallows</td><td>" << QString::number(folkes, 'f', 4) << "</td></tr>";
        indexesMessage << "</table>";

        QMessageBox* msgBox = new QMessageBox(this);
        msgBox->setAttribute(Qt::WA_DeleteOnClose); //makes sure the msgbox is deleted automatically when closed
        msgBox->setStandardButtons( QMessageBox::Ok);
        msgBox->setWindowTitle( tr("Validation indexes"));
        msgBox->setText(indexesMessage.join(""));
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setModal( false ); // if you want it non-modal
        msgBox->setMinimumWidth(500);
        msgBox->show();
    }

    // Draw scene
    scene->clear();

    QBrush backBrush(Qt::white);
    scene->setBackgroundBrush(backBrush);

    float penSize =  ui->pensizeSpinBox->value();

    NoiseGraphicsItem* noiseItem = new NoiseGraphicsItem(*points, Qt::black, penSize);
    scene->addItem(noiseItem);

    for(int i=0; i< clusters.size(); i++)
    {
        Cluster* cluster = clusters.at(i);

        QColor color = QColor::fromHsv(i * 359 / clusters.size(), 200, 190 + (i * 65 / clusters.size()));

        ClusterGraphicsItem* item = new ClusterGraphicsItem(cluster, color, _gamma, penSize, (ui->drawConvexHullCheckBox->isChecked()));
        scene->addItem(item);
    }

    QRectF sceneRectangle = scene->itemsBoundingRect();

    gridItem = new GridGraphicsItem(_gamma, sceneRectangle);
    gridItem->setVisible(ui->drawGridCheckBox->checkState() == Qt::Checked);
    scene->addItem(gridItem);

    // fit view on screen
    ui->graphicsView->resetTransform();
    ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    // flip vertically
    QTransform transform;
    transform.rotate(180, Qt::XAxis);
    ui->graphicsView->setTransform(transform, true);

    // cleanup
    delete points;
}

void MainWindow::on_runButton_clicked()
{
    runClusteringBenchmark();
}


void MainWindow::on_drawGridCheckBox_toggled(bool checked)
{
    if(gridItem != NULL)
    {
        gridItem->setVisible(checked);
    }
}

void MainWindow::on_aboutButton_clicked()
{
    QStringList aboutHTMLText;
    aboutHTMLText << "<h2>Flood Fill Density-Based Clustering</h2>";
    aboutHTMLText << "<p>An algorithm for approximation of density-based clustering by space quantization on geospatial datasets.</p>";

    aboutHTMLText << "<h3>Usage</h3>";
    aboutHTMLText << "<p>Select an input dataset, a value for Gamma and MinPts and click on Run.";
    aboutHTMLText << "Change options to modify the renderding mode and calculate validation indexes.</p>";

    QMessageBox::about(this, "About FFDBC", aboutHTMLText.join(' '));
}
