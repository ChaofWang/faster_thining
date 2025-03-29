#include <vector>
#include <cstdint>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>

static void faster_thining(const cv::Mat& src, cv::Mat& dst) {
    // 核心思想，查表法，列出周围8像素的所有情况下，针对每一种情况直接判断是否需要删除，
    // 每次删除，一共迭代两轮，其中第一轮是删除横纵方向上的情况，第二轮删除斜对角方向上的情况
    // lut 0代表不能删除，1代表横纵方向下可以删除，2代表斜对角方向上可以删除，3代表不能删除
    // lut 索引采用8bit 二进制索引方式进行索引
    const std::vector<uchar> lut = {0, 0, 0, 1, 0, 0, 1, 3, 0, 0, 3, 1, 1, 0,
                                    1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0,
                                    3, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    2, 0, 0, 0, 3, 0, 2, 2, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,
                                    0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0,
                                    3, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0,
                                    2, 0, 0, 0, 3, 1, 0, 0, 1, 3, 0, 0, 0, 0,
                                    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 1, 3, 1, 0, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 1, 3,
                                    0, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    2, 3, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                                    0, 0, 3, 3, 0, 1, 0, 0, 0, 0, 2, 2, 0, 0,
                                    2, 0, 0, 0};
    auto mask = src.clone();
    mask = mask / 255.0;

    int nrows = mask.rows + 2;
    int ncols = mask.cols + 2;

    cv::Mat skeleton(nrows, ncols, CV_8U, cv::Scalar(0));
    mask.copyTo(skeleton(cv::Rect(1, 1, mask.cols, mask.rows)));

    cv::Mat cleaned_skeleton = skeleton.clone();

    bool pixel_removed = true;
    while (pixel_removed) {
        pixel_removed = false;

        for (int pass_num = 0; pass_num < 2; ++pass_num) {
            bool first_pass = (pass_num == 0);

            for (int row = 1; row < nrows - 1; ++row) {
                uchar* skeleton_ptr = skeleton.ptr<uchar>(row) + 1;
                uchar* cleaned_skeleton_ptr = cleaned_skeleton.ptr<uchar>(row) + 1;
                for (int col = 1; col < ncols - 1; ++col) {
                    if (*skeleton_ptr) {
                        uchar neighbors =
                                lut[skeleton_ptr[-ncols - 1] +
                                    2 * skeleton_ptr[-ncols] +
                                    4 * skeleton_ptr[-ncols + 1] +
                                    8 * skeleton_ptr[1] +
                                    16 * skeleton_ptr[ncols + 1] +
                                    32 * skeleton_ptr[ncols] +
                                    64 * skeleton_ptr[ncols - 1] +
                                    128 * skeleton_ptr[-1]];

                        if (neighbors == 0) {
                            ++skeleton_ptr;
                            ++cleaned_skeleton_ptr;
                            continue;
                        } else if ((neighbors == 3) ||
                                   (neighbors == 1 && first_pass) ||
                                   (neighbors == 2 && !first_pass)) {
                            // 删除像素点
                            *cleaned_skeleton_ptr = 0;
                            pixel_removed = true;
                        }
                    }
                    ++skeleton_ptr;
                    ++cleaned_skeleton_ptr;
                }
            }
            cleaned_skeleton.copyTo(skeleton);
        }
    }

    dst = skeleton(cv::Rect(1, 1, skeleton.cols - 2, skeleton.rows - 2)).clone();
    dst = dst * 255;
}


#define TIME_COST(start, end) std::chrono::duration_cast<std::chrono::milliseconds>((end) - (start)).count()

int main() {
    std::string filename = "./tmp.png";
    auto mask = cv::imread(filename, cv::IMREAD_UNCHANGED);
    cv::Mat sk;

    auto tic_0 = std::chrono::high_resolution_clock::now();
    for(int i=0; i< 100;i++){
        faster_thining(mask, sk);
    }
    std::cout << "faster_thining time: " << TIME_COST(tic_0, std::chrono::system_clock::now()) / 1000.0  / 100.0 << " seconds per time"<< std::endl;
    cv::imwrite("sk.png", sk);

    cv::Mat sk_orig;
    auto tic_1 = std::chrono::high_resolution_clock::now();
    for(int i=0; i< 100;i++){
        cv::ximgproc::thinning(mask, sk_orig);
    }
    std::cout << "cv::ximgproc::thinning time: " <<TIME_COST(tic_1, std::chrono::system_clock::now()) / 1000.0 / 100.0  << " seconds  per time"<< std::endl;

    cv::imwrite("sk_orig.png", sk_orig);
    /***
     *
     * faster_thining time: 0.0292352
       cv::ximgproc::thinning time: 0.261634
     */

}