#ifndef RECTIFYTHREAD_H
#define RECTIFYTHREAD_H
#include <QThread>
#include <QDebug>
#include <QImage>
#include <vector>

using namespace std;

class RectifyThread: public QThread{
private:
    const QImage &original_pixels;
    QImage *rectified_pixels;
    int rectified_width;
    const vector<long double> &correction_factor;
    int end_row;
    int start_row;
public:
    RectifyThread(const QImage &original_pixels, QImage *rectified_pixels, int rectified_width, const vector<long double> &correction_factor, int end_row, int start_row):
        original_pixels(original_pixels), rectified_pixels(rectified_pixels), rectified_width(rectified_width), correction_factor(correction_factor), end_row(end_row), start_row(start_row){}
    void run();
};

#endif // RECTIFYTHREAD_H
