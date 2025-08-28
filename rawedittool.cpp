#include "rawedittool.h"

#include <QDebug>

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

cv::Mat RawEditTool::RawToMat(QByteArray *rawData, const int &rawWidth, const int &rawHeight){

    cv::Mat mat(rawHeight, rawWidth, CV_8UC1);
    std::memcpy(mat.data, rawData->constData(), rawData->size());
    return mat;
}
QByteArray RawEditTool::MatToRaw(const cv::Mat &mat){
    // 检查 Mat 是否是连续内存（确保可以直接拷贝）
    if (!mat.isContinuous()) {
        qDebug() << "Mat data is not continuous, need to clone first";
        return QByteArray();
    }
    // 计算数据大小（rows × cols × 每个元素的字节数）
    size_t dataSize = mat.total() * mat.elemSize();
    // 创建 QByteArray 并直接拷贝数据
    QByteArray rawData(dataSize, '\0');
    std::memcpy(rawData.data(), mat.data, dataSize);
    return rawData;
}

// 高斯模糊
void RawEditTool::Gaussian(QByteArray *rawData, const int &rawWidth, const int &rawHeight){
    cv::Mat rawMat = RawToMat(rawData, rawWidth, rawHeight);
    // 参数1：输入  参数2：输出
    cv::GaussianBlur(rawMat, rawMat, cv::Size(5, 5), 2, 2);
    (*rawData) = MatToRaw(rawMat);
}

