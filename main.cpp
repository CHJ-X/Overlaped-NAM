#include"encoder.h"
#include"decoder.h"
#include"cv.h"
using namespace std;
using namespace cv;

int main()
{
    Mat pic(8, 8);
    Mat R(8, 8);
    pic.set(Point(4, 0), Point(7, 3), 1);
    pic.set(Point(1, 4), Point(2, 7), 1);
    pic.set(Point(0, 5), Point(3, 6), 1);
    cout << pic << endl;
    R = encoder(pic);
    cout << R << endl;
    system("pause");
}