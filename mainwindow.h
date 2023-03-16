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

    void on_inversionButton_clicked();

    void on_brightnessButton_clicked();

    void on_contrastButton_clicked();

    void on_resetButton_clicked();

    void on_gammaButton_clicked();

    void on_gaussianBlurButton_clicked();

    void on_saveFileButton_clicked();

    void on_expandImageButton_clicked();

    void on_meanBlurButton_clicked();

    void on_edgeDetectionButton_clicked();

    void on_sharpenButton_clicked();

    void on_embossButton_clicked();

    void updateFilterField();

    void on_kernelXSpinBox_valueChanged(int arg);

    void on_kernelYSpinBox_valueChanged(int arg1);

    void on_applyCustomConvolution_clicked();

    void on_saveFilterButton_clicked();

    std::vector<std::vector<int>> filterFieldToVector();

    void vectorToFilterField(std::vector<std::vector<int>> filter);

    void on_loadFilterButton_clicked();

    void on_labBlurButton_clicked();

    void on_computeWeightButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
