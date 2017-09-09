//
// Created by flamming on 2017/9/9.
//

#ifndef NAR_CAMERA_H
#define NAR_CAMERA_H

#include <opencv2/opencv.hpp>
#include <memory>

namespace nar
{
	/*
	implement of pinhole camera model.
	*/
    class Camera
    {
    public:
        Camera() {};
        Camera(double fx_, double fy_, double cx_, double cy_);
        static std::shared_ptr<Camera> getInstance();
        void setPar(double fx_, double fy_, double cx_, double cy_);

        cv::Vec2d camera2pixel(const cv::Vec3d& p_c);
        cv::Vec3d pixel2camera(const cv::Vec2d& p_p, double depth);
        const cv::Matx33d& getK();
    private:
        double fx, fy, cx, cy;
        cv::Matx33d K;
        cv::Matx33d K_inv;
    };
    typedef std::shared_ptr<Camera> CameraPtr;
}
#endif //MUTISRCMATING_CAMERA_H
