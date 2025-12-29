#include "gtransform.h"
#include <QPixmap>
#include <QPainter>
#include <QSizePolicy>
#include <QFileDialog>
#include <QMessageBox>

Gtransform::Gtransform(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);
    leftLayout = new QVBoxLayout(this);
    mirrorGroup = new QGroupBox(tr("鏡射"),this);
    groupLayout = new QVBoxLayout(mirrorGroup);

    hCheckBox = new QCheckBox(tr("水平"),mirrorGroup);
    vCheckBox = new QCheckBox(tr("垂直"),mirrorGroup);
    mirrorButton = new QPushButton(tr("執行"),mirrorGroup);
    AButton = new QPushButton(tr("存檔"),this);
    hCheckBox->setGeometry(QRect(13,28,87,19));
    vCheckBox->setGeometry(QRect(13,54,87,19));
    mirrorButton->setGeometry(QRect(13,80,93,28));
    AButton->setGeometry(QRect(13,250,93,28));
    groupLayout->addWidget(hCheckBox);
    groupLayout->addWidget(vCheckBox);
    groupLayout->addWidget(mirrorButton);
    leftLayout->addWidget(mirrorGroup);
    leftLayout->addWidget(AButton);
    rotateDial = new QDial(this);
    rotateDial->setNotchesVisible(true);
    vSpacer = new QSpacerItem(20,58,QSizePolicy::Minimum,
                              QSizePolicy::Expanding);
    leftLayout->addWidget(rotateDial);
    leftLayout->addItem(vSpacer);

    mainLayout->addLayout(leftLayout);

    inWin = new QLabel(this);
    inWin->setScaledContents(true);
    QPixmap *initPixmap = new QPixmap(300,200);
    initPixmap->fill(QColor(255,255,255));
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    QPainter *paint = new QPainter(initPixmap);
    paint->setPen(*(new QColor(0,0,0)));
    paint->begin(initPixmap);
    paint->drawRect(15,15,60,40);
    paint->end();
    if(srcImg.isNull()){
        srcImg = initPixmap->toImage();
    }

    inWin->setPixmap(*initPixmap);
    inWin->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    mainLayout->addWidget(inWin);
    connect(mirrorButton,SIGNAL(clicked()),this,SLOT(mirroredImage()));
    connect(rotateDial,SIGNAL(valueChanged(int)),this,SLOT(rotatedImage()));
    connect(AButton, SIGNAL(clicked()), this, SLOT(saveImage()));

}

Gtransform::~Gtransform() {

}
void Gtransform:: mirroredImage(){
    bool H,V;
    if(srcImg.isNull())
        return;
    H = hCheckBox->isChecked();
    V = vCheckBox->isChecked();
    dstImg = srcImg.mirrored(H,V);
    inWin->setPixmap(QPixmap::fromImage(dstImg));
    srcImg = dstImg;
}
void Gtransform:: rotatedImage(){

    QTransform tran;
    int angle;
    if(srcImg.isNull())
        return;
    angle = rotateDial->value();
    tran.rotate(angle);
    dstImg = srcImg.transformed(tran);
    inWin->setPixmap(QPixmap::fromImage(dstImg));
}
void Gtransform::saveImage()
{
    if (dstImg.isNull()) {
        QMessageBox::warning(this, "存檔失敗", "目前沒有可儲存的圖片！");
        return;
    }

    QString savePath = QFileDialog::getSaveFileName(
        this,
        "儲存圖片",
        "",
        "Images (*.png *.jpg *.bmp)"
        );

    if (savePath.isEmpty()) return;

    if (!dstImg.save(savePath)) {
        QMessageBox::critical(this, "存檔失敗", "無法存檔至指定位置！");
    } else {
        QMessageBox::information(this, "存檔成功", QString("圖片成功保存至: %1").arg(savePath));
    }
}
