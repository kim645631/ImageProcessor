#ifndef IP_H
#define IP_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QImage>
#include <QLabel>
#include "gtransform.h"

class Ip : public QMainWindow
{
    Q_OBJECT

public:
    Ip(QWidget *parent = nullptr);
    ~Ip();
    void createActions();
    void createMenus();
    void createToolBars();
    void loadFile(QString filename);
private slots:
    void showOpenFile();
    void getZoomOut();
    void getZoomIn();
    void showGeometryTtansform();
private:
    Gtransform  *gWin;
    QWidget     *central;
    QMenu       *fileMeun;
    QToolBar    *fileTool;
    QToolBar    *toolTool;
    QImage      img;
    QString     filename;
    QLabel      *imgWin;
    QAction     *openFileAction;
    QAction     *exitAction;
    QAction     *zoomOut;
    QAction     *zoomIn;
    QAction     *geometryAction;
};
#endif // IP_H
