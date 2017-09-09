//
// Created by flamming on 01/07/2017.
//

#include <opencv2/opencv.hpp>
#include <memory>
#include <DBoW3/DBoW3.h>
#include <unordered_map>
#include <FrameStruct.h>
#ifndef NAR_MARKERDB_H
#define NAR_MARKERDB_H

namespace nar
{
	/*
	DBoW3,which is an implement for visual bag of words,is used here for recognition of markers.
	*/
	class MarkerDB
	{
	public:
		void addMarker(cv::Mat marker);
		void load(std::string filename);
		static std::shared_ptr<MarkerDB> getInstance();
		
		/*
		a method for user to build his own vocabulary.
		*/
		static void build(std::vector<cv::Mat>& voc_imgs, int n_orb_kpt = 1000);
		int queryMarker(cv::Mat img);
		std::vector<FramePtr> m_ptrs;
		std::shared_ptr<DBoW3::Vocabulary> dict;
	private:
		std::shared_ptr<DBoW3::Database> db;
	};
	typedef std::shared_ptr<MarkerDB> MarkerDBPtr;
	
}
#endif // !ARTHUS_MARKER_H
