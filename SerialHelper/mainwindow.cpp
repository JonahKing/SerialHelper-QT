#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialconfig.h"
#include <QTextCodec>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QtDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial_config = new SerialConfig;
    port=new QSerialPort;
    p_auto_reply_windows = new AutoReplyWindows(this);
    p_temperature_windows = new TemperatureWindows(this);
    frame_duration_timer = new QTimer(this);


    connect(port,SIGNAL(readyRead()),this,SLOT(readread()));
    connect(frame_duration_timer,SIGNAL(timeout()),this,SLOT(readread()));
    connect(ui->Open,SIGNAL(triggered()),this,SLOT(openFileSlot()));
    connect(ui->Save,SIGNAL(triggered()),this,SLOT(saveFileSlot()));
    connect(ui->AutoReply,SIGNAL(triggered()),p_auto_reply_windows,SLOT(show()));
    connect(ui->TemparetureGraph,SIGNAL(triggered()),p_temperature_windows,SLOT(show()));



    QList<QSerialPortInfo> strlist=QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::const_iterator iter;
    for(iter=strlist.constBegin();iter!=strlist.constEnd();++iter)
    {
        ui->SerialNum->addItem((*iter).portName());
    }
    QList<QByteArray>list  =  QTextCodec::availableCodecs();

    ui->SerialOnoffIcon->setStyleSheet("border-image: url(:/new/img/red.png);");
    ui->FrameDuration->setValidator(new QIntValidator(10,5000,this));

    serial_config->receive_frame_duration =  ui->FrameDuration->text().toInt();

    QRegExp rx("[a-fA-F0-9 ]{160}");
    QRegExpValidator *validator = new QRegExpValidator(rx, this);
    ui->SendDataEditLIne1->setValidator(validator);
    ui->SendDataEditLIne2->setValidator(validator);
    ui->SendDataEditLIne3->setValidator(validator);
    ui->ZigbeeMAC->setValidator(validator);

    ui->FrameFilterEdit1->setValidator(validator);
    ui->FrameFilterEdit2->setValidator(validator);
    ui->FrameFilterEdit3->setValidator(validator);

    connect(ui->SendDataEditLIne1,SIGNAL(textChanged(QString)),this,SLOT(ForceHexAlign(QString)));
    connect(ui->SendDataEditLIne2,SIGNAL(textChanged(QString)),this,SLOT(ForceHexAlign( QString )));
    connect(ui->SendDataEditLIne3,SIGNAL(textChanged(QString)),this,SLOT(ForceHexAlign( QString)));

    connect(ui->SendDataEditLIne1,SIGNAL(textChanged(QString)),this,SLOT(SaveUserSetting(QString)));
    connect(ui->SendDataEditLIne2,SIGNAL(textChanged(QString)),this,SLOT(SaveUserSetting( QString )));
    connect(ui->SendDataEditLIne3,SIGNAL(textChanged(QString)),this,SLOT(SaveUserSetting( QString)));



    connect(ui->FrameFilterEdit1,SIGNAL(textChanged(QString)),this,SLOT(ForceHexAlign(QString)));
    connect(ui->FrameFilterEdit2,SIGNAL(textChanged(QString)),this,SLOT(ForceHexAlign( QString )));
    connect(ui->FrameFilterEdit3,SIGNAL(textChanged(QString)),this,SLOT(ForceHexAlign( QString)));
    connect(ui->ZigbeeMAC,SIGNAL(textChanged(QString)),this,SLOT(ForceHexAlign( QString)));

    connect(ui->FrameFilterEdit1,SIGNAL(textChanged(QString)),this,SLOT(SaveUserSetting(QString)));
    connect(ui->FrameFilterEdit2,SIGNAL(textChanged(QString)),this,SLOT(SaveUserSetting( QString)));
    connect(ui->FrameFilterEdit3,SIGNAL(textChanged(QString)),this,SLOT(SaveUserSetting( QString)));


    connect(ui->SendDataButton1,SIGNAL(clicked(bool)),this,SLOT(on_SendDataButton_clicked()));
    connect(ui->SendDataButton2,SIGNAL(clicked(bool)),this,SLOT(on_SendDataButton_clicked()));
    connect(ui->SendDataButton3,SIGNAL(clicked(bool)),this,SLOT(on_SendDataButton_clicked()));


    connect(ui->SendDataTimming1,SIGNAL(stateChanged(int)),this,SLOT(on_SendDataTimming_stateChanged(int)));
    connect(ui->SendDataTimming2,SIGNAL(stateChanged(int)),this,SLOT(on_SendDataTimming_stateChanged(int)));
    connect(ui->SendDataTimming3,SIGNAL(stateChanged(int)),this,SLOT(on_SendDataTimming_stateChanged(int)));

    connect(this,SIGNAL(BuffReceivefinished(QString)),this->p_auto_reply_windows,SLOT(ReceiveDataOk(QString)));
    connect(this,SIGNAL(BuffReceivefinished(QString)),this->p_temperature_windows,SLOT(ReceiveDataOk(QString)));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSend()));


    timer_joinnet = new QTimer(this);
    connect(timer_joinnet, SIGNAL(timeout()), this, SLOT(V4AutoJoinNet()));

    ParameterInit();


}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ParameterInit()
{
    //this->setWindowTitle("初始化成功");
    QSettings settings("ICConfig.ini", QSettings::IniFormat);
    ui->SendDataEditLIne1->setText(settings.value("SendDataEditLIne1").toString());
    ui->SendDataEditLIne2->setText(settings.value("SendDataEditLIne2").toString());
    ui->SendDataEditLIne3->setText(settings.value("SendDataEditLIne3").toString());
    ui->FrameDuration->setText(settings.value("FrameDuration").toString());

    ui->FrameFilterEdit1->setText(settings.value("FrameFilterEdit1").toString());
    ui->FrameFilterEdit2->setText(settings.value("FrameFilterEdit2").toString());
    ui->FrameFilterEdit3->setText(settings.value("FrameFilterEdit3").toString());

}
void MainWindow::ParameterSave(QString Type, QString p)
{
    QSettings settings("ICConfig.ini", QSettings::IniFormat);

    settings.setValue(Type,p);
}

void BuffReceivefinished(QString arg)
{
    ;
}


void MainWindow::readread()
{
    QByteArray arr= port->readAll();
    QDateTime current_data = QDateTime::currentDateTime();
    static QTime  start_time = QTime ::currentTime();
    QTime  current_time =QTime ::currentTime();

    static QString strBuffer = NULL;

    static QString receive_buff = NULL;

    QString contains_string1 = ui->FrameFilterEdit1->text();
    QString contains_string2 = ui->FrameFilterEdit2->text();
    QString contains_string3 = ui->FrameFilterEdit3->text();


    if((QSerialPort*)(sender()) == port)
    {
        frame_duration_timer->stop();
        frame_duration_timer->start(ui->FrameDuration->text().toInt());
        for(int i=0;i<arr.length();i++)
        {
            receive_buff += QString("%1").arg((uchar)arr.at(i),2,16,QLatin1Char('0')).toUpper()+" ";
        }
    }
    if((QTimer*)(sender()) == frame_duration_timer)
    {
        frame_duration_timer->stop();
        strBuffer = "\n" + current_data.toString("yyyy-MM-dd")+"    " + current_time.toString("hh:mm:ss:zzz")+"  :  ";
        strBuffer+= receive_buff;
        if(true != serial_config->stop_display)
        {

            if(0 == ui->ZigbeeVersionSelect->currentIndex())
            {
                if(-1 != receive_buff.indexOf("21 08 00 00 00 00 25 01"))
                {
                    this->serial_config->allow_joint_net = true;
                        QPalette pal = ui->AllowJoinNetButton->palette();
                        ui->AllowJoinNetButton->setPalette(pal);
                        ui->AllowJoinNetButton->setStyleSheet("background-color:green");
                        ui->AllowJoinNetButton->setText("允许入网状态");
                }

                if(-1 != receive_buff.indexOf("21 08 00 00 00 00 25 00"))
                {
                    this->serial_config->allow_joint_net = false;
                        QPalette pal = ui->AllowJoinNetButton->palette();
                        ui->AllowJoinNetButton->setPalette(pal);
                        ui->AllowJoinNetButton->setStyleSheet("background-color:red");
                        ui->AllowJoinNetButton->setText("关闭入网状态");
                }

            }
            else if(1 == ui->ZigbeeVersionSelect->currentIndex())
            {
                switch(v4_joinnet_state)
                {
                case 0x01:
                    if(-1 != receive_buff.indexOf("01 80 07 00 01 85 03 03"))
                    {
                        v4_joinnet_state = 0x02;
                    }
                    break;
                case 0x02:
                    if(-1 != receive_buff.indexOf("01 80 00 00 04 A4 00 00 00 20 03"))
                    {
                        v4_joinnet_state = 0x03;
                    }
                    break;
                case 0x03:
                    if(-1 != receive_buff.indexOf("01 80 00 00 04 A5 00 00 00 21 03"))
                    {
                        v4_joinnet_state = 0x04;
                    }
                    break;
                case 0x04:
                    if(-1 != receive_buff.indexOf("01 80 00 00 04 A0 00 00 00 24 03"))
                    {
                        v4_joinnet_state = 0x05;
                    }
                    break;
                case 0x05:
                    if(-1 != receive_buff.indexOf("01 80 00 00 04 E6 00 2B 00 49 03"))
                    {
                        v4_joinnet_state = 0x06;
                    }
                    break;
                case 0x06:
                    if(-1 != receive_buff.indexOf("01 80 14 00 01 94 01 03"))
                    {
                        v4_joinnet_state = 0x00;
                        this->serial_config->allow_joint_net = true;
                        QPalette pal = ui->AllowJoinNetButton->palette();
                        ui->AllowJoinNetButton->setPalette(pal);
                        ui->AllowJoinNetButton->setStyleSheet("background-color:green");
                        ui->AllowJoinNetButton->setText("允许入网状态");
                    }
                    if(-1 != receive_buff.indexOf("01 80 14 00 01 95 00 03"))
                    {
                        v4_joinnet_state = 0x00;
                        this->serial_config->allow_joint_net = false;
                        QPalette pal = ui->AllowJoinNetButton->palette();
                        ui->AllowJoinNetButton->setPalette(pal);
                        ui->AllowJoinNetButton->setStyleSheet("background-color:red");
                        ui->AllowJoinNetButton->setText("关闭入网状态");
                    }
                    break;

                }
            }



            unsigned int printf = 0;
            if(Qt::Checked == ui->FrameFilter1Enable->checkState())
            {
                if((-1 !=strBuffer.indexOf(contains_string1)))
                {
                         printf = 1;
                }
            }
            if(Qt::Checked == ui->FrameFilter2Enable->checkState())
            {
                if((-1 !=strBuffer.indexOf(contains_string2)))
                {
                   printf = 1;
                }
            }
            if(Qt::Checked == ui->FrameFilter3Enable->checkState())
            {
                 if((-1 !=strBuffer.indexOf(contains_string3)))
                {
                   printf = 1;
                }
            }
            if((Qt::Unchecked == ui->FrameFilter1Enable->checkState())\
             &&(Qt::Unchecked == ui->FrameFilter2Enable->checkState())\
             &&(Qt::Unchecked == ui->FrameFilter3Enable->checkState()))
            {
                printf = 1;
            }

            if(1 == printf)
            {
                ui->ReceiveDataBrowser->setTextColor(Qt::black);
                ui->ReceiveDataBrowser->insertPlainText(strBuffer);
                emit BuffReceivefinished(receive_buff);

                QString send_string = receive_buff;

                if(this->serial_config->allow_joint_net == true)
                {
                    if((-1 != receive_buff.indexOf("01 0E"))&&(-1 != receive_buff.indexOf("23 8B FF FF FF FF A8")))
                    {
                          if(1 == ui->HostVersionReply->currentIndex())//原样回复
                          {
                                  AutoSend(send_string);
                          }
                          if(2 == ui->HostVersionReply->currentIndex())
                          {
                               send_string.replace("23 8B FF FF FF FF A8","23 8B 03 02 04 00 AD");
                               AutoSend(send_string);
                          }
                     }
                    else if(-1 != receive_buff.indexOf("23 80"))
                    {
                          if(1 == ui->DevicesTypeReply->currentIndex())//原样回复
                          {
                                  AutoSend(send_string);
                          }
                     }
                    else if(-1 != receive_buff.indexOf("23 85"))
                    {
                          if(1 == ui->DevicesVersionReply->currentIndex())//原样回复
                          {
                                  AutoSend(send_string);
                          }
                     }
                }
            }    

        }
        strBuffer =  "";
        receive_buff = "";
    }

}
void MainWindow::on_SerialOnoffBUtton_clicked()
{
    if(serial_config->serial_open_status)
    {
        port->close();
        ui->SerialOnoffIcon->setStyleSheet("border-image: url(:/new/img/red.png);");
        serial_config->serial_open_status=false;
        ui->SerialOnoffBUtton->setText("打开");
    }
    else
    {
#ifdef Q_OS_WIN
        port->setPortName(ui->SerialNum->currentText());
#else
        port->setPortName("/dev/"+ui->SerialNum->currentText());
#endif
        port->setBaudRate(ui->Baud->currentText().toInt());
        port->setDataBits(getbit(ui->DataBitLength->value()));
        if(port->open(QIODevice::ReadWrite))
        {
            ui->SerialOnoffBUtton->setText("关闭");
            ui->SerialOnoffIcon->setStyleSheet("border-image: url(:/new/img/lv.png);");
            serial_config->serial_open_status=true;
        }
        else
        {
            serial_config->serial_open_status=false;
            ui->SerialOnoffBUtton->setText("打开失败！");
            ui->SerialOnoffIcon->setStyleSheet("border-image: url(:/new/img/red.png);");
        }
    }
}

void MainWindow::on_REceiveStopButton_clicked()
{
    serial_config->stop_display = !serial_config->stop_display;
    if(true == serial_config->stop_display)
    {
         ui->REceiveStopButton->setText("停止显示");
         QPalette pal = ui->REceiveStopButton->palette();
         ui->REceiveStopButton->setPalette(pal);
         ui->REceiveStopButton->setStyleSheet("background-color:red");
    }
    else
    {
        ui->REceiveStopButton->setText("正常显示");
        QPalette pal = ui->REceiveStopButton->palette();
        ui->REceiveStopButton->setPalette(pal);
        ui->REceiveStopButton->setStyleSheet("background-color:green");
    }
}

void MainWindow::on_FrameDuration_editingFinished()
{
    serial_config->receive_frame_duration =  ui->FrameDuration->text().toInt();
}


void MainWindow::on_SendDataButton_clicked()
{
    if(!port->isOpen())
    {
        return;
    }
    QString str;
    if(ui->SendDataButton1 == (QPushButton *)sender())
    {
         str=ui->SendDataEditLIne1->text();
    }
    if(ui->SendDataButton2 == (QPushButton *)sender())
    {
         str=ui->SendDataEditLIne2->text();
    }
    if(ui->SendDataButton3 == (QPushButton *)sender())
    {
         str=ui->SendDataEditLIne3->text();
    }
        QStringList strlist=str.trimmed().split(" ");
        QByteArray arr;
        for(int i=0;i<strlist.count();++i)
        {
            bool bl=false;
            QString ch=strlist.at(i);
            uchar byte=(uchar)ch.toInt(&bl,16);
            if(!bl)
            {
                return;
            }
            arr.append(byte);
        }
        port->write(arr);
        QDateTime current_data = QDateTime::currentDateTime();
        QTime  current_time =QTime ::currentTime();
        QString strBuffer = "\n" + current_data.toString("yyyy-MM-dd")+"    " \
                + current_time.toString("hh:mm:ss:zzz")+"  :  ";
        if(!serial_config->stop_display)
        {
            ui->ReceiveDataBrowser->setTextColor(Qt::red);
            ui->ReceiveDataBrowser->insertPlainText(strBuffer+str);
        }
}


void MainWindow::AutoSend(QString str)
{
    if(!port->isOpen())
    {
        return;
    }
        QStringList strlist=str.trimmed().split(" ");
        QByteArray arr;
        for(int i=0;i<strlist.count();++i)
        {
            bool bl=false;
            QString ch=strlist.at(i);
            uchar byte=(uchar)ch.toInt(&bl,16);
            if(!bl)
            {
                return;
            }
            arr.append(byte);
        }
        port->write(arr);
        ui->ReceiveDataBrowser->moveCursor(QTextCursor::End);
        QDateTime current_data = QDateTime::currentDateTime();
        QTime  current_time =QTime ::currentTime();
        QString strBuffer = "\n" + current_data.toString("yyyy-MM-dd")+"    " \
                + current_time.toString("hh:mm:ss:zzz")+"  :  ";
        if(!serial_config->stop_display)
        {
            if(Qt::Checked != ui->SendDataDisplay->checkState())
            {
                ui->ReceiveDataBrowser->setTextColor(Qt::red);
                ui->ReceiveDataBrowser->insertPlainText(strBuffer+str);
            }
        }
}
void MainWindow::on_ReceiveDataBrowser_textChanged()
{
    ui->ReceiveDataBrowser->moveCursor(QTextCursor::End);

}
void MainWindow::TimerSend(void)
{
    int temp = 1000;
    if(1 == serial_config->current_timming_index)
    {
        AutoSend(ui->SendDataEditLIne1->text());
        if(serial_config->send_data_timming2)
        {
            temp = ui->SendDurationEditLine2->text().toInt();
            timer->start(temp);
            serial_config->current_timming_index = 2;
        }
        else if(serial_config->send_data_timming3)
        {
            temp = ui->SendDurationEditLine3->text().toInt();
            timer->start(temp);
            serial_config->current_timming_index = 3;
        }
        else if(serial_config->send_data_timming1)
        {
            temp = ui->SendDurationEditLine1->text().toInt();
            timer->start(temp);

        }
        else
        {
            timer->stop();
        }

    }
    else if(2 == serial_config->current_timming_index)
    {
        AutoSend(ui->SendDataEditLIne2->text());
        if(serial_config->send_data_timming3)
        {
            temp = ui->SendDurationEditLine3->text().toInt();
            timer->start(temp);
            serial_config->current_timming_index = 3;
        }
        else if(serial_config->send_data_timming1)
        {
            temp = ui->SendDurationEditLine1->text().toInt();
            timer->start(temp);
            serial_config->current_timming_index = 1;
        }
        else if(serial_config->send_data_timming2)
        {
            temp = ui->SendDurationEditLine2->text().toInt();
            timer->start(temp);
        }
        else
        {
            timer->stop();
        }

    }
    else if(3 == serial_config->current_timming_index)
    {
        AutoSend(ui->SendDataEditLIne3->text());
        if(serial_config->send_data_timming1)
        {
            temp = ui->SendDurationEditLine1->text().toInt();
            timer->start(temp);
            serial_config->current_timming_index = 1;
        }
        else if(serial_config->send_data_timming2)
        {
            temp = ui->SendDurationEditLine2->text().toInt();
            timer->start(temp);
            serial_config->current_timming_index = 2;
        }
        else if(serial_config->send_data_timming3)
        {
            temp = ui->SendDurationEditLine3->text().toInt();
            timer->start(temp);
        }
        else
        {
            timer->stop();
        }

    }

}

void MainWindow::on_SendDataTimming_stateChanged(int arg1)
{
    if( ui->SendDataTimming1 == sender())
    {
        if(Qt::Checked == arg1)
        {
            serial_config->current_timming_index = 1;
            serial_config->send_data_timming1 = true;
            int temp = ui->SendDurationEditLine1->text().toInt();
            timer->start(temp);
        }
        else
        {
            serial_config->send_data_timming1 = false;
        }
    }
    if( ui->SendDataTimming2 == sender())
    {
        if(Qt::Checked == arg1)
        {
            serial_config->current_timming_index = 2;
            serial_config->send_data_timming2 = true;
            int temp = ui->SendDurationEditLine2->text().toInt();
            timer->start(temp);
        }
        else
        {
            serial_config->send_data_timming2 = false;
        }
    }
    if( ui->SendDataTimming3 == sender())
    {
        if(Qt::Checked == arg1)
        {
            serial_config->current_timming_index = 3;
            serial_config->send_data_timming3 = true;
            int temp = ui->SendDurationEditLine3->text().toInt();
            timer->start(temp);
        }
        else
        {
            serial_config->send_data_timming3 = false;
        }
    }
}

void MainWindow::openFileSlot()
{
        fileName = QFileDialog::getOpenFileName(this, tr("打开文件"),QDir::homePath(),tr("文本文件 (*.*);;"));
        this->readFile();
}

void MainWindow::readFile(){

    //得到路径不为空
    if(!fileName.isEmpty()){
        QFile *file = new QFile;
        file->setFileName(fileName);

        bool isOpen = file->open(QIODevice::ReadOnly);
        if(isOpen){
            ui->ReceiveDataBrowser->clear();
            QTextStream in(file);

            while (!in.atEnd()) {
                ui->ReceiveDataBrowser->append(in.readLine());
                //光标移动到开始位置
                ui->ReceiveDataBrowser->moveCursor(QTextCursor::Start);
            }
            //已读完
            fileContent = ui->ReceiveDataBrowser->document()->toPlainText();

            if(fileName.lastIndexOf("\\") != -1){
                //设置标题
                this->setWindowTitle(fileName.mid(fileName.lastIndexOf('\\')+1)+" - 记事本");
            }else{
                //设置标题
                this->setWindowTitle(fileName.mid(fileName.lastIndexOf('/')+1)+" - 记事本");
            }

            file->close();
            //ui->statusBar->showMessage("");
        }else{
            QMessageBox box(QMessageBox::Question,"提示","打开文件失败！");
            box.setIcon(QMessageBox::Warning);
            box.setStandardButtons (QMessageBox::Ok);
            box.setButtonText (QMessageBox::Ok,QString("确定"));
            box.exec();
        }
    }
}
void MainWindow::saveFileSlot(){
    //判断是新建还是读取的文本

    if(fileName.isEmpty()){//新建
        //弹出保存文件对话框
        fileName = QFileDialog::getSaveFileName(this, tr("打开文件"),QDir::homePath(),tr("文本文件 (*.*);;"));
        if(!fileName.isEmpty()){
            //保存文件
            this->saveTextToFile();

        }
    }else{//读取的文本
        this->saveTextToFile();
    }
}
//保存文件
void MainWindow::saveTextToFile(){
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        out << ui->ReceiveDataBrowser->toPlainText();
        file.close();
        this->setWindowTitle(fileName.mid(fileName.lastIndexOf('/')+1)+" - 记事本");
        fileContent = ui->ReceiveDataBrowser->document()->toPlainText();
        //ui->statusBar->showMessage("已保存",3000);
    }else{
        QMessageBox box(QMessageBox::Question,"提示","保存文件失败！");
        box.setIcon(QMessageBox::Warning);
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确定"));
        box.exec();
    }

}


void MainWindow::SaveUserSetting(const QString &arg1)
{

    if(ui->SendDataEditLIne1 == (QLineEdit*)sender())
    {
        ParameterSave("SendDataEditLIne1",arg1);
    }
    if(ui->SendDataEditLIne2 == (QLineEdit*)sender())
    {
        ParameterSave("SendDataEditLIne2",arg1);
    }
    if(ui->SendDataEditLIne3 == (QLineEdit*)sender())
    {
        ParameterSave("SendDataEditLIne3",arg1);
    }

    if(ui->FrameFilterEdit1 == (QLineEdit*)sender())
    {
        ParameterSave("FrameFilterEdit1",arg1);
    }
    if(ui->FrameFilterEdit2 == (QLineEdit*)sender())
    {
        ParameterSave("FrameFilterEdit2",arg1);
    }
    if(ui->FrameFilterEdit3 == (QLineEdit*)sender())
    {
        ParameterSave("FrameFilterEdit3",arg1);
    }

}



void MainWindow::ForceHexAlign(const QString &arg1)
{
    QString str_source=arg1;//ui->SendDataEditLIne1->text();
    QString str_des;
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
    //ui->SendDataEditLIne1->setText(str_des);
    ((QLineEdit*)sender())->setText(str_des);

}

void MainWindow::on_FrameDuration_textChanged(const QString &arg1)
{
    ParameterSave("FrameDuration",arg1);
}

void MainWindow::on_AllowJoinNetButton_clicked()
{
    if(0 == ui->ZigbeeVersionSelect->currentIndex())//V3
    {
        if(false == this->serial_config->allow_joint_net)
        {
            AutoSend("22 09 00 00 00 00 00 25 01");
        }
        else
        {
             AutoSend("22 09 00 00 00 00 00 25 00");
        }
    }
    else if(1 == ui->ZigbeeVersionSelect->currentIndex())//V3
    {
        if(false == this->serial_config->allow_joint_net)
        {
            v4_joinnet_state = 1;
            timer_joinnet->start(100);

        }
    }


}
void MainWindow::on_ResetZigbeebutton_clicked()
{
    QString send_str = "01 0C "+ ui->ZigbeeMAC->text()+"AA 06 89 00 01 8E";
    AutoSend(send_str);
}

void MainWindow::V4AutoJoinNet()
{
    static int current_index = 0;

    if(current_index == v4_joinnet_state)
    {
        current_index = 0;
        v4_joinnet_state = 0;
        timer_joinnet->stop();
        return;
    }
    if(1 == v4_joinnet_state)
    {
        AutoSend("01 02 10 12 02 10 02 10 12 03");
        timer->start(100);
    }
    else if(2 == v4_joinnet_state)
    {
        AutoSend("01 02 10 20 02 10 02 18 28 02 10 02 10 02 10 02 10 02 10 02 10 02 10 02 10 03");
        timer->start(100);
    }
    else if(3 == v4_joinnet_state)
    {
        AutoSend("01 02 10 21 02 10 02 14 2D 02 10 02 10 02 18 02 10 03");
        timer->start(100);
    }
    else if(4 == v4_joinnet_state)
    {
        AutoSend("01 02 10 24 02 10 02 10 24 03");
        timer->start(100);
    }
    else if(5 == v4_joinnet_state)
    {
        AutoSend("01 02 10 49 02 10 02 14 72 FF FC 3C 02 10 03");
    }
    else if(6 == v4_joinnet_state)
    {
        AutoSend("01 80 00 00 04 90 00 00 00 14 03");
    }

    current_index = v4_joinnet_state;
}



