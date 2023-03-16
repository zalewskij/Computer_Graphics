#include <QImage>
#include <vector>
#include <QPixmap>

#ifndef CONVOLUTIONFILTERS_H
#define CONVOLUTIONFILTERS_H
class ConvolutionFilters
{
public:
    ConvolutionFilters();
    static QPixmap pad(QPixmap pixmap,int pading_left, int padding_top, int padding_right, int padding_bottom);
    static QPixmap applyFilter(QPixmap pixmap, int (*func)(int));
    static int inversion(int value);
    static int brightness(int value);
    static int contrast(int value);
    static int gamma(int value);
    static int black(int value);
    static int computeDivisor(std::vector<std::vector<int>> filter);
    static QPixmap convolve2d(QPixmap &pixmap, std::vector<std::vector<int>> filter, double divisor, double bias, int anchor_x, int anchor_y);
    static QPixmap convolve2d_v2(QPixmap &pixmap, std::vector<std::vector<int>> filter, double bias, int anchor_x, int anchor_y);
};
#endif // CONVOLUTIONFILTERS_H
