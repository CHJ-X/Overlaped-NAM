#include"Codec.h"
#include"BPD.h"
#include"RCompressor.h"
using namespace std;
using namespace cv;

void test01()
{
	Mat pic = imread("D:\\newPepper256.bmp", 0);
	Codec codec(pic);
	Rect rec(Point(0, 0), Point(2, 2));
	cout << "br";
	cout << rec.br() << endl;
}


void test02()
{
	Mat pic = imread("D:\\Lena256.bmp", 0);
	imshow("pic", pic);
	Mat roi = pic(Rect(Point(0, 0), Point(128, 128)));

	Codec codec(pic);
	Mat R = codec.encode();

	int count = 0;
	int count_hv = 0;
	for (int i = 0; i < R.rows; i++)
	{
		for (int j = 0; j < R.cols; j++)
		{
			if ( R.at<uchar>(i, j) == START)
			{
				count++;
			}
			if (R.at<uchar>(i, j) == H_MATRIX || R.at<uchar>(i, j) == V_MATRIX)
			{
				count_hv++;
			}
		}
	}

	cout << "count:" << count << endl;
	cout << "counthv:" << count_hv << endl;

	cout << "overlapped count: " << codec.overlapArea.size() << endl;
	cout << "blockCount: " << codec.blockCount << endl;
	Mat newPic = codec.decode(R);
	bool isSame = true;
	for (int i = 0; i < R.rows; i++)
	{
		for (int j = 0; j < R.cols; j++)
		{
			if (pic.at<uchar>(i, j) != newPic.at<uchar>(i, j))
			{
				isSame = false;
				
				break;
			}
		}
	}
	if (isSame)
	{
		cout << "same!!!" << endl;
	}
	else
	{
		cout << "not complete!!!!" << endl;

	}
	imshow("re:pic", newPic);
	waitKey(0);
	system("pause");
}

void test03()
{
	BPD bpd();
	Mat pic = imread("D:\\test.png", 0);
	vector<Mat> pics = BPD::calculate(pic, 8);
	for (int i = 0; i < pics.size(); i++)
	{
		imshow("pic" + std::to_string(i), pics[i]);
	}
	waitKey(0);
}

void test04()
{
	Mat pic = imread("D:\\test.png", 0);
	imshow("pic", pic);
	Codec codec(pic);
	Mat R = codec.encode();
	RCompressor RC;
	RC.encode(R);
}

void test05()
{
	Mat pic = imread("D:\\test.jpg");
	cv::Mat image_gray;
	cv::cvtColor(pic, image_gray, cv::COLOR_BGR2GRAY);
	BPD bpd();
	vector<Mat> pics = BPD::calculate(image_gray, 8);

	imshow("pic7", pics[7]);
	Codec codec(pics[7]);
	imwrite("pic7.bmp", pics[7]);
	Mat R = codec.encode();

	

	cout << "overlapped count: " << codec.overlapArea.size() << endl;
	cout << "pics7:" << codec.blockCount << endl;
	waitKey(0);
}

int main()
{
	// test01();
	test02();
	// test05();
	//test04();
}
