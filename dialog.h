#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <opencv2/opencv.hpp>
#include <cvmat2qimage.h>
#include  <QTimer>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_begin_play_clicked();

private:
    Ui::Dialog *ui;
    cv::VideoCapture camera;
    cv::Mat frame,dstImage;
    QImage img;
    QTimer *timer;
};

#endif // DIALOG_H
