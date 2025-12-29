#ifndef GTRANSFORM_H
#define GTRANSFORM_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QSpacerItem>
#include <QDial>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QImage>

class Gtransform : public QWidget
{
    Q_OBJECT

public:
    Gtransform(QWidget *parent = nullptr);
    ~Gtransform();
    QLabel      *inWin;
    QGroupBox   *mirrorGroup;
    QCheckBox   *hCheckBox;
    QCheckBox   *vCheckBox;
    QPushButton *mirrorButton;
    QDial       *rotateDial;
    QSpacerItem *vSpacer;
    QHBoxLayout *mainLayout;
    QVBoxLayout *groupLayout;
    QVBoxLayout *leftLayout;
    QImage      srcImg;
    QImage      dstImg;
    QPushButton *AButton;


private slots:
    void mirroredImage();
    void rotatedImage();
    void saveImage();

};
#endif // GTRANSFORM_H
