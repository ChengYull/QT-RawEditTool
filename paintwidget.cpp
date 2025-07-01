#include "paintwidget.h"
#include "ui_paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PaintWidget)
{
    ui->setupUi(this);
}

PaintWidget::~PaintWidget()
{
    delete ui;
}

bool PaintWidget::setRawData(QByteArray &rawData, int width, int height, int pixelSize){
    bool bRes = false;
    if(rawData.isEmpty() || width <= 0 || height <= 0 || pixelSize <= 0){
        qDebug() << "set Raw data failed, empty data or wrong width/height";
        return bRes;
    }
    if(rawData.size() != width * height * pixelSize){
        qDebug() << "set Raw data failed, width/height and pixel size set wrong with raw size";
        return bRes;
    }
    if(pixelSize == 2){
        QByteArray tmpData;
        for(int i = 0; i < rawData.size(); i += 2){
            quint16 rawValue = rawData[i + 1] << 8 | rawData[i];
            // 归一化到0-255
            uchar val = rawValue * (255.0 / 1023.0);
            tmpData.append(val);
        }
        rawData = tmpData;
    }
    m_rawData = rawData;
    m_rawDataRoot = rawData;
    m_iRawWidth = width;
    m_iRawHeight = height;
    m_iPixelSize = pixelSize;
    m_isRawLoaded = true;
    bRes = true;
    update();
    return bRes;
}
void PaintWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    if (m_isRawLoaded) {
        painter.save();  // 保存当前状态
        painter.translate(m_drawPoint);
        painter.scale(m_scale, m_scale);

        // 获取从设备坐标(像素)到逻辑坐标(图像坐标)的变换
        QTransform transform = painter.transform().inverted();

        // 将widget的矩形(像素坐标)映射到图像坐标空间
        QRect visibleRect = transform.mapRect(QRect(0, 0, width(), height()));

        // 对齐到整数像素边界
        int startX = qMax(0, visibleRect.left());
        int startY = qMax(0, visibleRect.top());
        int endX = qMin(m_iRawWidth-1, visibleRect.right());
        int endY = qMin(m_iRawHeight-1, visibleRect.bottom());

        int step = 1;
        if(m_scale <= 4)
            step = 2;
        else if(m_scale <= 2)
            step = 4;
        else if(m_scale <= 1)
            step = 8;
        else{
            for (int y = startY; y <= endY; y++) {
                for (int x = startX; x <= endX; x++) {
                    int idx = x + y * m_iRawWidth;
                    uchar pixelValue = m_rawData.at(idx);
                    painter.setPen(QColor(pixelValue, pixelValue, pixelValue));
                    painter.drawPoint(x, y);
                }
            }
            painter.restore();  // 恢复状态
            return;
        }
        // 只渲染可见部分
        for (int y = startY; y <= endY; y += step) {
            for (int x = startX; x <= endX; x += step) {
                int idx = x + y * m_iRawWidth;
                uchar pixelValue = m_rawData.at(idx);
                painter.fillRect(x, y, step, step, QColor(pixelValue, pixelValue, pixelValue));
            }
        }
        painter.restore();  // 恢复状态
    }
}

void PaintWidget::setZoom(float scale){
    if (scale > 0.1 && scale < 20.0) {
        m_scale = scale;
        update();
    }
}
void PaintWidget::zoomIn(){
    setZoom(m_scale * 1.2);
}
void PaintWidget::zoomOut(){
    setZoom(m_scale / 1.2);
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isRawLoaded && m_isMousePress) {
        QPoint delta = event->pos() - m_mousePoint;
        m_mousePoint = event->pos();
        m_drawPoint += delta;
        update();
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{

    if (m_isRawLoaded && event->button() == Qt::LeftButton) {
        m_isMousePress = true;
        m_mousePoint = event->pos();
    }

    if (m_isRawLoaded && event->button() == Qt::RightButton) {
        emit roiSelectStart(getPixelPosFromMouse(event->pos()));
    }
    m_currentPoint = getPixelPosFromMouse(event->pos());
    qDebug() << "Pixel at:" << m_currentPoint;
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isRawLoaded && event->button() == Qt::LeftButton)
        m_isMousePress = false;

    if (m_isRawLoaded && event->button() == Qt::RightButton) {
        emit roiSelectEnd(getPixelPosFromMouse(event->pos()));
    }
}


void PaintWidget::wheelEvent(QWheelEvent *event)
{
    int flag = event->angleDelta().y();
    double oldScale = m_scale;
    m_scale *= (flag > 0) ? 1.2 : 0.8;

    // 限制缩放范围
    m_scale = qBound(0.05, m_scale, 20.0);
    QPoint pos = event->position().toPoint();
    if (rect().contains(pos)) {
        // 计算缩放中心点
        double ratio = m_scale / oldScale;
        double newX = pos.x() - (pos.x() - m_drawPoint.x()) * ratio;
        double newY = pos.y() - (pos.y() - m_drawPoint.y()) * ratio;
        m_drawPoint = QPoint(newX, newY);
    } else {
        // 以图片中心缩放
        QPoint oldCenter = rect().center();
        double newWidth = width() * m_scale;
        double newHeight = height() * m_scale;
        m_drawPoint = QPoint(oldCenter.x() - newWidth / 2, oldCenter.y() - newHeight / 2);
    }

    update();
}

// 获取鼠标所在像素坐标
QPoint PaintWidget::getPixelPosFromMouse(const QPoint& mousePos) {
    if (!m_isRawLoaded) return QPoint(-1, -1);

    // 转换为图像坐标系（考虑平移和缩放）
    QPointF imagePos = QPointF(mousePos - m_drawPoint) / m_scale;

    // 确保坐标在图像范围内
    int x = qBound(0, (int)imagePos.x(), m_iRawWidth - 1);
    int y = qBound(0, (int)imagePos.y(), m_iRawHeight - 1);

    return QPoint(x, y);
}

// 重置图像
void PaintWidget::reset(){
    m_rawData = m_rawDataRoot;
    update();
}

// 获取raw图数据
QByteArray* PaintWidget::getRawData(){
    return &m_rawData;
}
void PaintWidget::operateRaw(const QByteArray &rawData){
    m_rawData = rawData;
    update();
}

int PaintWidget::getRawWidth(){
    return m_iRawWidth;
}
