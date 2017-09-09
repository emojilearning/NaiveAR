//
// Created by flamming on 20/07/2017.
//

#include <FrameStruct.h>
#ifndef NAR_MATCH_TRICKS_H
#define NAR_MATCH_TRICKS_H
namespace nar
{
	bool GetProjectMarkerCorners(const FramePtr marker, const cv::Mat H_f_m, std::vector<cv::Point2f>& scene_corners);
	cv::Mat match(FramePtr marker, FramePtr img, std::vector<cv::Point2f>& scene_corners);
	void featureMatchByKnn(cv::Mat& marker, cv::Mat& img, std::vector<cv::DMatch>& matches);
	cv::Mat getHomography(std::vector<cv::KeyPoint> &kpts_m,
		std::vector<cv::KeyPoint> &kpts_i, std::vector<cv::DMatch>& matches);
};
#endif // !MATCH_TRICKS_H
