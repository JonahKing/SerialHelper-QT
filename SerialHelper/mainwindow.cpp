#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialconfig.h"
#include <QTextCodec>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial_config = new SerialConfig;
    port=new QSerialPort;
p_auto_reply_windows = new AutoReplyWindows(this);

    connect(port,SIGNAL(readyRead()),this,SLOT(readread()));
    connect(ui->Open,SIGNAL(triggered()),this,SLOT(openFileSlot()));
    connect(ui->Save,SIGNAL(triggered()),this,SLOT(saveFileSlot()));
    connect(ui->AutoReply,SIGNAL(triggered()),p_auto_reply_windows,SLOT(show()));





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


    connect(ui->SendDataEditLIne1,SIGNAL(textChanged(QString)),this,SLOT(SendDataEditLIne_textChanged(QString)));
    connect(ui->SendDataEditLIne2,SIGNAL(textChanged(QString)),this,SLOT(SendDataEditLIne_textChanged( QString )));
    connect(ui->SendDataEditLIne3,SIGNAL(textChanged(QString)),this,SLOT(SendDataEditLIne_textChanged( QString)));


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSend()));
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::readread()
{
    QByteArray arr= port->readAll();
    QDateTime current_data = QDateTime::currentDateTime();
    static QTime  start_time = QTime ::currentTime();
    QTime  current_time =QTime ::currentTime();

    if(start_time.msecsTo(current_time) > serial_config->receive_frame_duration)
    {
        start_time = current_time;
        QString strBuffer = "\n" + current_data.toString("yyyy-MM-dd")+"    " \
                + current_time.toString("hh:mm:ss:zzz")+"  :  ";
        ui->ReceiveDataBrowser->setTextColor(Qt::red);

        if(true != serial_config->stop_display)
        {
            ui->ReceiveDataBrowser->insertPlainText(strBuffer);
        }
    }
    start_time = current_time;
    QString str;
    for(int i=0;i<arr.length();i++)
    {
        str+= QString("%1").arg((uchar)arr.at(i),2,16,QLatin1Char('0')).toUpper()+" ";
    }
    if(true != serial_config->stop_display)
    {
        ui->ReceiveDataBrowser->setTextColor(Qt::red);
        ui->ReceiveDataBrowser->insertPlainText(str);
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
        port->setPortName(ui->comboBoxserial->currentText());
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
         ui->REceiveStopButton->setText("继续显示");
    }
    else
    {
        ui->REceiveStopButton->setText("暂停显示");
    }
}

void MainWindow::on_FrameDuration_editingFinished()
{
    serial_config->receive_frame_duration =  ui->FrameDuration->text().toInt();
}

void MainWindow::on_SendDataButton1_clicked()
{
    if(!port->isOpen())
    {
        return;
    }
    {
        QString str=ui->SendDataEditLIne1->text();
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
            ui->ReceiveDataBrowser->setTextColor(Qt::blue);
            ui->ReceiveDataBrowser->insertPlainText(strBuffer);

            ui->ReceiveDataBrowser->setTextColor(Qt::blue);
            ui->ReceiveDataBrowser->insertPlainText(str);
        }
    }

}


void MainWindow::on_SendDataButton2_clicked()
{
    if(!port->isOpen())
    {
        return;
    }
    {
        QString str=ui->SendDataEditLIne2->text();
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
            ui->ReceiveDataBrowser->setTextColor(Qt::blue);
            ui->ReceiveDataBrowser->insertPlainText(strBuffer);

            ui->ReceiveDataBrowser->setTextColor(Qt::blue);
            ui->ReceiveDataBrowser->insertPlainText(str);
        }
    }

}

void MainWindow::on_SendDataButton3_clicked()
{
    if(!port->isOpen())
    {
        return;
    }
    {
        QString str=ui->SendDataEditLIne3->text();
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
            ui->ReceiveDataBrowser->setTextColor(Qt::blue);
            ui->ReceiveDataBrowser->insertPlainText(strBuffer);

            ui->ReceiveDataBrowser->setTextColor(Qt::blue);
            ui->ReceiveDataBrowser->insertPlainText(str);
        }


    }
}

void MainWindow::on_ReceiveDataBrowser_textChanged()
{
    ui->ReceiveDataBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::on_SendDataTimming1_stateChanged(int arg1)
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

void MainWindow::TimerSend(void)
{
    int temp = 1000;
    if(1 == serial_config->current_timming_index)
    {
        on_SendDataButton1_clicked();
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
        on_SendDataButton2_clicked();
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
        on_SendDataButton3_clicked();
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


void MainWindow::on_SendDataTimming2_stateChanged(int arg1)
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

void MainWindow::on_SendDataTimming3_stateChanged(int arg1)
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

void MainWindow::SendDataEditLIne_textChanged(const QString &arg1)
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
