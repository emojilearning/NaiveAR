#include <FrameStruct.h>
#include <DBoW3/Database.h>
#include <DBoW3/Vocabulary.h>
using namespace DBoW3;
using namespace std;
using namespace cv;
namespace nar
{
	Mat convertToGray(Mat& img)
	{
		Mat out;
		if (img.channels() == 4)
			cv::cvtColor(img, out, CV_RGBA2GRAY);
		else if (img.channels() == 3)
			cv::cvtColor(img, out, CV_BGR2GRAY);
		else if (img.channels() == 1)
			out = img.clone();
		return  out;
	}
	//methods of Frame
	void Frame::extractFeature(int n_feature)
	{
		auto orbDetector = ORB::create(n_feature);
		orbDetector->detectAndCompute(image, noArray(), kpts, descriptor);
	}
	void Frame::extractFeature(int n_feature,Mat mask)
	{
		auto orbDetector = ORB::create(n_feature);
		orbDetector->detectAndCompute(image, mask, kpts, descriptor);
	}

	//methods of Marker
	void Frame::init(int n_faeture, const shared_ptr<DBoW3::Vocabulary> voc)
	{
		extractFeature(n_faeture);
	};
	void Frame::refresh()
	{
	}
}