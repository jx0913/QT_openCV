#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    flag = false;
    connect(timer,SIGNAL(timeout()),this,SLOT(readFrame()));


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::cartoonifyImage(cv::Mat &srcColor, cv::Mat &dst)
{
        cv::Mat gray;
        cv::cvtColor(srcColor , gray ,CV_BGR2GRAY);  //默认彩色格式是BGR

        const int MEDIAN_BLUR_FILTER_SIZE = 7;
        cv::medianBlur(gray ,gray ,MEDIAN_BLUR_FILTER_SIZE);  //中值滤波器去噪
        cv::Mat edges;
        const int LAPLACIAN_FILTER_SIZE = 5;
        cv::Laplacian(gray ,edges , CV_8U ,LAPLACIAN_FILTER_SIZE);

        cv::Mat mask;
        const int EDGES_THRESHOLD = 65;
        cv::threshold(edges , mask ,EDGES_THRESHOLD,255 ,cv::THRESH_BINARY_INV); //生成边缘掩码mask
        mask.copyTo(dst);

        cv::Size sizeSrc = srcColor.size();  //算法较为复制，为提高效率，将原图像长宽缩减一半进行双边滤波处理
        cv::Size smallSize;
        smallSize.width = sizeSrc.width/2;
        smallSize.height = sizeSrc.height/2;
        cv::Mat smallImage = cv::Mat(smallSize ,CV_8UC3);
        cv::resize(srcColor ,smallImage ,smallSize ,0,0,cv::INTER_LINEAR);

        cv::Mat tmp = cv::Mat(smallSize ,CV_8UC3);
        int repetitions = 3;
        for(int i = 0; i<repetitions ;i++)
        {
            int ksize = 9;
            double sigmaColor = 11;
            double sigmaSpace = 5;
            bilateralFilter(smallImage ,tmp ,ksize,sigmaColor,sigmaSpace);
            bilateralFilter(tmp ,smallImage ,ksize,sigmaColor,sigmaSpace);
        }

        cv::Mat bigImg;
        cv::resize(smallImage ,bigImg,sizeSrc ,0 ,0 ,cv::INTER_LINEAR);
        dst.setTo(0); //全置为0
        bigImg.copyTo(dst , mask);//掩码中黑色部分不会被复制，生成 素描掩码+ 彩色图画 = 卡通图画

}

void MainWindow::readFrame()
{

    if ( cameraFlag )
        camera >>frame;
    else
        videoFile >> frame;

    if(frame.empty()){
        std::cerr<<"ERROR：ouldn't grab a camera frame."<<std::endl;
        exit(1);
        }
    if (flag)
        cartoonifyImage(frame , dstImage);
    else
        frame.copyTo(dstImage);

    img = cvMat2QImage(dstImage);
    ui->play->resize(frame.cols,frame.rows);
    ui->play->setPixmap(QPixmap::fromImage(img));
   // dialog->on_showNumber_clicked();


}

void MainWindow::on_openCamera_clicked()
{
        camera.open(0);
        cameraFlag = true;
       if (!camera.isOpened())
       {
           std::cerr <<"Could not access the camera or video!"<<
           std::endl;
           exit(1);
       }
       camera.set(CV_CAP_PROP_FRAME_WIDTH ,800);
       camera.set(CV_CAP_PROP_FRAME_HEIGHT , 450); //设置分辨率

       timer->start(13);  //显示时间控制
}

void MainWindow::on_startCaroonify_clicked()
{
    flag = !flag;
}

void MainWindow::on_closeCamera_clicked()
{
    timer->stop();
    camera.release();
    exit(1);  //结束退出
}

void MainWindow::on_openDialog_clicked()
{
    dialog = new Dialog(this);
    dialog->setModal(false);
    dialog->show();
}

void MainWindow::on_openFile_triggered()
{
    /*说明：这样就会产生一个对话框，和系统的资源管理器差不多的。返回的是你选择文件的 绝对路径。
    参数1：父窗口
    参数2：对话框的标题
    参数3：默认的打开的位置，如”我的文档“等
    参数4：文件的过滤器，注意文件类型之间用  ；；  分开
    */
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择视频文件"), " ",  tr("Allfile(*.*);;video(*.avi);;video(*.mp4)"));
    std::string fileSource = fileName.toStdString();
    //std::cout << fileSource <<std::endl;
    videoFile.open( fileSource );
    if( !videoFile.isOpened() ){
        std::cout << "Video file open falied!"<< std::endl;
        exit(0);
    }
    timer->start(13);
}
