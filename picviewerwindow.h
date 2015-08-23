#ifndef PICVIEWERWINDOW_H
#define PICVIEWERWINDOW_H

#include <QtWidgets>
#include <QMovie>
#include <QMainWindow>

#define PADDING 2
enum Direction {
    UP=0, DOWN=1, LEFT, RIGHT,
    LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP,
    NONE
};

namespace Ui {
class PicViewerWindow;
}

class PicViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PicViewerWindow(QWidget *parent = 0);
    ~PicViewerWindow();

private:
    Ui::PicViewerWindow *ui;

    QMovie *movie;
    double picCenterPctX;
    double picCenterPctY;
    int mouseDir;//鼠标在窗口边缘时的标记
    bool clickOnFrame;
    QString currentMovieDirectory;
    QPoint drag_pos;//用于窗口拖动，存储鼠标坐标
    QPoint scrollBarValue;//存储两个滚动条的数值用于拖动
    QScrollBar *vScrollBar;
    QScrollBar *hScrollBar;
    QPropertyAnimation *fadeOutAnimation;
    QPropertyAnimation *floatUpAnimation;
    QParallelAnimationGroup *exitAnimationGroup;
    void openPic(QString fileName);
    void region(const QPoint &cursorGlobalPoint);
    QRect getPicRect(double zoomPct);

private slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void on_closeBtn_clicked();

protected:
    void resizeEvent(QResizeEvent *);
    void dragEnterEvent(QDragEnterEvent *event);//拖放相关
    void dropEvent(QDropEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // PICVIEWERWINDOW_H
