#ifndef CASCADEDETECT_H
#define CASCADEDETECT_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

//级联分类器对象建立
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;



Mat cascade_detect(Mat frame)
{
    vector<Rect> faces;
    Mat frame_gray;
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    //人脸检测
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30,30) );
    for ( size_t i = 0; i < faces.size(); i++ )
        {
            Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
            ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
            Mat faceROI = frame_gray( faces[i] );
            std::vector<Rect> eyes;
            //-- In each face, detect eyes
            eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
            for ( size_t j = 0; j < eyes.size(); j++ )
            {
                Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
                int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
                circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
            }
        }
    return frame;

}


#endif // CASCADEDETECT_H
