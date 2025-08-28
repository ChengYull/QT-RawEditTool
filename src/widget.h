#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_zoomInBtn_clicked();

    void on_zoomOutBtn_clicked();

    void on_chooseRawBtn_clicked();

    void on_fillValueBtn_clicked();

    void on_gaussianBtn_clicked();

    void on_resetBtn_clicked();

    void on_saveBtn_clicked();

private:
    Ui::Widget *ui;
    QSize parseImageSize(const QString &fileName);
    QPoint m_roiStart;
    QPoint m_roiEnd;
    QString m_fileBaseName;
    QString m_version = "v1.0.0";
};
#endif // WIDGET_H
