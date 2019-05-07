#include "autoreplywindows.h"
#include "ui_autoreplywindows.h"

AutoReplyWindows::AutoReplyWindows(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AutoReplyWindows)
{
    ui->setupUi(this);

    setWindowTitle("自动回复设置");
    for(int i = 0;i<ui->tableWidget->rowCount ();i++)
    {
        QCheckBox *check_box=new QCheckBox;

        check_box->setCheckState (Qt::Unchecked);
        ui->tableWidget->setCellWidget(i,0,check_box); //插入复选框
        connect((QCheckBox*)ui->tableWidget->cellWidget (i,0 ),SIGNAL(stateChanged(int)),this,SLOT(close()));

    }
    //connect(ui->pushButton, SIGNAL(clicked()), this,SLOT(close()));
   //connect(ui->pushButton, SIGNAL(clicked()), this,SLOT(test()));

}

AutoReplyWindows::~AutoReplyWindows()
{
    delete ui;
}
void AutoReplyWindows::settingTableChanged(int row, int col)
{
    //ui->tableWidget->item(2,3)->setText("hello");
    setWindowTitle("触发成功");
}
void AutoReplyWindows::test(void)
{
    setWindowTitle("触发失败");
}
