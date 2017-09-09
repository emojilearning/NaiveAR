//
// Created by flamming on 2017/9/9.
//

#ifndef NAR_SYSTEM_H
#define NAR_SYSTEM_H

#include <Config.h>
#include <Camera.h>
#include <ARBox.h>
#include <FrameStruct.h>
#include <MutiSrcMatching.h>

namespace nar {
    class System {
    public:
        void init();
		cv::Mat pushImage(cv::Mat img);

    private:
        ConfigPtr p_config;
        CameraPtr p_camera;
        MarkerDetector p_detector;
        FramePtr last_frame;
        FramePtr current_frame;
        ARBoX box;
        cv::Vec3d t;
        cv::Matx33d R;
        cv::Matx44d T;
		cv::Mat H_f_m;
		cv::Vec4d planar;
        bool isTracking= false;
    };
}

#endif //MUTISRCMATING_SYSTEM_H
