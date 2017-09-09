//
// Created by flamming on 2017/9/9.
//
/*
The file includes all parameters,macros and paths used by the project;
*/
#ifndef NAR_CONFIG_H
#define NAR_CONFIG_H
#include <memory>
#include <string>

#define OPENCV_DRAW
//#define TRACKING_BY_DETECTION //tracking using a slam-like method or tracking by detection
//#define GENVOC

namespace nar
{
	class Config
	{
	public:
		static std::shared_ptr<Config> getInstance()
		{
			static std::shared_ptr<Config> f = std::make_shared<Config>();
			return f;
		};
		int match_max_point_num() { return match_max_point_num_; }
		int k_of_means() { return k_of_means_; }
		int lv_of_means() { return lv_of_means_; }
		double f() { return f_; }
		double cx() { return cx_; }
		double cy() { return cy_; }
		int width() { return width_; }
		int height() { return height_; }
		std::string& vocUrl() { return vocUrl_; }
		Config() {};
	private:
		int match_max_point_num_{ 35 };
		int k_of_means_{ 10 };
		int lv_of_means_{ 5 };
		double f_{ 490 };
		double cx_{ 320 };
		int width_{ 640 };
		double cy_{ 240 };
		int height_{ 480 };
		std::string vocUrl_{ "../../ambd.voc" };
	};
	typedef std::shared_ptr<Config> ConfigPtr;
}

#endif //MUTISRCMATING_CONFIG_H
