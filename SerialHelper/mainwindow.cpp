#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialconfig.h"
#include <QTextCodec>
#include <QDebug>
#include <QTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial_config = new SerialConfig;
    port=new QSerialPort;

    connect(port,SIGNAL(readyRead()),this,SLOT(readread()));


    QList<QSerialPortInfo> strlist=QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::const_iterator iter;
    for(iter=strlist.constBegin();iter!=strlist.constEnd();++iter)
    {
        ui->SerialNum->addItem((*iter).portName());
    }
    QList<QByteArray>list  =  QTextCodec::availableCodecs();


    ui->SerialOnoffIcon->setStyleSheet("border-image: url(:/new/img/red.png);");
    ui->FrameDuration->setValidator(new QIntValidator(50,5000,this));

    serial_config->receive_frame_duration =  ui->FrameDuration->text().toInt();

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
        ui->ReceiveDataBrowser->setTextColor(Qt::blue);
        ui->ReceiveDataBrowser->insertPlainText(strBuffer);

        ui->ReceiveDataBrowser->setTextColor(Qt::blue);
        ui->ReceiveDataBrowser->insertPlainText(str);
    }

}
