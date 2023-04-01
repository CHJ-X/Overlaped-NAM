#include"encoder.h"
#include"decoder.h"
#include<map>

using namespace std;
using namespace cv;

int main()
{
    Mat pic = Mat::zeros(8, 8, CV_8UC1);
    Mat R = Mat::zeros(8, 8, CV_8UC1);

    setRectInMat(pic, Point(4, 0), Point(7, 3), 1);
    setRectInMat(pic, Point(1, 4), Point(2, 7), 1);
    setRectInMat(pic, Point(0, 5), Point(3, 6), 1);
    cout << pic << endl;
    R = encoder(pic);
    cout << R << endl;
    pic = decoder(R);
    cout << pic << endl;
    system("pause");
}