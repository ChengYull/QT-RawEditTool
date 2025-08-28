#ifndef RAWEDITTOOL_H
#define RAWEDITTOOL_H
#include <QByteArray>
#include <QPoint>

#include <opencv2/opencv.hpp>
class RawEditTool
{
public:
    // 单例模式
    static RawEditTool *Get(){
        static RawEditTool cp;
        return &cp;
    }

    // 填充ROI区域
    void fillValueToROI(QByteArray *rawData, const int &rawWidth, const QPoint &roiStart, const QPoint &roiEnd, const uchar &fillValue);

    cv::Mat RawToMat(QByteArray *rawData, const int &rawWidth, const int &rawHeight);
    QByteArray MatToRaw(const cv::Mat &mat);
    void Gaussian(QByteArray *rawData, const int &rawWidth, const int &rawHeight);
};

#endif // RAWEDITTOOL_H
