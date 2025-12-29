#include "ip.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include "gtransform.h"
#include "mouseevent.h"

Ip::Ip(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("影像處理"));
    central = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    imgWin = new QLabel();
    QPixmap *initPixmap = new QPixmap(300,200);
    gWin = new Gtransform();
    initPixmap->fill(QColor(255,255,255));
    imgWin->resize(300,200);
    imgWin->setScaledContents(true);
    imgWin->setPixmap(*initPixmap);
    mainLayout->addWidget(imgWin);
    setCentralWidget(central);
    setMouseTracking(true);
    createActions();
    createMenus();
    createToolBars();
}

Ip::~Ip() {

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
}
void Ip::createToolBars(){
    fileTool = addToolBar("file");
    fileTool->addAction(openFileAction);
    toolTool = addToolBar("file");
    toolTool->addAction(zoomOut);
    toolTool->addAction(zoomIn);
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
    mousePosLabel->setText(str);
}
void Ip::mousePressEvent(QMouseEvent *event){
    QString str ="(" + QString::number(event->x()) + ", " +
                  QString::number(event->y()) + ")";
    if(event->button()==Qt::LeftButton){
        statusBar()->showMessage(tr("左鍵:")+str);
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
    statusBar()->showMessage(tr("釋放:")+str);
    qDebug()<<"釋放";
}
