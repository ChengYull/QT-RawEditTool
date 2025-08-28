#include "widget.h"
#include "ui_widget.h"
#include "rawedittool.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("raw图编辑器 " + m_version);
    this->setLayout(ui->mainLayout);
    connect(ui->paintWidget, &PaintWidget::roiSelectStart, this, [this](const QPoint& point) {
        QString pos = QString("(%1, %2)").arg(point.x()).arg(point.y());
        ui->ROIStartPoint->setText(pos);
        m_roiStart = point;
    });
    connect(ui->paintWidget, &PaintWidget::roiSelectEnd, this, [this](const QPoint& point) {
        QString pos = QString("(%1, %2)").arg(point.x()).arg(point.y());
        ui->ROIEndPoint->setText(pos);
        m_roiEnd = point;
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_zoomInBtn_clicked()
{
    ui->paintWidget->zoomIn();
}


void Widget::on_zoomOutBtn_clicked()
{
    ui->paintWidget->zoomOut();
}


void Widget::on_chooseRawBtn_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                "请选择图片",
                                                "",
                                                "Images (*.raw;);");
    if (path.isEmpty()) {
        qDebug() << "用户取消选择";
        return;
    }
    QFileInfo fileInfo(path);

    QString fileName = fileInfo.fileName();
    m_fileBaseName = fileName.left(fileName.length() - 4);
    qDebug() << "名字：" << m_fileBaseName;
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return;
    }

    // 读取整个文件到QByteArray
    QByteArray rawData = file.readAll();
    int pixelSize = 0;
    if (fileName.contains("raw8")){
        pixelSize = 1;
    }else{
        pixelSize = 2;
    }

    QSize size = parseImageSize(fileName);
    ui->widthLabel->setText(QString::number(size.width()));
    ui->label_4->setText(QString::number(size.height()));
    qDebug() << "width:" << size.width() << ", height:" << size.height() << ", pixel size:" << pixelSize;
    if (size.isEmpty()) {
        QMessageBox::warning(nullptr, "文件大小不匹配", "图片尺寸与文件大小不匹配");
        qDebug() << "raw file is not named with size!";
        return;
    }
    if(!ui->paintWidget->setRawData(rawData, size.width(), size.height(), pixelSize)){
        QMessageBox::warning(nullptr, "加载图片失败", "请重新选择图片");
        qDebug() << "加载图片失败 ，请重新选择图片";
        return;
    }
}

QSize Widget::parseImageSize(const QString &fileName)
{
    QRegularExpression re("(\\d+)X(\\d+)");
    QRegularExpressionMatch match = re.match(fileName);
    if (match.hasMatch()) {
        return QSize(match.captured(1).toInt(), match.captured(2).toInt());
    }
    return QSize(0, 0); // 无效尺寸
}



void Widget::on_fillValueBtn_clicked()
{
    if(m_roiStart.isNull() || m_roiEnd.isNull()){
        qDebug() << "请先选择ROI区域...";
    }
    uchar fillValue = ui->fillValueSpinBox->value();
    QByteArray* rawData = ui->paintWidget->getRawData();
    if(rawData == nullptr) return;
    int rawWidth = ui->paintWidget->getRawWidth();
    RawEditTool::Get()->fillValueToROI(rawData, rawWidth, m_roiStart, m_roiEnd, fillValue);
    ui->paintWidget->update();
}


void Widget::on_saveBtn_clicked()
{
    QString savePath = "";
    // 获取时间戳取模100
    QString tag = QString::number(QDateTime::currentSecsSinceEpoch() % 100);
    QString defaultName = m_fileBaseName + tag + ".raw";
    savePath = QFileDialog::getSaveFileName(this,
                                            "请选择保存位置",
                                            defaultName,
                                            "Image Files (*.jpg;*.png);");
    if (savePath.isEmpty())
        return;

    if(!ui->paintWidget->saveRaw(savePath)){
        qDebug() << "保存失败：" << savePath;
    }
}


void Widget::on_resetBtn_clicked()
{
    ui->paintWidget->reset();
}


void Widget::on_gaussianBtn_clicked()
{
    int width = ui->paintWidget->getRawWidth();
    int height = ui->paintWidget->getRawHeight();
    QByteArray* rawData = ui->paintWidget->getRawData();
    if(rawData == nullptr) return;
    RawEditTool::Get()->Gaussian(rawData, width, height);
    ui->paintWidget->update();
}


