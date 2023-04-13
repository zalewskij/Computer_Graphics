#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QPixmap originalPixmap;
    QPixmap modifiedPixmap;
    void update(QPixmap pixmap);


private slots:
    void on_loadFileButton_clicked();

    void on_resetButton_clicked();

    void on_saveFileButton_clicked();

    void on_expandImageButton_clicked();

    void updateFilterField();

    void on_kernelXSpinBox_valueChanged(int arg);

    void on_kernelYSpinBox_valueChanged(int arg1);

    void on_applyCustomConvolution_clicked();

    void on_saveFilterButton_clicked();

    std::vector<std::vector<int>> filterFieldToVector();

    void vectorToFilterField(std::vector<std::vector<int>> filter);

    void on_loadFilterButton_clicked();

    void on_computeWeightButton_clicked();

    void on_actionRandom_Dithering_triggered();

    void on_actionLumosity_triggered();

    void on_actionoctree_triggered();

    void on_actionGaussian_blur_triggered();

    void on_actionMean_Blur_triggered();

    void on_actionEdge_Preserving_Blur_triggered();

    void on_actionEdge_Detection_triggered();

    void on_actionSharpen_triggered();

    void on_actionEmboss_triggered();

    void on_actionInversion_triggered();

    void on_actionBrightness_Correction_triggered();

    void on_actionContrast_triggered();

    void on_actionGamma_triggered();

    void on_actionHistogram_stretching_triggered();

    void on_actionHistogram_equalization_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
