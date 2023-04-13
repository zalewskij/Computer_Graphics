#include "function_filters.h"

FunctionFilters::FunctionFilters(){}
    //function filters
QPixmap FunctionFilters::applyFilter(QPixmap pixmap, int (*func)(int)){
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

int FunctionFilters::inversion(int value){
    return std::clamp(255-value, 0,255);
}
int FunctionFilters::brightness(int value){
    int brightness = 10;
    return std::clamp(value+brightness, 0, 255);
}
int FunctionFilters::contrast(int value){
    double b = 0.2;
    return std::clamp(int(value+(value-128)*b), 0, 255);
}
int FunctionFilters::gamma(int value){
    double gamma = 0.95;
    return std::clamp(int(255*pow(value/255.0, gamma)), 0, 255);
}
int FunctionFilters::black(int value){
    return 0;
}
QPixmap FunctionFilters::rgbToGrayscale(QPixmap pixmap){
    QImage img = pixmap.toImage();
    QImage result = img.convertToFormat(QImage::Format_Grayscale8);
    return QPixmap::fromImage(result);

//    int w = pixmap.width();
//    int h = pixmap.height();
//    QImage img = pixmap.toImage();
//    QImage result = QImage(w,h, QImage::Format_Grayscale8);
//    for (int x = 0; x < w; ++x) {
//        for (int y = 0; y < h; ++y) {
//            QColor pixel(img.pixel(x, y));
//            result.setPixel(x,y, 0.3 * pixel.red() + 0.59 * pixel.green() + 0.11 * pixel.blue());
//        }
//    }
//    return QPixmap::fromImage(result);
}
