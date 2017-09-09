//
// Created by flamming on 15/08/2017.
//

#ifndef NAR_MUTISRCMATCHING_H
#define NAR_MUTISRCMATCHING_H
#include <MarkerDB.h>


namespace nar {
    class MarkerDetector {
    public:
        struct DResult {
            int id;
            cv::Mat H_f_m;
        };

        void init();

        static void buildVoc();

        std::pair<cv::Point2f, cv::Point2f> getBBox(std::vector<cv::Point2f> vetexes);

        std::vector<DResult> processFrame(FramePtr rframe);

        std::shared_ptr<MarkerDB> mdb;

//#define UsePic

    };
}
#endif //MUTISRCMATING_MUTISRCMATCHING_H
