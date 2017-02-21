#include "dialog.h"
#include "ui_dialog.h"
#include <cascadedetect.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(on_begin_play_clicked()));
    camera.open(0);
    if(!camera.isOpened()){
        std::cerr << "camera open falied"<<std::endl;
    }
    camera.set(CV_CAP_PROP_FRAME_WIDTH ,800);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT , 450); //设置分辨率
    std::cout << "camera connected successful!"<< std::endl;

    face_cascade.load("/usr/local/Cellar/opencv3/3.2.0/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml");
    eyes_cascade.load("/usr/local/Cellar/opencv3/3.2.0/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml");

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_begin_play_clicked()
{
    timer->start(13);  //显示时间控制
    camera >> frame;//从摄像头读取一帧图像
    if( frame.empty()){
        std::cout << "No captured frame --Break!"<<std::endl;
        exit(0);
    }
    //使用级联分类器
    dstImage = cascade_detect( frame );
    img = cvMat2QImage(dstImage);
    ui->play->resize(frame.cols,frame.rows);
    ui->play->setPixmap(QPixmap::fromImage(img));
}
