#include <MatchTricks.h>
using namespace std;
using namespace cv;
namespace nar
{
	void featureMatchByKnn(Mat& desp_m , Mat& desp_i , vector<DMatch>& matches)
	{
		//FlannBasedMatcher matcher2(new flann::LshIndexParams(20, 10, 2));
        if (desp_i.rows == 0||desp_m.rows==0)
            return;
		auto cross_matcher = BFMatcher::create(NORM_HAMMING, true);
		auto knn_matcher = BFMatcher::create(NORM_HAMMING);
		//auto knn_matcher = &matcher2;
		vector<DMatch> cross_matches;
		vector<vector<DMatch>> knn_matches;
		cross_matcher->match(desp_m, desp_i, cross_matches);
		knn_matcher->knnMatch(desp_m, desp_i, knn_matches, 2);
		vector<bool> mask(knn_matches.size(), true);
		int i = 0;
		for (auto m = knn_matches.begin(); m != knn_matches.end();m++ )
		{
			if (m->size() == 1)
				continue;
			else if (m->size() == 0)
				mask[i] = false;
			else if ((*m)[0].distance / (*m)[1].distance > 1.0 / 1.5)
				mask[i] = false;
			i++;
		}
		i = 0;
		for (auto m : knn_matches)
		{
			if (!mask[i++])
				continue;
			for (auto n : cross_matches)
			{
				if (m[0].trainIdx == n.trainIdx&&m[0].queryIdx == n.queryIdx)
					matches.push_back(n);
			}
		}
		const int limit = 60;
		if (matches.size() > limit)
		{
			nth_element(matches.begin(), matches.begin() + limit, matches.end());
			matches.erase(matches.begin() + limit, matches.end());
		}
	}

	Mat getHomography(vector<KeyPoint> &kpts_m, vector<KeyPoint> &kpts_i, vector<DMatch>& matches)
	{
		vector<Point2f> m_p;
		vector<Point2f> f_p;
		for (auto correspondence : matches)
		{
			m_p.push_back(kpts_m[correspondence.queryIdx].pt);
			f_p.push_back(kpts_i[correspondence.trainIdx].pt);
		}

		return findHomography(m_p, f_p, RANSAC);
	}

	Mat match(FramePtr marker, FramePtr img, std::vector<cv::Point2f>& scene_corners)
	{
		scene_corners.resize(4);
		static int cnt = 0;
		vector<DMatch> matches;
		featureMatchByKnn(marker->descriptor, img->descriptor, matches);
		//cout << "matches" << matches.size() << endl;
		if (matches.size() < 6)
			return {};
		Mat H_f_m = getHomography(marker->kpts, img->kpts, matches);
		if (!H_f_m.data)
			return {};
		if (GetProjectMarkerCorners(marker, H_f_m, scene_corners))
		{
			cnt++;
			cout << "ADD UP TO" << cnt << endl;
			return H_f_m;
		}
		return {};
	}

	inline float GetAreaSize(std::vector<cv::Point2f>& points) 
	{
		assert(points.size() == 4);
		cv::Point2f a = points[1] - points[0];
		cv::Point2f b = points[3] - points[0];
		cv::Point2f c = points[1] - points[2];
		cv::Point2f d = points[3] - points[2];

		//float s1 = a.cross(b);

		return fabs(0.5 * a.cross(b)) + fabs(0.5 * c.cross(d));
	}

	bool isConvex(std::vector<cv::Point2f>& scene_corners)
	{
		auto flag = true;
		for (size_t i = 0; i < 2; i++)
		{
			auto edge = scene_corners[(i + 2) % 4] - scene_corners[i];
			auto edge1 = scene_corners[(i + 1) % 4] - scene_corners[i];
			auto edge2 = scene_corners[(i + 3) % 4] - scene_corners[i];
			auto r1 = edge.cross(edge1);
			auto r2 = edge.cross(edge2);
			if (r1*r2 > 0)
				flag = false;
		}
		return flag;
	}
	inline static double angle(cv::Point2f pt1, cv::Point2f pt2, cv::Point2f pt0)
	{
		double dx1 = pt1.x - pt0.x;
		double dy1 = pt1.y - pt0.y;
		double dx2 = pt2.x - pt0.x;
		double dy2 = pt2.y - pt0.y;
		return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
	}
	inline bool checkAngle(std::vector<cv::Point2f>& points) {
		if (points.size() < 4)
			throw std::string("checkAngle: points.size() < 4");
		double maxCosine = 0;
		for (int i = 0; i < 4; i++) {
			double cosine = fabs(angle(points[(i + 3) % 4], points[(i + 1) % 4], points[i]));
			maxCosine = std::max(cosine, maxCosine);
		}
		if (maxCosine < 0.3)
			return true;
		return false;
	}
	bool GetProjectMarkerCorners(const FramePtr marker, const Mat H_f_m, std::vector<cv::Point2f>& scene_corners)
	{
		//-- Get the corners from the image_1 ( the object to be "detected" )
		std::vector<Point2f> obj_corners(4);
		obj_corners[0] = Point2f(0, 0); obj_corners[1] = Point2f(marker->image.cols, 0);
		obj_corners[2] = Point2f(marker->image.cols, marker->image.rows); obj_corners[3] = Point2f(0, marker->image.rows);
		perspectiveTransform(obj_corners, scene_corners, H_f_m);
		auto flag = isConvex(scene_corners);
		flag = flag&(GetAreaSize(scene_corners) > 50)&checkAngle(scene_corners);
		return flag;
	}
}