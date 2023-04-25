#include "BPD.h"

vector<Mat> BPD::calculate(Mat& pic, const uint bitDepth)
{
	vector<Mat> results;
	for (int i = 0; i < bitDepth; i++)
	{
		Mat newPic = Mat::zeros(pic.rows, pic.cols, CV_8UC1);
		uchar mask = 1 << i;

		for (int i = 0; i < pic.rows; i++)
		{
			uchar* srcptr = pic.ptr<uchar>(i);
			uchar* dstptr = newPic.ptr<uchar>(i);
			for (int j = 0; j < pic.cols; j++)
			{
				if (srcptr[j] & mask)
				{
					dstptr[j] = WHITE;
				}
			}
		}
		results.push_back(newPic);
	}
	return results;
}
