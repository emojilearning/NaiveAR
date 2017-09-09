//
// Created by flamming on 01/07/2017.
//

#include <opencv2/opencv.hpp>
#include <memory>
#include <DBoW3/DBoW3.h>
#include <unordered_map>
#ifndef NAR_MARKER_H
#define NAR_MARKER_H

namespace nar
{
	cv::Mat convertToGray(cv::Mat& img);
	/*
	definition of Frame
	*/
	class Frame
	{
	public:
		Frame(cv::Mat img) :initialImg(img),image(convertToGray(img)) {};
		void extractFeature(int n_features = 1000);
		void refresh();
		void extractFeature(int n_feature, cv::Mat mask);
		void init(int n_faeture, const std::shared_ptr<DBoW3::Vocabulary> voc);
		cv::Mat image;
		cv::Mat initialImg;
		std::vector<cv::KeyPoint> kpts;
		std::vector<cv::Point2f> pts;
		cv::Mat descriptor;
	};
	typedef std::shared_ptr<Frame> FramePtr;

	
}
#endif // !ARTHUS_MARKER_H
