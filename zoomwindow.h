#ifndef ZOOMWINDOW_H
#define ZOOMWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QToolBar>
#include <QAction>
#include <QMouseEvent>
#include <QPainter>
#include <QSpinBox>
#include <QColorDialog>
#include <QScrollArea>

class ZoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    ZoomWindow(const QImage &image, QWidget *parent = nullptr);
    ~ZoomWindow();

private slots:
    void saveAs();
    void toggleDrawMode();
    void choosePenColor();
    void changePenWidth(int width);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void createActions();
    void createToolBars();
    void updateImageLabel();

    QImage img;
    QImage drawImg;
    QLabel *imgLabel;
    QScrollArea *scrollArea;
    QToolBar *toolbar;
    QAction *saveAsAction;
    QAction *drawModeAction;
    QAction *penColorAction;
    QSpinBox *penWidthSpinBox;
    
    bool drawMode;
    bool drawing;
    QPoint lastPoint;
    QColor penColor;
    int penWidth;
};

#endif // ZOOMWINDOW_H
