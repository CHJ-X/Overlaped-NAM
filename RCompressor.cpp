#include "RCompressor.h"

/*represent
*  00 -- START
*  01 -- H_MATRIX
*  10 -- V_MATRIX
*  11 -- ISOLATED
*/
string RCompressor::encode(Mat& R)
{
	uchar a = 1;
	std::cout << hashmap[a];
	string s = "";
	int rows = R.rows;
	int cols = R.cols;
	for (int i = 0; i < rows; i++)
	{
		uchar* ptr = R.ptr<uchar>(i);
		int j = 0;
		for (; j < cols; j++)
		{

		}
	}


	return s;
}
