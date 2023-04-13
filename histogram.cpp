#include "histogram.h"

Histogram::Histogram(){}

QPixmap Histogram::histogramStretching(QPixmap pixmap){
    int ImaxRed = 0;
    int ImaxGreen = 0;
    int ImaxBlue = 0;
    int IminRed = 255;
    int IminGreen = 255;
    int IminBlue = 255;
    int w = pixmap.width();
    int h = pixmap.height();
    QImage img = pixmap.toImage();

    //finding max and min
    for(int x = 0; x<w; x++){
        for(int y = 0; y<h; y++){
            QColor pixel(img.pixel(x, y));
            int r = pixel.red();
            int g = pixel.green();
            int b = pixel.blue();

            //finding biggest
            ImaxRed = r > ImaxRed ? r : ImaxRed;
            ImaxGreen = g > ImaxGreen ? g : ImaxGreen;
            ImaxBlue = b > ImaxBlue ? b : ImaxBlue;

            //fidning smallest
            IminRed = r < IminRed ? r : IminRed;
            IminGreen = g < IminGreen ? g : IminGreen;
            IminBlue = b < IminBlue ? b : IminBlue;
        }
    }

    //setting new pixels
    for(int x = 0; x<w; x++){
        for(int y = 0; y<h; y++){
            QColor pixel(img.pixel(x, y));
            int newR = 255.0/(ImaxRed - IminRed)* (pixel.red() - IminRed);
            int newG = 255.0/(ImaxGreen - IminGreen)* (pixel.green() - IminGreen);
            int newB = 255.0/(ImaxBlue - IminBlue)* (pixel.blue() - IminBlue);
            img.setPixel(x,y, qRgb(newR,newG,newB));
        }
    }
    return  QPixmap::fromImage(img);
}

QPixmap Histogram::histogramEqualization(QPixmap pixmap){
    int w = pixmap.width();
    int h = pixmap.height();
    int n = w*h;
    QImage img = pixmap.toImage();
    std::vector<double> redHist(256, 0);
    std::vector<double> greenHist(256, 0);
    std::vector<double> blueHist(256, 0);

    //calculate cdf
    for(int x = 0; x<w; x++){
        for(int y = 0; y<h; y++){
            QColor pixel(img.pixel(x, y));
            int r = pixel.red();
            int g = pixel.green();
            int b = pixel.blue();
            for(int j = r; j<256;j++){
                redHist[j] +=1;
            }
            for(int j = g; j<256;j++){
                greenHist[j] +=1;
            }
            for(int j = b; j<256;j++){
                blueHist[j] +=1;
            }
        }
    }

    //normalize 0 to 1
    for(int i = 0; i < 256; i++){
        redHist[i] /= n;
        greenHist[i] /= n;
        blueHist[i] /= n;
    }

    //find minimum per channel
    double DminRed = *min_element(redHist.begin(), redHist.end());
    double DminGreen = *min_element(greenHist.begin(), greenHist.end());
    double DminBlue = *min_element(blueHist.begin(), blueHist.end());

    //set new values
    for(int x = 0; x<w; x++){
        for(int y = 0; y<h; y++){
            QColor pixel(img.pixel(x, y));
            int newR = (redHist[pixel.red()] - DminRed) / (1- DminRed) * (255-1);
            int newG = (greenHist[pixel.green()] - DminGreen) / (1- DminGreen) * (255-1);
            int newB = (blueHist[pixel.blue() - DminBlue] - DminBlue) / (1- DminBlue) * (255-1);
            img.setPixel(x,y, qRgb(newR,newG,newB));
        }
    }
    return  QPixmap::fromImage(img);
}
