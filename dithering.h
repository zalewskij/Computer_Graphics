#include <QImage>
#include <vector>
#include <QPixmap>
#include <QDebug>

#ifndef DITHERING_H
#define DITHERING_H


class Dithering
{
public:
    Dithering();
    static QPixmap randomDithering(QPixmap pixmap, int k);
    static int find_closest(const std::vector<int>& v, int x);
    static std::vector<int> init_vector_with_unique_random_numbers(int n);
};

#endif // DITHERING_H
