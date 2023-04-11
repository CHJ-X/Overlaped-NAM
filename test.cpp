#include"encoder.h"
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
	Mat pic = imread("D:\\newPepper256.bmp", 0);
    imshow("pic",pic);
    Mat roi = pic(Rect(Point(0, 0), Point(128, 128)));

    Codec codec(pic);
    Mat R = codec.encode();
	cout << "blockCount: " << codec.blockCount << endl;
	Mat newPic = codec.decode(R);
    imshow("re:pic", newPic);
    waitKey(0);
    system("pause");
}

int main()
{
	test01();
	test02();
}

