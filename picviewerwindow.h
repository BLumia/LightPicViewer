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
    int mouseDir;//鼠标在窗口边缘时的标记
    bool clickOnFrame;
    QString currentMovieDirectory;
    QPoint drag_pos;//用于窗口拖动，存储鼠标坐标
    void openPic(QString fileName);
    void region(const QPoint &cursorGlobalPoint);

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
