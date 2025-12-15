#include "ip.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
Ip::Ip(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("影像處理"));
    central = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    imgWin = new QLabel();
    QPixmap *initPixmap = new QPixmap(300,200);
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
    exitAction =new QAction(tr("結束&Q"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("退出程式"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));
}
void Ip::createMenus(){
    fileMeun = menuBar()->addMenu(tr("檔案&F"));
    fileMeun->addAction(openFileAction);
    fileMeun->addAction(exitAction);
}
void Ip::createToolBars(){
    fileTool = addToolBar("file");
    fileTool->addAction(openFileAction);
    toolTool = addToolBar("file");
    toolTool->addAction(zoomOut);
    toolTool->addAction(zoomIn);
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
