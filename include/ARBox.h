//
// Created by flamming on 2017/9/9.
//

#ifndef NAR_ARBOX_H
#define NAR_ARBOX_H
#include <Config.h>
#include <Camera.h>
#include <opencv2/opencv.hpp>
namespace nar
{
	/*
	what is presented as AR object.
	*/
	class ARBoX
	{
	public:
		ARBoX();
		void init(CameraPtr p);
		void drawOn(cv::Mat);
		void transform(cv::Matx44d);
	private:
		std::vector<cv::Point2d> obj_corners;
		double h;
		std::vector<cv::Vec4d> vertexes;
		std::vector<cv::Point2d> scene_corners;
		CameraPtr p;
	};
}

#endif //MUTISRCMATING_ARBOX_H
