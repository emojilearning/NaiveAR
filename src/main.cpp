//
// Created by flamming on 16/08/2017.
//
#include <MutiSrcMatching.h>
#include <opencv2/calib3d.hpp>
#include <ARBox.h>
#include <System.h>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

using namespace nar;
int main()
{

    System system1;
#ifndef GENVOC
    system1.init();
#else
	buildVoc();
	return 0;
#endif // GENVOC
#ifndef GenTestImg
    cv::VideoCapture capture(0);
    int i = 0;
    char name[80];
    while (true) {
        sprintf(name, "../Resource/TestImg/%d.jpg", i++);

        Mat frame;
#ifdef UsePic
        frame = imread(name);
#else
        if (!capture.read(frame))
            break;
#endif // UsePic
        if (!frame.data) {
            break;
        }
        Mat rfrm = system1.pushImage(frame);
        imshow("result",rfrm);
        waitKey(1);
    }
#else
	genTestImg();
#endif // !GenTestImg


	waitKey(0);
	return 0;
}