#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QImage>
namespace Ui {
class PaintWidget;
}

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = nullptr);
    ~PaintWidget();
    void zoomIn();
    void zoomOut();
    bool setRawData(QByteArray &rawData, int width, int height, int pixelSize);
    bool m_isRawLoaded = false;
    QPoint m_currentPoint;
    void reset();
    QByteArray* getRawData();
    void operateRaw(const QByteArray &rawData);
    int getRawWidth();
    int getRawHeight();
    bool saveRaw(const QString &savePath);
private:
    Ui::PaintWidget *ui;
    int m_iRawWidth;    // raw图宽度
    int m_iRawHeight;   // raw图高度
    int m_iPixelSize;   // raw图单个像素字节大小
    float m_scale = 1.0;// 缩放倍数
    QByteArray m_rawDataRoot = nullptr; // 源数据备份
    QByteArray m_rawData = nullptr;   // raw图数据

    void setZoom(float scale);  // 设置缩放倍数
    bool m_isMousePress = false;// 鼠标是否按下
    QPoint m_mousePoint;    // 鼠标位置
    QPoint m_drawPoint;     // 绘制起点

    QPoint getPixelPosFromMouse(const QPoint& mousePos);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

signals:
    void roiSelectStart(QPoint roiStart);
    void roiSelectEnd(QPoint roiEnd);

};

#endif // PAINTWIDGET_H
