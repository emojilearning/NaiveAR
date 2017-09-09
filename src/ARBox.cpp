//
// Created by flamming on 2017/9/9.
//
#include <ARBox.h>

using namespace cv;

void nar::ARBoX::drawOn(cv::Mat img)
{
	for (int i = 0; i < 4; i++)
	{
		line(img, scene_corners[2 * i], scene_corners[2 * i + 1], Scalar(255, 0, 0), 4);
		line(img, scene_corners[2 * i], scene_corners[(2 * i + 2) % 8], Scalar(255, 0, 0), 4);
		line(img, scene_corners[2 * i + 1], scene_corners[(2 * i + 3)%8], Scalar(255, 0, 0), 4);
	}
}

void nar::ARBoX::transform(cv::Matx44d T)
{
	scene_corners.clear();
	for (int i = 0; i < 8; i++)
	{
		Vec4d y = T * vertexes[i];
		scene_corners.push_back(p->camera2pixel({y(0),y(1),y(2)}));
	}
}

void nar::ARBoX::init(CameraPtr p_)
{
	p = p_;
	for (size_t i = 0; i < 4; i++)
	{
        auto temp = p->pixel2camera(obj_corners[i], 1);
		vertexes.emplace_back(temp(0),temp(1),temp(2),1);
		vertexes.emplace_back(vertexes.back()[0], vertexes.back()[1], h,1);
	}
}

nar::ARBoX::ARBoX()
{
	auto fg = Config::getInstance();
	obj_corners.emplace_back(0, 0);
	obj_corners.emplace_back(0, fg->cy()*2);
	obj_corners.emplace_back(fg->cx()*2, fg->cy()*2);
	obj_corners.emplace_back(fg->cx()*2, 0);
	h = 0.5;
}