#ifndef FUNCTIONFILTERS_H
#define FUNCTIONFILTERS_H

#include <QImage>
#include <vector>
#include <QPixmap>


class FunctionFilters
{
public:
    FunctionFilters();
    static QPixmap applyFilter(QPixmap pixmap, int (*func)(int));
    static QPixmap rgbToGrayscale(QPixmap pixmap);
    static int inversion(int value);
    static int brightness(int value);
    static int contrast(int value);
    static int gamma(int value);
    static int black(int value);
};

#endif // FUNCTIONFILTERS_H
