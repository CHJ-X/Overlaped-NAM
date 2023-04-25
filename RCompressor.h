#pragma once

#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<string>
#include<unordered_map>
using std::unordered_map;
using std::string;
using cv::Mat;

class RCompressor
{
public:
	string encode(Mat& R);
	Mat decode(string s);
private:
	/*represent
	*  00 -- START ----- 1
	*  01 -- H_MATRIX -- 2
	*  10 -- V_MATRIX -- 3
	*  11 -- ISOLATED -- 4
	*/
	unordered_map<uchar, string> hashmap = {
		{1, "00"},
		{2, "10"},
		{3, "10"},
		{4, "11"}
	};
};

