#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "assert.h"
using namespace cv;
using namespace std;

// void dfs(Mat &drawer, const vector<vector<Point>> &contours, 
//         const vector<Vec4i> &hierachy, const int &id, const int depth) {
//             if (id == -1) return;
//             static Scalar COLOR_LIST = {255, 255, 255};
//             if (contourArea(contours[id]) > 10000) {
//                 drawContours(drawer, contours, id, COLOR_LIST, 1.5);
//             }
//             for (int i = hierachy[id][2]; i != -1; i = hierachy[i][0]) {
//                 dfs(drawer, contours, hierachy, i, depth + 1);
//             }
//         }

int main() {
    Mat img = imread("../test.png");
    Mat channel[3], r_b, output;
    vector<vector<Point>> contours;
    vector<Vec4i> heirachy;
    // 二值化
    assert(img.channels() == 3);
    split(img, channel);
    r_b = 1.5*channel[2] - 1.3*channel[1] - 1.1*channel[0];
    normalize(r_b, r_b, 0., 255., NORM_MINMAX);
    threshold(r_b, output, 38, 255, THRESH_BINARY);
    Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(6, 6), Point(-1, -1));
    Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(6, 6), Point(-1, -1));
    erode(output, output, kernel_erode);
    dilate(output, output, kernel_dilate);

    // 绘制边界
    morphologyEx(output, output, MORPH_OPEN, kernel_dilate);
    findContours(output, contours, heirachy, RETR_TREE, CHAIN_APPROX_NONE);
    Mat drawer = Mat::zeros(Size(output.cols, output.rows), CV_8UC3);
    drawer = img.clone();
    for (int i = 0; i != -1; i = heirachy[i][0]) {
        if (contourArea(contours[i]) > 5000) {
            drawContours(drawer, contours, i, {255, 255, 255}, 1.5);
            Rect rect = boundingRect(contours[i]);
            rectangle(drawer, rect, {255, 255, 255}, 2);
        }
        // dfs(drawer, contours, heirachy, i, 0);
    }
    imshow("contours", drawer);
    waitKey();
    return 0;
}