#include "convolution_filters.h"
#include <QDebug>
#include <QSaveFile>
#include <QFile>
#include <QFileDialog>
#include <numeric>

ConvolutionFilters::ConvolutionFilters(){}

//function filters
QPixmap ConvolutionFilters::applyFilter(QPixmap pixmap, int (*func)(int)){
    int w = pixmap.width();
    int h = pixmap.height();
    QImage img = pixmap.toImage();
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            QColor pixel(img.pixel(x, y));
            int r = func(pixel.red());
            int g = func(pixel.green());
            int b = func(pixel.blue());
            img.setPixel(x, y, qRgb(r,g,b));
        }
    }
    return QPixmap::fromImage(img);
}

int ConvolutionFilters::inversion(int value){
    return std::clamp(255-value, 0,255);
}
int ConvolutionFilters::brightness(int value){
    int brightness = 10;
    return std::clamp(value+brightness, 0, 255);
}
int ConvolutionFilters::contrast(int value){
    double b = 0.2;
    return std::clamp(int(value+(value-128)*b), 0, 255);
}
int ConvolutionFilters::gamma(int value){
    double gamma = 0.95;
    return std::clamp(int(255*pow(value/255.0, gamma)), 0, 255);
}
int ConvolutionFilters::black(int value){
    return 0;
}

int ConvolutionFilters::computeDivisor(std::vector<std::vector<int>> filter){
    int divisor = 0;
    for(int x = 0 ; x < filter.size(); x++){
        for(int y = 0; y< filter[0].size(); y++){
            divisor+=filter[x][y];
        }
    }
    return divisor;
}

QPixmap ConvolutionFilters::pad(QPixmap pixmap, int padding_left, int padding_top, int padding_right, int padding_bottom){
    //create new image
    int width = pixmap.width();
    int heigth = pixmap.height();
    int new_width = width + padding_right + padding_left;
    int new_heigth = heigth + padding_top + padding_bottom;


    QImage origin = pixmap.toImage();
    QImage result = QImage(new_width, new_heigth, QImage::Format_RGB32);
    result.fill(Qt::white);

    //copy previous image
    for (int x = 0; x < width; x++) {
        for(int y = 0; y < heigth; y++){
            QColor clrCurrent(origin.pixel(x, y));
            int r = clrCurrent.red();
            int g = clrCurrent.green();
            int b = clrCurrent.blue();
            result.setPixel(x+padding_left, y+padding_top, qRgb(r, g, b));
        }
    }

    //copy image on borders
    for(int x = padding_left; x < new_width-padding_right; x++){
        for(int y = 1; y < padding_top+1; y++){
            result.setPixel(x, padding_top-y, result.pixel(x,padding_top));
        }
        for(int y = 1; y< padding_bottom+1;y++){
            result.setPixel(x, new_heigth-y, result.pixel(x,new_heigth-padding_bottom-1));
        }
    }
    for(int y = 0; y < new_heigth; y++){
        for(int x = 1; x < padding_left+1; x++){
            result.setPixel(padding_left-x, y, result.pixel(padding_left,y));
        }
        for(int x = 1; x < padding_left+1; x++){
            result.setPixel(new_width-x, y, result.pixel(new_width-padding_right-1,y));
        }
    }
    return QPixmap::fromImage(result);
}

QPixmap ConvolutionFilters::convolve2d(QPixmap &pixmap, std::vector<std::vector<int>> filter, double divisor, double bias, int anchor_x, int anchor_y) {
    int w = pixmap.width();
    int h = pixmap.height();
    int filter_x = filter[0].size();
    int filter_y = filter.size();
    int padding_top = anchor_y;
    int padding_left = anchor_x;
    int padding_right = filter_x - anchor_x - 1;
    int padding_bottom = filter_y - anchor_y - 1;

    QPixmap expanded = ConvolutionFilters::pad(pixmap, padding_left, padding_top, padding_right, padding_bottom);
    QImage expanded_img = expanded.toImage();
    QImage img = pixmap.toImage();

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int sum_r = 0;
            int sum_g = 0;
            int sum_b = 0;
            for (int i = 0; i < filter_y; i++) {
                for (int j = 0; j < filter_x; j++) {
                    int p = i - anchor_y;
                    int q = j - anchor_x;
                    QColor pixel(expanded_img.pixel(x + q + padding_left, y + p + padding_top));
                    sum_r += filter[i][j] * pixel.red();
                    sum_g += filter[i][j] * pixel.green();
                    sum_b += filter[i][j] * pixel.blue();
                }
            }
            sum_r = std::clamp(int(sum_r / divisor + bias), 0, 255);
            sum_g = std::clamp(int(sum_g / divisor + bias), 0, 255);
            sum_b = std::clamp(int(sum_b / divisor + bias), 0, 255);
            img.setPixel(x, y, qRgb(sum_r, sum_g, sum_b));
        }
    }
    return QPixmap::fromImage(img);
}


QPixmap ConvolutionFilters::convolve2d_v2(QPixmap &pixmap, std::vector<std::vector<int>> filter, double bias, int anchor_x, int anchor_y) {
    int M = 100;
    int w = pixmap.width();
    int h = pixmap.height();
    int filter_x = filter[0].size();
    int filter_y = filter.size();
    int padding_top = anchor_y;
    int padding_left = anchor_x;
    int padding_right = filter_x - anchor_x - 1;
    int padding_bottom = filter_y - anchor_y - 1;
    int divisor = 0;

    QPixmap expanded = ConvolutionFilters::pad(pixmap, padding_left, padding_top, padding_right, padding_bottom);
    QImage expanded_img = expanded.toImage();
    QImage img = pixmap.toImage();

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int sum_r = 0;
            int sum_g = 0;
            int sum_b = 0;
            for (int i = 0; i < filter_y; i++) {
                for (int j = 0; j < filter_x; j++) {
                    int p = i - anchor_y;
                    int q = j - anchor_x;
                    QColor pixel(expanded_img.pixel(x + q + padding_left, y + p + padding_top)); //considered pixel
                    QColor center(expanded_img.pixel(x,y));
                    int d = abs(pixel.red()-center.red()) + abs(pixel.green() - center.green()) + abs(pixel.blue() - center.blue());
                    if(d<M){
                        sum_r += filter[i][j] * pixel.red();
                        sum_g += filter[i][j] * pixel.green();
                        sum_b += filter[i][j] * pixel.blue();
                        divisor+=filter[i][j];
                    }
                }
            }
            sum_r = std::clamp(int(sum_r / divisor + bias), 0, 255);
            sum_g = std::clamp(int(sum_g / divisor + bias), 0, 255);
            sum_b = std::clamp(int(sum_b / divisor + bias), 0, 255);
            img.setPixel(x, y, qRgb(sum_r, sum_g, sum_b));
            divisor = 0;
        }
    }
    return QPixmap::fromImage(img);
}
