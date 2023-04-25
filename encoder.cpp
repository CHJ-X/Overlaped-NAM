#include"Codec.h"
using namespace cv;
using cv::Mat;
using cv::Point;
using cv::Rect;
using std::cout;
using std::endl;
using std::string;
using std::vector;

Codec::Codec()
{
    this->cols = 256;
    this->rows = 256;
    this->R = Mat::zeros(rows, cols, CV_8UC1);
    this->flag = Mat::zeros(rows, cols, CV_8UC1);
}

Codec::Codec(Mat& pic)
{
    this->pic = pic;
    this->cols = pic.cols;
    this->rows = pic.rows;
    this->R = Mat::zeros(rows, cols, CV_8UC1);
    this->flag = Mat::zeros(rows, cols, CV_8UC1);
    this->blockCount = 0;
}

Codec::~Codec()
{
}

Mat Codec::encode()
{
    Point& cur = this->lt;
    int isolatedCount = 0;
    while (findInitPoint(cur))
    {
        expandX();
        if (isIsolated(this->lt, this->rb))
        {
            handleIsolated();
            isolatedCount++;
            continue;
        }
        expandY();
        vector<Rect> overlapRects = findOverlapRects();
        if (overlapRects.empty())
        {
            handleNoOverlap();
        }
        else
        {
            handleOverlapRects(overlapRects);
        }
    }
    cout << "now isolatedCount is" << isolatedCount << endl;
    blockCount = count();
    // this->blockCount += this->hashmap.size();
    return R;
}



bool Codec::findInitPoint(Point& cur)
{
    while (!isOutOfRange(cur))
    {
        // 图片中为1且为未访问则可以为起始点
        if (isWhite(cur) && flag.at<uchar>(cur) == FLAG_TYPE::UNVISITED)
        {
            DEBUG_COUT << "Found an initPoint: " << cur << endl;
            return true;
        }
        // left to right
        cur.x = (cur.x + 1) % cols;
        if (cur.x == 0)
        {
            // top to bottom
            cur.y = cur.y + 1;
            if (isOutOfRange(cur))
            {
                cout << "END ENCODING" << std::endl;
                return false;
            }
        }
    }
    cout << "END ENCODING" << std::endl;
    return false;
}

void Codec::expandX()
{
    DEBUG_COUT << "In expandX" << endl;
    this->rb = this->lt;
    do
    {
        this->rb.x++;
    } while (!isOutOfRange(rb) && isWhite(rb) && flag.at<uchar>(rb) != FLAG_TYPE::OVERLAPPED);

    DEBUG_COUT << "end expandX loop" << endl;
    rb.x--;
    Point tempRb(rb.x + 1, rb.y + 1);
    this->rec = Rect(this->lt, tempRb);
    DEBUG_COUT << "Now lt:" << rec.tl() << endl;
    DEBUG_COUT << "Now rb:" << rec.br() << endl;
    return;
}

void Codec::expandY()
{
    DEBUG_COUT << "In expandY" << endl;
    bool isBlock = true;
    do
    {
        rb.y++;
        if (isOutOfRange(rb))
        {
            break;
        }
        for (int i = lt.x; i <= rb.x; i++)
        {
            if (isWhite(Point(i, rb.y)) && flag.at<uchar>(rb.y, i) != FLAG_TYPE::OVERLAPPED)
            {
                continue;
            }
            else
            {
                isBlock = false;
                break;
            }
        }

    } while (isBlock);
    DEBUG_COUT << "end expandY loop" << endl;
    rb.y--;
    Point tempRb(rb.x + 1, rb.y + 1);
    this->rec = Rect(this->lt,tempRb);
    DEBUG_COUT << "Now lt:" << rec.tl() << endl;
    DEBUG_COUT << "Now rb:" << rec.br() << endl;
    return;
}

vector<Rect> Codec::findOverlapRects()
{
    DEBUG_COUT_INFO;
    DEBUG_COUT << "lt: " << lt.x << " " << lt.y << std::endl;
    DEBUG_COUT << "rb: " << rb.x << " " << rb.y << std::endl;
    // 说明在第一行，第一行不会遇到重叠情况
    if (lt.y == 0)
    {
        DEBUG_COUT << "can't find overlap rect" << endl;
        return vector<Rect>();
    }

    vector<Rect> overlapRects;

    for (int i = lt.x; i <= rb.x; i++)
    {
        if (flag.at<uchar>(lt.y, i) == FLAG_TYPE::VISITED)
        {
            Rect RectV = findRectV(i, lt.y);
            overlapRects.push_back(RectV);
            i = RectV.br().x - 1; // jump to next area
        }
    }

    return overlapRects;
}

void Codec::handleNoOverlap()
{
    DEBUG_COUT_INFO;

    setFlag(this->rec, FLAG_TYPE::VISITED);
    setR(this->rec, NODE_TYPE::H_MATRIX);

    return;
}

void Codec::handleOverlapRects(vector<Rect> overlapRects)
{
    // 判断是否已经完全重叠

    bool isCross = false;

    for (auto RectV : overlapRects)
    {
        /*if (!isCross && isCrossOverlap(this->rec, RectV) && checkLaw2(RectV))*/
        if (!isCross && isCrossOverlap(this->rec, RectV) && checkLaw2(RectV))
        {
            isCross = true;
            setFlag(RectV, FLAG_TYPE::OVERLAPPED);
            setR(RectV, NODE_TYPE::V_MATRIX);

            saveOverlapArea(this->rec, RectV);
        }
        else
        {
            divideRectV(RectV);
        }
    }

    if (isCross)
    {
        setFlag(this->rec, FLAG_TYPE::OVERLAPPED);
        setR(this->rec, NODE_TYPE::H_MATRIX);
    }
    else
    {
        handleNoOverlap();
    }
}

Rect Codec::findRectV(int x, int y)
{
    DEBUG_COUT << "now we're finding from (x, y): " << x << ", " << y << endl;
    for (int i = y; i >= 0; i--)
    {
        DEBUG_COUT << "now we're finding from (x, y): " << x << ", " << i << endl;
        DEBUG_COUT << "R.at<uchar>(i, x) : " << int(R.at<uchar>(i, x)) << endl;
        if (R.at<uchar>(i, x) == NODE_TYPE::START)
        {
            Point ltV(x, i);
            if (hashmap.find(point2str(ltV)) == hashmap.end())
            {
                DEBUG_COUT << "Error: R exists a unmatch START point" << endl;
                exit(-2);
            }
            Point rbV = hashmap[point2str(ltV)];
            rbV.x++;
            rbV.y++;
            return Rect(ltV, rbV);
        }
    }
    DEBUG_COUT << "Error: can't find RectV" << endl;
    DEBUG_COUT_INFO;
    exit(-1);
    return Rect();
}

bool Codec::checkLaw2(Rect RectV)
{
    if (RectV.tl().y == 0)
    {
        DEBUG_COUT << "checkLaw2: true" << endl;
        return true;
    }

    for (int i = 0; i < RectV.tl().y; i++)
    {
        for (int j = RectV.tl().x; j < RectV.br().x; j++)
        {
            if (R.at<uchar>(i, j) == NODE_TYPE::START)
            {
                DEBUG_COUT << "checkLaw2: false" << endl;
                return false;
            }

        }
    }
    DEBUG_COUT << "checkLaw2: true" << endl;
    return true;
}

void Codec::divideRectV(Rect RectV)
{
    DEBUG_COUT_INFO;
    // 上面突出

    if (RectV.tl().y < this->rec.tl().y)
    {
        Point tempBr = RectV.br();
        tempBr.y = this->rec.tl().y;

        Rect newRectV(RectV.tl(), tempBr);
        setR(newRectV, NODE_TYPE::H_MATRIX);
    }

    //下面突出
    if (RectV.br().y > this->rec.br().y)
    {
        Point tempTl = RectV.tl();
        tempTl.y = this->rec.br().y;
        Point tempBr = RectV.br();
        Rect newRectV(tempTl, tempBr);
        setR(newRectV, NODE_TYPE::H_MATRIX);
    }
}

bool Codec::isCrossOverlap(Rect RectH, Rect RectV)
{
    if (RectH.br().x > RectV.br().x && RectH.br().y < RectV.br().y
        && RectH.tl().x < RectV.tl().x && RectH.tl().y > RectV.tl().y)
    {
        return true;
    }
    return false;
}

void Codec::saveOverlapArea(Rect RectH, Rect RectV)
{
    Point newTl(RectV.tl().x, RectH.tl().y);
    Point newBr(RectV.br().x, RectV.br().y);
    this->overlapArea.push_back(Rect(newTl, newBr));
    return;
}

std::string Codec::point2str(const Point& p)
{
    return std::to_string(p.x) + " " + std::to_string(p.y);
}

bool Codec::isWhite(Point p)
{
    return pic.at<uchar>(p) == WHITE;
}

void Codec::setFlag(Rect rec, FLAG_TYPE flagValue)
{
    this->flag(rec).setTo(static_cast<uchar>(flagValue));
    return;
}

void Codec::setR(Rect rec, NODE_TYPE nodeValue)
{
    Mat roi = R(rec);
    roi.setTo(0);
    this->R.at<uchar>(rec.tl()) = NODE_TYPE::START;
    Point tempBr = rec.br();
    tempBr.x--;
    tempBr.y--;
    this->R.at<uchar>(tempBr) = nodeValue;
    DEBUG_COUT << "---------------------------------------" << endl;
    DEBUG_COUT << "now we're setting R, lt: " << rec.tl() << "rb++: " << rec.br() << endl;

    hashmap[point2str(rec.tl())] = tempBr;
    return;
}

bool Codec::isOutOfRange(const Point& p)
{
    return (p.x >= cols || p.y >= rows);
}

int Codec::count()
{
    int res = 0;
    for (int i = 0; i < R.rows; i++)
    {
        for (int j = 0; j < R.cols; j++)
        {
            if (R.at<uchar>(i, j) == ISOLATED || R.at<uchar>(i, j) == START)
            {
                res++;
            }
        }
    }

    return res;
}

bool Codec::isIsolated(const Point& lt, const Point& rb)
{
    return lt == rb;
}

void Codec::handleIsolated()
{
    DEBUG_COUT << "In handleIsolated" << endl;
    R.at<uchar>(this->lt) = NODE_TYPE::ISOLATED;
    flag.at<uchar>(this->lt) = FLAG_TYPE::VISITED;
    return;
}