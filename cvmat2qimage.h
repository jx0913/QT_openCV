#ifndef CVMAT2QIMAGE_H
#define CVMAT2QIMAGE_H

#include <opencv2/opencv.hpp>
#include <QImage>
#include <QDebug>


QImage cvMat2QImage(const cv::Mat mat);

#endif // CVMAT2QIMAGE_H
