#include "rawedittool.h"



void RawEditTool::fillValueToROI(QByteArray *rawData, const int &rawWidth,
                                 const QPoint &roiStart,const QPoint &roiEnd, const uchar &fillValue){
    int startX = roiStart.x();
    int startY = roiStart.y();
    int endX = roiEnd.x();
    int endY = roiEnd.y();

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            int idx = x + y * rawWidth;
            (*rawData)[idx] = fillValue;
        }
    }

}
