#include "autoreplywindows.h"
#include "ui_autoreplywindows.h"

AutoReplyWindows::AutoReplyWindows(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AutoReplyWindows)
{
    ui->setupUi(this);


    setWindowTitle("自动回复设置");


}

AutoReplyWindows::~AutoReplyWindows()
{
    delete ui;
}
