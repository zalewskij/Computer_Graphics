#include "dithering.h"
#include <algorithm>
#include <set>
#include <cstdlib>
#include <cmath>
#include <QDebug>

Dithering::Dithering(){}

int Dithering::find_closest(const std::vector<int>& v, int x) {
    auto it = std::min_element(v.begin(), v.end(), [x](int a, int b) {
        return std::abs(a - x) < std::abs(b - x);
    });
    return *it;
}

std::vector<int> Dithering::init_vector_with_unique_random_numbers(int n) {
    std::vector<int> vec;
    std::set<int> unique_numbers;
    while (unique_numbers.size() < n) {
        int rand_num = std::rand() % 254 + 1;
        if (unique_numbers.count(rand_num) == 0) {
            unique_numbers.insert(rand_num);
            vec.push_back(rand_num);
        }
    }
    return vec;
}
QPixmap Dithering::randomDithering(QPixmap pixmap, int k){
    int w = pixmap.width();
    int h = pixmap.height();
    QImage img = pixmap.toImage();
    for(int x = 0; x<w; x++){
        for(int y = 0; y<h; y++){
            std::vector discreteLevels = Dithering::init_vector_with_unique_random_numbers(k-2);
            discreteLevels.push_back(0);
            discreteLevels.push_back(255);
            QColor pixel(img.pixel(x, y));
            int R = Dithering::find_closest(discreteLevels, pixel.red());
            int G = Dithering::find_closest(discreteLevels, pixel.green());
            int B = Dithering::find_closest(discreteLevels, pixel.blue());
            img.setPixel(x,y, qRgb(R,G,B));
        }
    }
    return  QPixmap::fromImage(img);
}

