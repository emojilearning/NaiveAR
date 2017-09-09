//
// Created by flamming on 2017/9/9.
//

#include "System.h"
#include <MatchTricks.h>
using namespace std;
using namespace cv;
namespace nar
{


    void System::init()
    {
        p_detector.init();
        p_camera = Camera::getInstance();
		p_config = Config::getInstance();
		p_camera->setPar(p_config->f(), p_config->f(), p_config->cx(), p_config->cy());//make_shared<Camera>(p_config->f(),p_config->f(),p_config->cx(),p_config->cy());
        box.init(p_camera);
		planar = { 0,0,-1,1 };
    }

    Mat System::pushImage(cv::Mat frame) {
        auto rfrm = frame.clone();
        last_frame = current_frame;
        current_frame = make_shared<Frame>(frame);
        if(isTracking)
        {
            current_frame->extractFeature(1000);
            vector<DMatch> matches;
            featureMatchByKnn(last_frame->descriptor, current_frame->descriptor, matches);
            //cout << "matches" << matches.size() << endl;
            if (matches.size() < 6) {
                isTracking = false;
                return rfrm;
            }
            Mat H_f_m_1 = getHomography(last_frame->kpts, current_frame->kpts, matches);
            H_f_m = H_f_m_1*H_f_m;
			sloveByPnp(last_frame->kpts, current_frame->kpts, matches);
        }
        else {
            auto results = p_detector.processFrame(current_frame);
            if (results.empty()) {
                return rfrm;
            }
            for (auto r:results) {
                H_f_m = r.H_f_m;
            }
        }
        vector<Mat> tContainer, nContainer, RContainer;
        cv::decomposeHomographyMat(H_f_m, p_camera->getK(), RContainer, tContainer, nContainer);

        int maxIdx = -1;
        double maxd = -1;
        for (int i = 0; i < 4; ++i) {
            if (nContainer[i].at<double>(2) > 0 && fabs(nContainer[i].at<double>(2)) > (maxd)) {
                maxd = fabs(nContainer[i].at<double>(2));
                maxIdx = i;
            }
        }
        if (maxIdx == -1 || tContainer[maxIdx].dot(tContainer[maxIdx]) > 40) {
            return rfrm;
        }
        t = tContainer[maxIdx];
        R = RContainer[maxIdx];
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                T(i,j) = R(i,j);
            }
            T(i,3) = t(i);
            T(3,i) = 0;
        }
		Matx44d tempT;
		transpose(T.inv(), tempT);
		planar = tempT*planar;
#ifdef OPENCV_DRAW
        box.transform(T);
        box.drawOn(rfrm);
#endif

#ifdef TRACKING_BY_DETECTION
		isTracking = false;
#else
		isTracking = true;
#endif // TRACKING_BY_DETECTION


        return rfrm;
    }


}
