#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QTimer>
#include <QMainWindow>
#include <dialog.h>
#include <opencv2/opencv.hpp>
#include <cvmat2qimage.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void readFrame();//接收timer信号
    void cartoonifyImage(cv::Mat &srcColor, cv::Mat &dst );


    void on_openCamera_clicked();

    void on_startCaroonify_clicked();

    void on_closeCamera_clicked();


    void on_openDialog_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QImage img;
    cv::VideoCapture camera;
    cv::Mat frame,dstImage;
    bool flag;
    Dialog *dialog;
};

#endif // MAINWINDOW_H
