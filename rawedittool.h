#ifndef RAWEDITTOOL_H
#define RAWEDITTOOL_H
#include <QByteArray>
#include <QPoint>
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
};

#endif // RAWEDITTOOL_H
