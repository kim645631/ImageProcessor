#include "ip.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include "gtransform.h"
#include "mouseevent.h"
#include "zoomwindow.h"
#include <QInputDialog>

Ip::Ip(QWidget *parent)
    : QMainWindow(parent), selectMode(false), selecting(false), rubberBand(nullptr)
{
    statusLabel = new QLabel;
    statusLabel->setText(tr("指標位置:"));
    statusLabel->setFixedWidth(100);
    MousePosLabel = new QLabel;
    MousePosLabel->setText(tr(" "));
    MousePosLabel->setFixedWidth(200);
    statusBar()->addPermanentWidget(statusLabel);
    statusBar()->addPermanentWidget(MousePosLabel);
    setMouseTracking(true);

    setWindowTitle(tr("影像處理"));
    central = new QWidget();
    central->setMouseTracking(true);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    imgWin = new QLabel();
    imgWin->setMouseTracking(true);
    QPixmap *initPixmap = new QPixmap(300,200);
    gWin = new Gtransform();
    initPixmap->fill(QColor(255,255,255));
    imgWin->resize(300,200);
    imgWin->setScaledContents(true);
    imgWin->setPixmap(*initPixmap);
    mainLayout->addWidget(imgWin);
    setCentralWidget(central);
    createActions();
    createMenus();
    createToolBars();
}

Ip::~Ip() {
    if (rubberBand) {
        delete rubberBand;
    }
}
void Ip::createActions(){
    openFileAction =new QAction(tr("開啟檔案&O"),this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(tr("開啟影像檔案"));
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(showOpenFile()));
    zoomOut = new QAction(tr("縮小"));
    zoomOut->setStatusTip(tr("縮小影像"));
    connect(zoomOut,SIGNAL(triggered()),this,SLOT(getZoomOut()));
    zoomIn = new QAction(tr("放大"));
    zoomIn->setStatusTip(tr("放大影像"));
    connect(zoomIn,SIGNAL(triggered()),this,SLOT(getZoomIn()));
    geometryAction = new QAction(tr("幾何轉換"));
    geometryAction->setStatusTip(tr("影像幾何轉換"));
    connect(geometryAction,SIGNAL(triggered()),this,SLOT(showGeometryTtansform()));
    selectModeAction = new QAction(tr("選取放大"));
    selectModeAction->setCheckable(true);
    selectModeAction->setStatusTip(tr("拖移選取範圍放大照片"));
    connect(selectModeAction,SIGNAL(triggered()),this,SLOT(toggleSelectMode()));
    exitAction =new QAction(tr("結束&Q"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("退出程式"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));
}
void Ip::createMenus(){
    fileMeun = menuBar()->addMenu(tr("檔案&F"));
    fileMeun->addAction(openFileAction);
    fileMeun->addAction(geometryAction);
    fileMeun->addAction(exitAction);
    fileMeun = menuBar()->addMenu(tr("工具&T"));
    fileMeun->addAction(zoomOut);
    fileMeun->addAction(zoomIn);
    fileMeun->addAction(selectModeAction);
}
void Ip::createToolBars(){
    fileTool = addToolBar("file");
    fileTool->addAction(openFileAction);
    toolTool = addToolBar("file");
    toolTool->addAction(zoomOut);
    toolTool->addAction(zoomIn);
    toolTool->addAction(selectModeAction);
    toolTool->addAction(geometryAction);

}
void Ip::loadFile(QString filename){
    qDebug()<<QString("file name:%1").arg(filename);
    QByteArray ba=filename.toLatin1();
    printf("FN:%s\n",(char *) ba.data());
    img.load(filename);
    imgWin->setPixmap(QPixmap::fromImage(img));
}
void Ip::showOpenFile(){
    filename = QFileDialog::getOpenFileName(this,tr("開啟影像"),tr("."),
                                            "png(*.png);;bmp(*.bmp)"
                                            ";;Jpg(*.jpg)");
    if(!filename.isEmpty())
    {
        if(img.isNull())
        {
            loadFile(filename);
        }
        else
        {
            Ip *newIPWin = new Ip();
            newIPWin->show();
            newIPWin->loadFile(filename);
        }
    }
}
void Ip::getZoomOut()
{
    QImage zoomOuted;
    zoomOuted =img.scaled(img.width()/2,img.height()/2);
    QLabel *ret=new QLabel();
    ret->setPixmap(QPixmap::fromImage(zoomOuted));
    ret->setWindowTitle(tr("縮小結果"));
    ret->show();


}
void Ip::getZoomIn()
{
    QImage zoomIned;
    zoomIned =img.scaled(img.width()*2,img.height()*2);
    QLabel *ret=new QLabel();
    ret->setPixmap(QPixmap::fromImage(zoomIned));
    ret->setWindowTitle(tr("放大結果"));
    ret->show();


}
void Ip::showGeometryTtansform(){
    if (!img.isNull()){
        gWin->srcImg = img;
        gWin->inWin->setPixmap(QPixmap::fromImage(gWin->srcImg));
        gWin->show();
    }
}
void Ip::mouseMoveEvent(QMouseEvent *event){
    QPointF pos = event->position();
    this->x = static_cast<int>(pos.x());
    this->y = static_cast<int>(pos.y());
    QString str ="(" + QString::number(event->x()) + ", " +
                  QString::number(event->y()) + ")";
    if (!img.isNull() && this->x >= 0 && this->x < img.width() && this->y >= 0 && this->y < img.height()) {
        int gray = qGray(img.pixel(this->x, this->y));
        str += " Gray: (" + QString::number(gray) + ")";
    }
    MousePosLabel->setText(str);
    
    // Handle selection mode
    if (selectMode && selecting && rubberBand) {
        rubberBand->setGeometry(QRect(selectionStart, event->pos()).normalized());
    }
}
void Ip::mousePressEvent(QMouseEvent *event){
    QString str ="(" + QString::number(event->x()) + ", " +
                  QString::number(event->y()) + ")";
    if(event->button()==Qt::LeftButton){
        if (selectMode && !img.isNull()) {
            selecting = true;
            selectionStart = event->pos();
            if (!rubberBand) {
                rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
            }
            rubberBand->setGeometry(QRect(selectionStart, QSize()));
            rubberBand->show();
        } else {
            statusBar()->showMessage(tr("左鍵:")+str);
        }
    }
    else if(event->button()==Qt::RightButton){
        statusBar()->showMessage(tr("右鍵:")+str);
    }
    else if(event->button()==Qt::MiddleButton){
        statusBar()->showMessage(tr("中鍵:")+str);
    }
    qDebug()<<"按壓";
}
void Ip::mouseReleaseEvent(QMouseEvent *event){
    QString str ="(" + QString::number(event->x()) + ", " +
                  QString::number(event->y()) + ")";
    
    if (selectMode && selecting && event->button() == Qt::LeftButton) {
        selecting = false;
        selectionEnd = event->pos();
        
        if (rubberBand) {
            rubberBand->hide();
        }
        
        // Calculate selection rectangle
        QRect selectionRect = QRect(selectionStart, selectionEnd).normalized();
        
        // Check if selection is valid
        if (selectionRect.width() > 10 && selectionRect.height() > 10 && !img.isNull()) {
            // Get zoom ratio from user
            bool ok;
            double zoomRatio = QInputDialog::getDouble(this, tr("放大倍率"),
                                                       tr("請輸入放大倍率:"), 2.0, 0.1, 10.0, 1, &ok);
            
            if (ok) {
                // Map selection to image coordinates
                QRect imgRect = imgWin->geometry();
                double scaleX = (double)img.width() / imgWin->width();
                double scaleY = (double)img.height() / imgWin->height();
                
                // Calculate the position relative to imgWin
                QPoint localStart = imgWin->mapFrom(this, selectionStart);
                QPoint localEnd = imgWin->mapFrom(this, selectionEnd);
                
                int imgX = qMax(0, qMin((int)(localStart.x() * scaleX), img.width()));
                int imgY = qMax(0, qMin((int)(localStart.y() * scaleY), img.height()));
                int imgW = qMax(1, qMin((int)((localEnd.x() - localStart.x()) * scaleX), img.width() - imgX));
                int imgH = qMax(1, qMin((int)((localEnd.y() - localStart.y()) * scaleY), img.height() - imgY));
                
                // Extract and zoom selected area
                QImage selectedArea = img.copy(imgX, imgY, imgW, imgH);
                QImage zoomedImage = selectedArea.scaled(
                    (int)(selectedArea.width() * zoomRatio),
                    (int)(selectedArea.height() * zoomRatio),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                );
                
                // Open zoom window
                ZoomWindow *zoomWin = new ZoomWindow(zoomedImage);
                zoomWin->setAttribute(Qt::WA_DeleteOnClose);
                zoomWin->show();
            }
        }
        
        statusBar()->showMessage(tr("釋放:")+str);
    } else {
        statusBar()->showMessage(tr("釋放:")+str);
    }
    qDebug()<<"釋放";
}
void Ip::toggleSelectMode(){
    selectMode = selectModeAction->isChecked();
    if (selectMode) {
        setCursor(Qt::CrossCursor);
        statusBar()->showMessage(tr("選取放大模式已啟用 - 拖移選取要放大的區域"));
    } else {
        setCursor(Qt::ArrowCursor);
        statusBar()->showMessage(tr("選取放大模式已停用"));
        if (rubberBand) {
            rubberBand->hide();
        }
        selecting = false;
    }
}
