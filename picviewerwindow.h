#ifndef PICVIEWERWINDOW_H
#define PICVIEWERWINDOW_H

#include <QMainWindow>

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
};

#endif // PICVIEWERWINDOW_H
