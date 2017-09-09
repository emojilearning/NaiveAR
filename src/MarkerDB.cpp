#include <MarkerDB.h>
#include <memory>
using namespace DBoW3;
using namespace std;
using namespace cv;
//methods of MarkerDB
namespace nar
{

	void nar::MarkerDB::load(string filename)
	{
		dict = make_shared<Vocabulary>();
		dict->load(filename);
		db = make_shared<Database>(*dict);
	}

	void MarkerDB::build(std::vector<cv::Mat>& voc_imgs,int n_orb_kpt)
	{
		Vocabulary voc(10, 6, DBoW3::TF_IDF, DBoW3::L1_NORM);
		auto orbDetector = ORB::create(n_orb_kpt);
		vector<Mat> all_desp;
		for (auto& img : voc_imgs)
		{
			vector<KeyPoint> kpts;
			Mat desp;
			orbDetector->detectAndCompute(img, noArray(), kpts, desp);
			all_desp.push_back(desp);
		}
		voc.create(all_desp);
		voc.save("ambd.voc");
	}

	void MarkerDB::addMarker(Mat marker)
	{
		auto temp = make_shared<Frame>(marker);
		m_ptrs.push_back(temp);
		temp->init(2000,dict);
		auto id = db->add(temp->descriptor);
	}

	shared_ptr<MarkerDB> MarkerDB::getInstance()
	{
		static shared_ptr<MarkerDB> db = make_shared<MarkerDB>();
		return db;
	}

	int MarkerDB::queryMarker(Mat imgdes)
	{
		if(!imgdes.data)
			return  -1;
		QueryResults ret;
		db->query(imgdes, ret);
		//cout << ret << endl;
		if (ret.empty())
			return -1;
		return ret.back().Id;
	}
}