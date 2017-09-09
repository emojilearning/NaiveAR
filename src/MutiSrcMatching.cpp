#include <opencv2/opencv.hpp>
#include <MarkerDB.h>
#include <MatchTricks.h>
#include <MutiSrcMatching.h>
#include <Config.h>

using namespace std;
using namespace cv;
using namespace nar;

//#define GENVOC
namespace nar {
    void MarkerDetector::init() {
        vector<Mat> marker;
        mdb = MarkerDB::getInstance();
        mdb->load(Config::getInstance()->vocUrl());

        char markUrl[50];
		Mat marker1 = imread("../../Resource/Marker/marker.jpg");
		
		mdb->addMarker(marker1);
    }

    void MarkerDetector::buildVoc() {
        vector<Mat> voc_imgs;
        char markUrl[50];
        for (int i = 0; i < 100; i++) {
            sprintf(markUrl, "../../Resource/traindata/traindata (%d).jpg", i + 1);
            voc_imgs.push_back(imread(markUrl));
        }

        MarkerDB::build(voc_imgs);
    }

    pair<Point2f, Point2f> MarkerDetector::getBBox(vector<Point2f> vetexes) {
        Point2f lu{1000, 1000};
        Point2f rd{-1, -1};
        vector<float> x;
        vector<float> y;
        for (size_t i = 0; i < vetexes.size(); i++) {
            lu.x = lu.x < vetexes[i].x ? lu.x : vetexes[i].x;
            lu.y = lu.y < vetexes[i].y ? lu.y : vetexes[i].y;
            rd.x = rd.x > vetexes[i].x ? rd.x : vetexes[i].x;
            rd.y = rd.y > vetexes[i].y ? rd.y : vetexes[i].y;
        }
        return make_pair(lu, rd);
    }


    vector<MarkerDetector::DResult> MarkerDetector::processFrame(FramePtr rframe) {
        vector<DResult> results;
        rframe->init(2000, mdb->dict);
        vector<bool> mask(rframe->kpts.size(), true);
        auto id = mdb->queryMarker(rframe->descriptor);
		int cnt = 0;
        while (id != -1&&mdb->m_ptrs.size()>cnt) {
			cnt++;
            vector<Point2f> vetexes;
            Mat tdes;
            auto marker = mdb->m_ptrs[id];
            auto H_f_m = match(marker, rframe, vetexes);
            if (H_f_m.data) {
#ifdef OPENCV_SHOW
                line(rframe->initialImg, vetexes[0], vetexes[1], Scalar(0, 255, 0), 4);
                line(rframe->initialImg, vetexes[1], vetexes[2], Scalar(0, 255, 0), 4);
                line(rframe->initialImg, vetexes[2], vetexes[3], Scalar(0, 255, 0), 4);
                line(rframe->initialImg, vetexes[3], vetexes[0], Scalar(0, 255, 0), 4);
#endif
                auto bbox = getBBox(vetexes);
                for (auto i = 0; i < rframe->kpts.size(); i++) {
                    auto pt = rframe->kpts[i].pt;
                    if (!(pt.x > bbox.second.x || pt.x < bbox.first.x || pt.y > bbox.second.y ||
                          pt.y < bbox.first.y)) {
                        mask[i] = false;
                    } else {
                        tdes.push_back(rframe->descriptor.row(i));
                    }
                }
            } else
                break;
            results.push_back({id, H_f_m});
            //cout<<H_f_m<<endl;

            id = mdb->queryMarker(tdes);
        }
#ifdef OPENCV_SHOW
        imshow("detection", rframe->initialImg);
#endif
        return results;
    }

}

