#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
using namespace cv;
using namespace std;

Eigen::Matrix4d getRotation(Eigen::MatrixXd cam_pos, Eigen::Quaterniond cam_quat) {
    Eigen::Matrix3d R;
    R = cam_quat.toRotationMatrix().transpose();
    Eigen::Matrix4d res = Eigen::Matrix4d::Zero();
    res.block(0, 0, 3, 3) = R;
    res.block(0, 3, 3, 1) = -R*cam_pos;
    res(3, 3) = 1;
    return res;
}

int main() {
    int n;
    Mat graph = Mat::zeros(1000, 2000, CV_8UC3);
    ifstream fin("../points.txt");
    fin >> n;
    Eigen::Quaterniond cam_quat(-0.5, 0.5, 0.5, -0.5);
    Eigen::MatrixXd cam_pos(3,1);
    cam_pos << 2.0, 2.0, 2.0;
    Eigen::Matrix4d cam_out = getRotation(cam_pos, cam_quat);
    Eigen::MatrixXd cam_in(3, 4);
    cam_in << 400., 0., 190., 0.,
            0., 400., 160., 0.,
            0., 0., 1., 0.;
    Eigen::MatrixXd point(4, 1);
    point(3) = 1;
    Eigen::MatrixXd res(3, 1);
    vector<Point2f> points;
    for (int i = 0 ; i < n; i++) {
        fin >> point(0) >> point(1) >> point(2);
        res = cam_in*cam_out*point;
        points.push_back(Point2f(res(0)/res(2), res(1)/res(2)));
    }
    for (int i = 0; i < points.size(); i++) circle(graph, points[i], 1, {255, 200, 100}, -1);
    imshow("result", graph);
    imwrite("../result.jpg", graph);
    waitKey();
    return 0;
}