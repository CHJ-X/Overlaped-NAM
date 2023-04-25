#pragma once
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<vector>

using std::vector;
using cv::Mat;

/*Binary-bit Plane Decomposition, BPD*/

class BPD
{
public:
	const static uchar WHITE = 255;
	static vector<Mat> calculate(Mat& pic, const uint bitDepth);
};

