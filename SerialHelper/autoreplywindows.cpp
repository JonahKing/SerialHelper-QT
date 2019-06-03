#include "autoreplywindows.h"
#include "ui_autoreplywindows.h"
#include <QSettings>
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
        ui->tableWidget->setCellWidget(i,0,check_box); //插入复选

    }

    for(int i = 0;i<ui->MunualSendTabWidget->rowCount ();i++)
    {
        QPushButton *button=new QPushButton;
        button->setProperty("ID",i);
        button->setText("发送");
        ui->MunualSendTabWidget->setCellWidget(i,3,button); //插入复选
        connect(button,SIGNAL(clicked(bool)),this,SLOT(send_button_fun(bool)));
    }

    ParameterInit();

    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(ForceHexAlign(int,int)));
    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(SaveUserSetting(int,int)));
    connect(ui->MunualSendTabWidget,SIGNAL(cellChanged(int,int)),this,SLOT(ForceHexAlign(int,int)));
    connect(ui->MunualSendTabWidget,SIGNAL(cellChanged(int,int)),this,SLOT(SaveUserSetting(int,int)));


    connect(this,SIGNAL(AutoReplyToWindows(QString)),parent,SLOT(AutoSend(QString)));
    for(int i = 0;i<ui->tableWidget->rowCount ();i++)
    {
        connect((QCheckBox*)ui->tableWidget->cellWidget(i,0),SIGNAL(stateChanged(int)),this,SLOT(SaveUserSetting(int)));
    }
}

AutoReplyWindows::~AutoReplyWindows()
{
    delete ui;
}

void AutoReplyWindows::ParameterInit(void)
{
    QSettings settings("ICConfig.ini", QSettings::IniFormat);

    for(int i = 0; i< ui->tableWidget->rowCount();i++)
    {

        QString state = settings.value(QString::number(i*100+0)).toString();
        if(-1 != state.indexOf("Qt::Checked"))
        {
             ((QCheckBox*)ui->tableWidget->cellWidget(i,0))->setCheckState(Qt::Checked);
        }

        for(int j = 1;j<ui->tableWidget->columnCount();j++)
        {
            //ui->tableWidget->item(i,j)->setText(settings.value(QString::number(i*100+j)).toString());
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(settings.value(QString::number(i*100+j)).toString()));
        }

    }
    for(int i = 0; i< ui->MunualSendTabWidget->rowCount();i++)
    {

        for(int j = 0;j<ui->MunualSendTabWidget->columnCount();j++)
        {
            ui->MunualSendTabWidget->setItem(i,j,new QTableWidgetItem(settings.value(QString::number(10000+i*100+j)).toString()));
        }

    }
}

void AutoReplyWindows::SaveUserSetting(int row ,int column)
{
         QString Type, p;
         if(sender() == ui->tableWidget)
         {
             Type  = QString::number(row*100+column);
             p = ui->tableWidget->item(row,column)->text();

             QSettings settings("ICConfig.ini", QSettings::IniFormat);
             settings.setValue(Type,p);
         }
         if(sender() == ui->MunualSendTabWidget)
         {
             Type  = QString::number(10000+row*100+column);
             p = ui->MunualSendTabWidget->item(row,column)->text();

             QSettings settings("ICConfig.ini", QSettings::IniFormat);
             settings.setValue(Type,p);
         }

}
void AutoReplyWindows::SaveUserSetting(int arg)
{
    QString Type, p;
    for(int row= 0;row < ui->tableWidget->rowCount();row++)
    {
        Type  = QString::number(row*100+0);
        if(Qt::Checked == ((QCheckBox*)ui->tableWidget->cellWidget(row,0))->checkState())
         {
             p = "Qt::Checked";
         }
         else
         {
             p = "Qt::Unchecked";
         }
        QSettings settings("ICConfig.ini", QSettings::IniFormat);
        settings.setValue(Type,p);
    }
}

void AutoReplyWindows::ReceiveDataOk(QString arg)
{

    QString contain_str;
     QString send_str;
    for(int i= 0; i < ui->tableWidget->rowCount();i++)
    {
        if(Qt::Checked == ((QCheckBox*)ui->tableWidget->cellWidget(i,0))->checkState())
        {
            contain_str = ui->tableWidget->item(i,1)->text();
            if(-1 != arg.indexOf(contain_str))
            {
                send_str = ui->tableWidget->item(i,2)->text();
                if(0 == send_str.length())
                {
                    send_str = arg;
                }
                emit AutoReplyToWindows(send_str);
            }

        }
    }
}

void AutoReplyWindows::ForceHexAlign(int row,int column)
{
    QString str_source = NULL;
    QString str_des;
    if(sender() == ui->tableWidget)
    {
       str_source =ui->tableWidget->item(row,column)->text();//ui->SendDataEditLIne1->text();
    }
    if(sender() == ui->MunualSendTabWidget)
    {
        str_source =ui->MunualSendTabWidget->item(row,column)->text();//ui->SendDataEditLIne1->text();
    }
    int count = 0;
    str_source = str_source.remove(QRegExp("\\s"));

    for(int i = 0; i <str_source.length();i++ )
    {
        str_des += str_source.at(i).toUpper();
        //if(operator!=(QChar::Space,str_source.at(i)))
        {
            count++;
        }
        if(0 == (i+1)%2)
        {
               str_des += " ";
        }
    }
    if(sender() == ui->tableWidget)
    {
       ui->tableWidget->item(row,column)->setText(str_des);
    }
    if(sender() == ui->MunualSendTabWidget)
    {
        ui->MunualSendTabWidget->item(row,column)->setText(str_des);
    }

}

void AutoReplyWindows::send_button_fun(bool)
{
    QPushButton *button = (QPushButton*)sender();
    int row = button->property("id").toInt();
    QString send_str = ui->MunualSendTabWidget->item(row,0)->text()+" "\
            +ui->MunualSendTabWidget->item(row,1)->text()+" "\
            +ui->MunualSendTabWidget->item(row,2)->text();
    emit AutoReplyToWindows(send_str);
}
void AutoReplyToWindows(QString)
{
    ;
}
