#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "assert.h"
#include "string"
using namespace cv;
using namespace std;

int main() {
    Mat img, gray, channel[3], output;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat drawer = Mat::zeros(Size(gray.cols, gray.rows), CV_8UC3);
    vector<vector<Point>> contour;
    vector<Vec4i> hierachy;
    vector<Point> hull;
    // cout << jpg + to_string(1) << endl;
    for (int i = 1; i < 6; i++) {
        img = imread("../plates/00" + to_string(i) + ".jpg");
        assert(img.channels() == 3);
        // 二值化
        split(img, channel);
        gray = 1.5*channel[0] - channel[1] - channel[2];
        normalize(gray, gray, 0., 255., NORM_MINMAX);
        threshold(gray, gray, 130, 255, THRESH_OTSU);
        morphologyEx(gray, gray, MORPH_CLOSE, kernel);

        // 绘制边界
        drawer = img.clone();
        findContours(gray, contour, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE);
        int incon;
        for (int i = 0; i + 1; i = hierachy[i][0]) {
            incon = 0;
            for (int j = hierachy[i][2]; j + 1; j = hierachy[j][0]) {
                incon++;
            }
            if (incon > 5) {
                vector<vector<Point>> approx_contour(1);
                convexHull(contour[i], hull);
                approxPolyDP(hull, approx_contour[0], 20, true);
                drawContours(drawer, approx_contour, 0, {100, 100, 255}, 2);
            }
        }
        // imshow("img", output);
        imshow("contour", drawer);
        waitKey();
    }
    return 0;
}