#include <QImage>
#include <vector>
#include <QPixmap>

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

class Histogram
{
public:
    Histogram();
    static QPixmap histogramStretching(QPixmap pixmap);
    static QPixmap histogramEqualization(QPixmap pixmap);
};

#endif // HISTOGRAM_H
