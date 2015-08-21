#include "picviewerwindow.h"
#include "ui_picviewerwindow.h"

PicViewerWindow::PicViewerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PicViewerWindow)
{
    ui->setupUi(this);

    clickOnFrame = false;//是否点击到面板上

    this->setWindowFlags(Qt::FramelessWindowHint);// | Qt::Tool
    this->setAttribute(Qt::WA_TranslucentBackground, true);//背景透明（需要上一行配合）

    picCenterPctX = 0.5;
    picCenterPctY = 0.5;

    movie = new QMovie(this);
    movie->setCacheMode(QMovie::CacheAll);
    currentMovieDirectory="movies";

    //connect(movie,SIGNAL(frameChanged(int)),this,SLOT(updateFrameSlider()));
    //connect(movie,SIGNAL(stateChanged(QMovie::MovieState)),this,SLOT(updateButtons()));
    //connect(frameSlider,SIGNAL(valueChanged(int)),this,SLOT(goToFrame(int)));
    //connect(speedSpinBox,SIGNAL(valueChanged(int)),movie,SLOT(setSpeed(int)));

    //动画部分，窗口渐隐
    fadeOutAnimation = new QPropertyAnimation(this, "windowOpacity");
    fadeOutAnimation->setDuration(300);
    fadeOutAnimation->setStartValue(1);
    fadeOutAnimation->setEndValue(0);
    //窗口调节大小动画
    floatUpAnimation = new QPropertyAnimation(this, "geometry");
    floatUpAnimation->setDuration(300);
    floatUpAnimation->setEasingCurve(QEasingCurve::OutCirc);
    //上面的两个动画组合并绑定动画完成后退出程序
    exitAnimationGroup = new QParallelAnimationGroup;
    exitAnimationGroup->addAnimation(fadeOutAnimation);
    exitAnimationGroup->addAnimation(floatUpAnimation);
    connect(exitAnimationGroup, SIGNAL(finished()), this, SLOT(close()));//动画效果结束后隐藏

    ui->viewerLabel->setMovie(movie);
    movie->start();
}

PicViewerWindow::~PicViewerWindow()
{
    delete ui;
    delete fadeOutAnimation;
    delete floatUpAnimation;
    delete exitAnimationGroup;
}

void PicViewerWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint gloPoint = event->globalPos();
    this->region(gloPoint);
    if (event->button() == Qt::LeftButton)
    {
        drag_pos = event->pos();
        clickOnFrame = true;//点击到窗口框架上才会变成true，改动之后新增功能：判断点击是否在边框右侧
        event->accept();
    }
}

void PicViewerWindow::mouseReleaseEvent(QMouseEvent *)
{
    clickOnFrame = false;//弹起鼠标按键时，恢复
    if(mouseDir != NONE) {
        this->releaseMouse();
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void PicViewerWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 无边框窗体的边缘拖动resize方案参考了如下博客的代码
    // http://www.cnblogs.com/xufeiyang/p/3313104.html
    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    if (event->buttons() & Qt::LeftButton && clickOnFrame)
    {

        if(mouseDir != NONE) {
            QRect rMove(tl, rb);

            switch(mouseDir) {
            case LEFT:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                break;
            case RIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                break;
            case UP:
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case DOWN:
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case LEFTTOP:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case RIGHTTOP:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setY(gloPoint.y());
                break;
            case LEFTBOTTOM:
                rMove.setX(gloPoint.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case RIGHTBOTTOM:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            default:
                break;
            }
            this->setGeometry(rMove);
        } else {
            move(event->globalPos() - drag_pos);
        }
        event->accept();
    }
}

void PicViewerWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void PicViewerWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty())
        return;

    if(fileName.toLower().endsWith(".jpg")||
            fileName.toLower().endsWith(".jpeg")||
            fileName.toLower().endsWith(".png")||
            fileName.toLower().endsWith(".gif")||
            fileName.toLower().endsWith(".bmp"))
    {
        openPic(fileName);//仅处理一个文件
    }
}

void PicViewerWindow::openPic(QString fileName) {
    currentMovieDirectory = QFileInfo(fileName).path();

    setWindowTitle(QFileInfo(fileName).fileName());

    movie->stop();
    ui->viewerLabel->setMovie(movie);
    movie->setFileName(fileName);
    movie->start();

    QPixmap img(fileName);

    ui->viewerLabel->resize(img.width(),img.height());
    ui->viewerLabel->setGeometry(getPicRect(1));//载入后更新图片位置

}

void PicViewerWindow::on_closeBtn_clicked()
{
    floatUpAnimation->setStartValue(QRect(this->geometry().x(), this->geometry().y(), this->geometry().width(), this->geometry().height()));
    floatUpAnimation->setEndValue(QRect(this->geometry().x(), this->geometry().y()-80, this->geometry().width(), this->geometry().height()));
    exitAnimationGroup->start();//动画完成之后，程序将退出
    //qApp->exit();//主窗口关闭后，关闭程序
}

void PicViewerWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->accept();
    floatUpAnimation->setStartValue(QRect(this->geometry().x(), this->geometry().y(), this->geometry().width(), this->geometry().height()));
    floatUpAnimation->setEndValue(QRect(this->geometry().x(), this->geometry().y()-80, this->geometry().width(), this->geometry().height()));
    exitAnimationGroup->start();//动画完成之后，程序将退出
    //qApp->exit();//主窗口关闭后，关闭程序
}

void PicViewerWindow::region(const QPoint &cursorGlobalPoint)
{
    // 获取窗体在屏幕上的位置区域，tl为topleft点，rb为rightbottom点
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
        mouseDir = LEFTTOP;// 左上角
        this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
    } else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        mouseDir = RIGHTBOTTOM;// 右下角
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        mouseDir = LEFTBOTTOM;//左下角
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
        mouseDir = RIGHTTOP;// 右上角
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x()) {
        mouseDir = LEFT;// 左边
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - PADDING) {
        mouseDir = RIGHT;// 右边
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= tl.y() && y <= tl.y() + PADDING){
        mouseDir = UP;// 上边
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - PADDING) {
        mouseDir = DOWN;// 下边
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else {
        mouseDir = NONE;// 默认
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void PicViewerWindow::resizeEvent(QResizeEvent *)
{
    ui->closeBtn->setGeometry(width()- 29, 5, 24, 24);
    ui->optionPanel->setGeometry(width()/2 - ui->optionPanel->width()/2, height()-40, 340, 40);
    ui->viewerLabel->setGeometry(getPicRect(1));
}

QRect PicViewerWindow::getPicRect(double zoomPct) {
    //暂时忽略缩放比例
    QRect rect;
    rect.setLeft(width()*picCenterPctX - ui->viewerLabel->width()/2);
    rect.setTop(height()*picCenterPctY - ui->viewerLabel->height()/2);
    rect.setWidth(ui->viewerLabel->width());
    rect.setHeight(ui->viewerLabel->height());
    return rect;
}
