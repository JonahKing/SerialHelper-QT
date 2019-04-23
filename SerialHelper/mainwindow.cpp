#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QDebug>
#include <QTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial_open_status = false;
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
    if(start_time.msecsTo(current_time) > 100)
    {
        start_time = current_time;
        QString strBuffer = "\n" + current_data.toString("yyyy-MM-dd")+"    " \
                + current_time.toString("hh:mm:ss:zzz")+"  :  ";
        ui->ReceiveDataBrowser->setTextColor(0xFF0000);
        ui->ReceiveDataBrowser->insertPlainText(strBuffer);
    }

    QString str;
    for(int i=0;i<arr.length();i++)
    {
        str+= QString("%1").arg((uchar)arr.at(i),2,16,QLatin1Char('0')).toUpper()+" ";
    }
    ui->ReceiveDataBrowser->setTextColor(Qt::black);
    ui->ReceiveDataBrowser->insertPlainText(str);

}
void MainWindow::on_SerialOnoffBUtton_clicked()
{
    if(serial_open_status)
    {
        port->close();
        ui->SerialOnoffIcon->setStyleSheet("border-image: url(:/new/img/red.png);");
        serial_open_status=false;
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
            serial_open_status=true;
        }
        else
        {
            serial_open_status=false;
            ui->SerialOnoffBUtton->setText("打开失败！");
            ui->SerialOnoffIcon->setStyleSheet("border-image: url(:/new/img/red.png);");
        }
    }
}
