#include "zoomwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QScrollArea>
#include <QLabel>
#include <QStatusBar>

ZoomWindow::ZoomWindow(const QImage &image, QWidget *parent)
    : QMainWindow(parent), img(image), drawMode(false), drawing(false),
      penColor(Qt::red), penWidth(3)
{
    setWindowTitle(tr("放大視窗"));
    
    // Create a copy for drawing
    drawImg = img.copy();
    
    // Create central widget with scroll area
    scrollArea = new QScrollArea(this);
    imgLabel = new QLabel();
    imgLabel->setScaledContents(false);
    imgLabel->setMouseTracking(true);
    updateImageLabel();
    
    scrollArea->setWidget(imgLabel);
    scrollArea->setWidgetResizable(false);
    setCentralWidget(scrollArea);
    
    createActions();
    createToolBars();
    
    resize(800, 600);
}

ZoomWindow::~ZoomWindow()
{
}

void ZoomWindow::createActions()
{
    saveAsAction = new QAction(tr("另存新檔"), this);
    saveAsAction->setStatusTip(tr("儲存放大後的影像"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    
    drawModeAction = new QAction(tr("畫筆模式"), this);
    drawModeAction->setCheckable(true);
    drawModeAction->setStatusTip(tr("啟用/停用畫筆模式"));
    connect(drawModeAction, SIGNAL(triggered()), this, SLOT(toggleDrawMode()));
    
    penColorAction = new QAction(tr("筆刷顏色"), this);
    penColorAction->setStatusTip(tr("選擇筆刷顏色"));
    connect(penColorAction, SIGNAL(triggered()), this, SLOT(choosePenColor()));
}

void ZoomWindow::createToolBars()
{
    toolbar = addToolBar(tr("工具"));
    toolbar->addAction(saveAsAction);
    toolbar->addSeparator();
    toolbar->addAction(drawModeAction);
    toolbar->addAction(penColorAction);
    
    // Add pen width spinbox
    QLabel *widthLabel = new QLabel(tr(" 筆刷寬度: "));
    toolbar->addWidget(widthLabel);
    penWidthSpinBox = new QSpinBox();
    penWidthSpinBox->setMinimum(1);
    penWidthSpinBox->setMaximum(50);
    penWidthSpinBox->setValue(penWidth);
    connect(penWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changePenWidth(int)));
    toolbar->addWidget(penWidthSpinBox);
}

void ZoomWindow::saveAs()
{
    QString savePath = QFileDialog::getSaveFileName(
        this,
        tr("儲存圖片"),
        "",
        "Images (*.png *.jpg *.bmp)"
    );
    
    if (savePath.isEmpty()) return;
    
    if (!drawImg.save(savePath)) {
        QMessageBox::critical(this, tr("存檔失敗"), tr("無法存檔至指定位置！"));
    } else {
        QMessageBox::information(this, tr("存檔成功"), QString(tr("圖片成功保存至: %1")).arg(savePath));
    }
}

void ZoomWindow::toggleDrawMode()
{
    drawMode = drawModeAction->isChecked();
    if (drawMode) {
        imgLabel->setCursor(Qt::CrossCursor);
        statusBar()->showMessage(tr("畫筆模式已啟用"));
    } else {
        imgLabel->setCursor(Qt::ArrowCursor);
        statusBar()->showMessage(tr("畫筆模式已停用"));
    }
}

void ZoomWindow::choosePenColor()
{
    QColor color = QColorDialog::getColor(penColor, this, tr("選擇筆刷顏色"));
    if (color.isValid()) {
        penColor = color;
        statusBar()->showMessage(tr("筆刷顏色已更改"));
    }
}

void ZoomWindow::changePenWidth(int width)
{
    penWidth = width;
}

void ZoomWindow::mousePressEvent(QMouseEvent *event)
{
    if (drawMode && event->button() == Qt::LeftButton) {
        // Map from main window to scroll area viewport, then to imgLabel
        QPoint viewportPos = scrollArea->viewport()->mapFrom(this, event->pos());
        QPoint labelPos = imgLabel->mapFrom(scrollArea->viewport(), viewportPos);
        
        // Check if click is within the label bounds
        if (labelPos.x() >= 0 && labelPos.y() >= 0 && 
            labelPos.x() < drawImg.width() && labelPos.y() < drawImg.height()) {
            drawing = true;
            lastPoint = labelPos;
        }
    }
}

void ZoomWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (drawMode && drawing) {
        // Map from main window to scroll area viewport, then to imgLabel
        QPoint viewportPos = scrollArea->viewport()->mapFrom(this, event->pos());
        QPoint labelPos = imgLabel->mapFrom(scrollArea->viewport(), viewportPos);
        
        // Check if position is within the image bounds
        if (labelPos.x() >= 0 && labelPos.y() >= 0 && 
            labelPos.x() < drawImg.width() && labelPos.y() < drawImg.height()) {
            // Draw on the image
            QPainter painter(&drawImg);
            painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(lastPoint, labelPos);
            
            lastPoint = labelPos;
            updateImageLabel();
        }
    }
}

void ZoomWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (drawMode && event->button() == Qt::LeftButton) {
        drawing = false;
    }
}

void ZoomWindow::updateImageLabel()
{
    imgLabel->setPixmap(QPixmap::fromImage(drawImg));
    imgLabel->adjustSize();
}
