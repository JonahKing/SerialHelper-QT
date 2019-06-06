#include "autoreplywindows.h"
#include "ui_autoreplywindows.h"
#include <QSettings>
AutoReplyWindows::AutoReplyWindows(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AutoReplyWindows)
{
    ui->setupUi(this);


    QLineEdit *LineEdit=new QLineEdit();
    QRegExp rx("[a-fA-F0-9 ]{160}");
    QRegExpValidator *validator = new QRegExpValidator(rx, this);


    setWindowTitle("自动回复设置");
    for(int i = 0;i<ui->tableWidget->rowCount ();i++)
    {
        QCheckBox *check_box=new QCheckBox;
        check_box->setCheckState (Qt::Unchecked);
        ui->tableWidget->setCellWidget(i,0,check_box); //插入复选
        connect(check_box,SIGNAL(stateChanged(int)),this,SLOT(SaveUserSetting(int)));

    }

    for(int i = 0;i<ui->tableWidget->rowCount ();i++)
    {
        for(int j = 1;j<ui->tableWidget->columnCount()-1;j++)
        {
            QLineEdit *LineEdit=new QLineEdit();
            //QTableWidgetItem* table_widget = new QTableWidgetItem();
            LineEdit->setValidator(validator);
            LineEdit->setProperty("row",i);
            LineEdit->setProperty("column",j);

            LineEdit->setValidator(validator);
            ui->tableWidget->setCellWidget(i,j,LineEdit);
            connect(LineEdit,SIGNAL(textChanged(QString)),this,SLOT(SaveUserSetting(QString)));
        }

    }


    for(int i = 0;i<ui->MunualSendTabWidget->rowCount ();i++)
    {
         for(int j = 0;j<ui->MunualSendTabWidget->columnCount();j++)
         {
             if(j != 3)
             {
                 QLineEdit *LineEdit=new QLineEdit();
                 //QTableWidgetItem* table_widget = new QTableWidgetItem();
                 if(j != 4)
                 {
                     LineEdit->setValidator(validator);

                 }
                 LineEdit->setProperty("row",100+i);
                 LineEdit->setProperty("column",j);
                 ui->MunualSendTabWidget->setCellWidget(i,j,LineEdit);
                 connect(LineEdit,SIGNAL(textChanged(QString)),this,SLOT(SaveUserSetting(QString)));
             }

         }
    }

    for(int i = 0;i<ui->MunualSendTabWidget->rowCount();i++)
    {
        QPushButton *button=new QPushButton;
        button->setProperty("ID",i);
        button->setText("发送");
        button->setProperty("row",100+i);
        button->setProperty("column",3);
        ui->MunualSendTabWidget->setCellWidget(i,3,button);
        connect(button,SIGNAL(clicked(bool)),this,SLOT(send_button_fun(bool)));
    }


     ParameterInit();


    connect(this,SIGNAL(AutoReplyToWindows(QString)),parent,SLOT(AutoSend(QString)));

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

        for(int j = 1;j<ui->tableWidget->columnCount()-1;j++)
        {
            ((QLineEdit*)ui->tableWidget->cellWidget(i,j))->setText(settings.value(QString::number(i*100+j)).toString());
        }

    }
    for(int i = 0; i< ui->MunualSendTabWidget->rowCount();i++)
    {
        for(int j = 0;j<ui->MunualSendTabWidget->columnCount();j++)
        {
            if(j != (ui->MunualSendTabWidget->columnCount()-2))
            {
                ((QLineEdit*)ui->MunualSendTabWidget->cellWidget(i,j))->setText(settings.value(QString::number(10000+i*100+j)).toString());
             }
           }
    }
}
void AutoReplyWindows::SaveUserSetting(QString arg)
{
    QLineEdit *widget_item = (QLineEdit*)sender();

    int row = widget_item->property("row").toInt();
    int column = widget_item->property("column").toInt();


    QString str_des = NULL;
    if((row < 100)||(column != 4))
    {
        int count = 0;
        arg = arg.remove(QRegExp("\\s"));

        for(int i = 0; i <arg.length();i++ )
        {
            str_des += arg.at(i).toUpper();
            count++;
            if(0 == (i+1)%2)
            {
                   str_des += " ";
            }
        }
        widget_item->setText(str_des);

    }

    QString Type  = QString::number(row*100+column);
    QString p = arg;

    QSettings settings("ICConfig.ini", QSettings::IniFormat);
    settings.setValue(Type,p);
}
void AutoReplyWindows::SaveUserSetting(int arg)
{

    QTableWidgetItem *widget_item = (QTableWidgetItem *)sender();
    int row = widget_item->row();
    int column = widget_item->column();
    QString Type, p;
    Type  = QString::number(row*100+column);
    if(Qt::Checked == arg)
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

void AutoReplyWindows::send_button_fun(bool)
{
    QPushButton *button = (QPushButton*)sender();
    int row = button->property("ID").toInt();
    QString send_str = ((QLineEdit*)ui->MunualSendTabWidget->cellWidget(row,0))->text()
            +((QLineEdit*)ui->MunualSendTabWidget->cellWidget(row,1))->text()
            +((QLineEdit*)ui->MunualSendTabWidget->cellWidget(row,2))->text();
    emit AutoReplyToWindows(send_str);
}
void AutoReplyToWindows(QString)
{
    ;
}
