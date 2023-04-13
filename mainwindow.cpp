#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QFileDialog>
#include <QImage>
#include<cmath>
#include <algorithm>
#include "convolution_filters.h"
#include "function_filters.h"
#include "dithering.h"
#include "octree.h"
#include "histogram.h"
#include <fstream>
#include <QFile>
#include <QTextStream>



//MainWindow constructor and destructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateFilterField();

}
MainWindow::~MainWindow()
{
    delete ui;
}


//Load, Save, Reset, Expand
void MainWindow::on_loadFileButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Images (*.png *.jpeg *.jpg)"));

    //load image, set variables
    QImage img(file_name);
    QPixmap pixmap = QPixmap::fromImage(img);
    this->modifiedPixmap = pixmap;
    this->originalPixmap = pixmap;

    //resize to screen size and showstd:vecttor width heigth
    int w = ui->originalImage->width();
    int h = ui->originalImage->height();
    ui->originalImage ->setPixmap(this->originalPixmap);//.scaled(w,h, Qt::KeepAspectRatio));
    ui->modifiedImage ->setPixmap(this->modifiedPixmap);//.scaled(w,h, Qt::KeepAspectRatio));
}

void MainWindow::on_saveFileButton_clicked(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), QDir::homePath());
    QPixmap pixmap= this->modifiedPixmap;
    if(!pixmap.isNull()) pixmap.save(filename, "jpg");
}

void MainWindow::on_resetButton_clicked()
{
    this->modifiedPixmap = this->originalPixmap;
    int w = ui->originalImage->width();
    int h = ui->originalImage->height();
    ui->modifiedImage->setPixmap(this->originalPixmap);//.scaled(w,h, Qt::KeepAspectRatio));
}

void MainWindow::on_expandImageButton_clicked()
{
    this->modifiedPixmap = ConvolutionFilters::pad(this->modifiedPixmap,2,2,1,1);
    this->update(this->modifiedPixmap);
}


// GUI utilities, custom kernel filters
void MainWindow::update(QPixmap pixmap){
    int w = ui->modifiedImage->width();
    int h = ui->modifiedImage->height();
    ui->modifiedImage->setPixmap(pixmap);//.scaled(w,h, Qt::KeepAspectRatio));
}

//non-standard filters
void MainWindow::updateFilterField(){
    QLayoutItem *item = NULL;
    while ((item = ui->kernelGridLayout->takeAt(0)) != 0) {
        delete item->widget();
    }
    int x = ui->kernelXSpinBox->value();
    int y = ui->kernelYSpinBox->value();
    for(int i = 0; i < x; i++){
        for(int j =0; j < y; j++){
            QSpinBox *item = new QSpinBox(this);
            item->setValue(1);
            item->setMinimum(-99);
            item->setMaximum(99);
            ui->kernelGridLayout->addWidget(item, i,j);
        }
    }
}

void MainWindow::on_kernelXSpinBox_valueChanged(int arg1)
{
    updateFilterField();
}

void MainWindow::on_kernelYSpinBox_valueChanged(int arg1)
{
    updateFilterField();
}

std::vector<std::vector<int>> MainWindow::filterFieldToVector()
{
    int filter_x = ui->kernelXSpinBox->value();
    int filter_y = ui->kernelYSpinBox->value();
    std::vector<std::vector<int>> filter(filter_x, std::vector<int> (filter_y, 0));

    for(int i = 0; i < filter_x; i++){
        for(int j = 0; j< filter_y; j++){
            QLayoutItem *item  = ui->kernelGridLayout->itemAtPosition(i,j);
            if (!item || !item->widget()){
                continue;
            }
            QSpinBox *entry= qobject_cast<QSpinBox*>(item->widget());
            if (entry) {
              filter[i][j] = entry->value();
            }
        }
    }
    return filter;
}

void MainWindow::vectorToFilterField(std::vector<std::vector<int>> filter){
    QLayoutItem *item = NULL;
    while ((item = ui->kernelGridLayout->takeAt(0)) != 0) {
        delete item->widget();
    }
    int x = filter.size();
    int y = filter[0].size();
    for(int i = 0; i < x; i++){
        for(int j =0; j < y; j++){
            QSpinBox *item = new QSpinBox(this);
            item->setMinimum(-99);
            item->setMaximum(99);
            item->setValue(filter[i][j]);;
            ui->kernelGridLayout->addWidget(item, i,j);
        }
    }
}


void MainWindow::on_applyCustomConvolution_clicked()
{
    int anchor_x = ui->anchorXSpinBox->value();
    int anchor_y = ui->anchorYSpinBox->value();
    double bias = ui->biasSpinBox->value();
    double weight = ui->weightSpinBox->value();
    std::vector<std::vector<int>> filter = filterFieldToVector();
    this->modifiedPixmap=  ConvolutionFilters::convolve2d(this->modifiedPixmap, filter, weight, bias, anchor_x,  anchor_y);
    this->update(this->modifiedPixmap);
}


void MainWindow::on_saveFilterButton_clicked()
{
    std::vector<std::vector<int>> filter = filterFieldToVector();
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), QDir::homePath());
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream<<ui->weightSpinBox->value()<<"\n";
        stream<<ui->biasSpinBox->value()<<"\n";
        stream<<ui->anchorXSpinBox->value()<<"\n";
        stream<<ui->anchorYSpinBox->value()<<"\n";
       for (auto& row : filter) {
            for (auto col : row){
              stream << col <<',';
            }
            stream << '\n';
        }
    }
}

void MainWindow::on_loadFilterButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("Text (*.txt)"));
    QFile file(filename);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        std::vector<std::vector<int>> filter;
        QTextStream in(&file);
        //weight
        QString line = in.readLine();
        ui->weightSpinBox->setValue(line.toDouble());

        //bias
        line = in.readLine();
        ui-> biasSpinBox->setValue(line.toDouble());

        //anchor x y
        line = in.readLine();
        ui-> anchorXSpinBox->setValue(line.toDouble());
        line = in.readLine();
        ui-> anchorYSpinBox->setValue(line.toDouble());

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList values = line.split(",");
            std::vector<int> row;
            for (const QString& value : values) {
                row.push_back(value.toInt());
                qDebug()<<value.toInt();
            }
            row.pop_back();
            filter.push_back(row);
        }
        ui->kernelXSpinBox->setValue(filter.size());
        ui->kernelYSpinBox->setValue(filter[0].size());
        vectorToFilterField(filter);
    }
}

void MainWindow::on_computeWeightButton_clicked()
{
    std::vector <std::vector <int> > filter = filterFieldToVector();
    qDebug()<<ConvolutionFilters::computeDivisor(filter);
    ui->weightSpinBox->setValue(ConvolutionFilters::computeDivisor(filter));
}



//convolution filters
void MainWindow::on_actionGaussian_blur_triggered()
{
    double bias = ui->biasSpinBox->value();
    std::vector <std::vector<int>> filter = {{1,2,1}, {2,4,2}, {1,2,1}};
    this->modifiedPixmap=  ConvolutionFilters::convolve2d(this->modifiedPixmap, filter, 16, bias, 1,1);
    this->update(this->modifiedPixmap);
}


void MainWindow::on_actionMean_Blur_triggered()
{
    double bias = ui->biasSpinBox->value();
    std::vector <std::vector<int>> filter = {{1,1,1}, {1,1,1}, {1,1,1}};
    this->modifiedPixmap = ConvolutionFilters::convolve2d(this->modifiedPixmap, filter, 9.0, bias, 1,1);
    this->update(this->modifiedPixmap);
}


void MainWindow::on_actionEdge_Preserving_Blur_triggered()
{
    std::vector <std::vector <int> > filter(7, std::vector<int>(7, 1));
    this->modifiedPixmap=  ConvolutionFilters::convolve2d_v2(this->modifiedPixmap, filter, 0.0, 3,3);
    this->update(this->modifiedPixmap);
}


void MainWindow::on_actionEdge_Detection_triggered()
{
    std::vector <std::vector<int>> filter = {{0,-1,0}, {0,1,0}, {0,0,0}};
    this->modifiedPixmap=  ConvolutionFilters::convolve2d(this->modifiedPixmap, filter, 0.2, 0.0, 1,1);
    this->update(this->modifiedPixmap);
}


void MainWindow::on_actionSharpen_triggered()
{
    std::vector <std::vector<int>> filter = {{0,-1,0}, {-1,5,-1}, {0,-1,0}};
    this->modifiedPixmap=  ConvolutionFilters::convolve2d(this->modifiedPixmap, filter,1, 0.0, 1,1);
    this->update(this->modifiedPixmap);
}


void MainWindow::on_actionEmboss_triggered()
{
    std::vector <std::vector<int>> filter = {{-1,-1,0}, {-1,1,1}, {0,1,1}};
    this->modifiedPixmap=  ConvolutionFilters::convolve2d(this->modifiedPixmap, filter, 1, 0.0, 1,1);
    this->update(this->modifiedPixmap);
}



//Functional filters
void MainWindow::on_actionInversion_triggered()
{
    this->modifiedPixmap = FunctionFilters::applyFilter(this->modifiedPixmap, FunctionFilters::inversion);
    this->update(this->modifiedPixmap);
}


void MainWindow::on_actionBrightness_Correction_triggered()
{
    this->modifiedPixmap = FunctionFilters::applyFilter(this->modifiedPixmap, FunctionFilters::brightness);
    this->update(this->modifiedPixmap);
}


void MainWindow::on_actionContrast_triggered()
{
    this->modifiedPixmap = FunctionFilters::applyFilter(this->modifiedPixmap, FunctionFilters::contrast);
    this->update(this->modifiedPixmap);
}

void MainWindow::on_actionGamma_triggered()
{
    this->modifiedPixmap = FunctionFilters::applyFilter(this->modifiedPixmap, FunctionFilters::gamma);
    this->update(this->modifiedPixmap);
}


//Dithering, grayscale conversion
void MainWindow::on_actionRandom_Dithering_triggered()
{
    this->modifiedPixmap=  Dithering::randomDithering(this->modifiedPixmap, ui->colorsPerChannelSpinBox->value());
    this->update(this->modifiedPixmap);
}


void MainWindow::on_actionLumosity_triggered()
{
    this->modifiedPixmap=  FunctionFilters::rgbToGrayscale(this->modifiedPixmap);
    this->update(this->modifiedPixmap);
}


void MainWindow::on_actionoctree_triggered()
{
    int N = ui->ColorSpinBox->value();
    octree *tree = new octree(this->modifiedPixmap, N);
    this->modifiedPixmap = tree->colorQuantization(this->modifiedPixmap);
    this->update(this->modifiedPixmap);
}


void MainWindow::on_actionHistogram_stretching_triggered()
{
    this->modifiedPixmap=  Histogram::histogramStretching(this->modifiedPixmap);
    this->update(this->modifiedPixmap);
}


void MainWindow::on_actionHistogram_equalization_triggered()
{
    this->modifiedPixmap=  Histogram::histogramEqualization(this->modifiedPixmap);
    this->update(this->modifiedPixmap);

}

