#include"encoder.h"
#include"decoder.h"

using namespace std;
using namespace cv;
//
//int main()
//{
//    Mat pic = imread("D:\\newPepper256.bmp", 0);
//    imshow("pic",pic);
//    Mat roi = pic(Rect(Point(0, 0), Point(32, 32)));
//    Codec codec(roi);
//    Mat R = codec.encode();
//    cout << R << endl;
//    imshow("re:pic", pic);
//    waitKey(0);
//    system("pause");
//}