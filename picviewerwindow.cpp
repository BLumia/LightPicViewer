#include "picviewerwindow.h"
#include "ui_picviewerwindow.h"

PicViewerWindow::PicViewerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PicViewerWindow)
{
    ui->setupUi(this);
}

PicViewerWindow::~PicViewerWindow()
{
    delete ui;
}
