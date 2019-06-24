#include "temperaturewindows.h"
#include "ui_temperaturewindows.h"



TemperatureWindows::TemperatureWindows(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TemperatureWindows)
{
    ui->setupUi(this);
    this->setWindowTitle("温度曲线");
    //ui->widget->setBackground(QBrush(Qt::black));
    ui->widget->xAxis->setLabel("时间轴(sec)");
    ui->widget->xAxis->setRange(0,300);
    ui->widget->yAxis->setLabel("温度(°C)");
    ui->widget->yAxis->setRange(0,50);


    ui->widget->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);


    for(int i = 0;i<10;i++)
    {
        this->temperature[i].ZIgbee_ID = i;
    }

    this->temperature[0].pen_color = Qt::blue;
    this->temperature[1].pen_color = Qt::red;
    this->temperature[2].pen_color = Qt::green;
    this->temperature[3].pen_color = Qt::black;
    this->temperature[4].pen_color = Qt::darkGray;
    this->temperature[5].pen_color = Qt::cyan;
    this->temperature[6].pen_color = Qt::magenta;

    this->temperature[7].pen_color = Qt::darkRed;
    this->temperature[8].pen_color = Qt::darkGreen;
    this->temperature[9].pen_color = Qt::darkBlue;
   // darkBlue,
    for(int i = 0; i<10;i++)
    {
        ui->widget->addGraph();
        ui->widget->graph(i)->setPen(QPen(this->temperature[i].pen_color));
    }


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateTemperatureGraph()));
    timer->start(1000);





    ui->widget_2->xAxis->setLabel("时间轴(sec)");
    ui->widget_2->xAxis->setRange(0,300);
    ui->widget_2->yAxis->setLabel("温度(°C)");
    ui->widget_2->yAxis->setRange(0,50);
    ui->widget_2->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);




    ui->widget_3->xAxis->setLabel("时间轴(sec)");
    ui->widget_3->xAxis->setRange(0,300);
    ui->widget_3->yAxis->setLabel("湿度(%RH)");
    ui->widget_3->yAxis->setRange(0,50);
    ui->widget_3->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);


    ui->widget_4->xAxis->setLabel("时间轴(sec)");
    ui->widget_4->xAxis->setRange(0,300);
    ui->widget_4->yAxis->setLabel("光照（lux）");
    ui->widget_4->yAxis->setRange(0,50);
    ui->widget_4->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);



    ui->widget_5->xAxis->setLabel("时间轴(sec)");
    ui->widget_5->xAxis->setRange(0,300);
    ui->widget_5->yAxis->setLabel("粉尘(ug/m3)");
    ui->widget_5->yAxis->setRange(0,50);
    ui->widget_5->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);


    ui->widget_6->xAxis->setLabel("时间轴(sec)");
    ui->widget_6->xAxis->setRange(0,300);
    ui->widget_6->yAxis->setLabel("CO2(ppm)");
    ui->widget_6->yAxis->setRange(0,50);
    ui->widget_6->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);



    ui->widget_7->xAxis->setLabel("时间轴(sec)");
    ui->widget_7->xAxis->setRange(0,300);
    ui->widget_7->yAxis->setLabel("甲醛（ug/m3）");
    ui->widget_7->yAxis->setRange(0,50);
    ui->widget_7->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);


    ui->widget_8->xAxis->setLabel("时间轴(sec)");
    ui->widget_8->xAxis->setRange(0,300);
    ui->widget_8->yAxis->setLabel("噪音");
    ui->widget_8->yAxis->setRange(0,50);
    ui->widget_8->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);




    for(int i = 0;i <10;i++)
    {
        this->devices_seven_in_one[0].pen_color = Qt::blue;
        this->devices_seven_in_one[1].pen_color = Qt::red;
        this->devices_seven_in_one[2].pen_color = Qt::green;
        this->devices_seven_in_one[3].pen_color = Qt::black;
        this->devices_seven_in_one[4].pen_color = Qt::darkGray;
        this->devices_seven_in_one[5].pen_color = Qt::cyan;
        this->devices_seven_in_one[6].pen_color = Qt::magenta;

        this->devices_seven_in_one[7].pen_color = Qt::darkRed;
        this->devices_seven_in_one[8].pen_color = Qt::darkGreen;
        this->devices_seven_in_one[9].pen_color = Qt::darkBlue;
    }


    for(int i = 0;i <10;i++)
    {
        ui->widget_2->addGraph();
        ui->widget_2->graph(i)->setPen(QPen(this->temperature[i].pen_color));
        ui->widget_3->addGraph();
        ui->widget_3->graph(i)->setPen(QPen(this->temperature[i].pen_color));
        ui->widget_4->addGraph();
        ui->widget_4->graph(i)->setPen(QPen(this->temperature[i].pen_color));
        ui->widget_5->addGraph();
        ui->widget_5->graph(i)->setPen(QPen(this->temperature[i].pen_color));
        ui->widget_6->addGraph();
        ui->widget_6->graph(i)->setPen(QPen(this->temperature[i].pen_color));
        ui->widget_7->addGraph();
        ui->widget_7->graph(i)->setPen(QPen(this->temperature[i].pen_color));
        ui->widget_8->addGraph();
        ui->widget_8->graph(i)->setPen(QPen(this->temperature[i].pen_color));
    }

}

TemperatureWindows::~TemperatureWindows()
{
    delete ui;
}
void TemperatureWindows::UpdateTemperatureGraph(void)
{
    static int count = 0;
    int i = 0;
    count++;
    for( i = 0;i<10;i++)
    {
        ui->widget->graph(i)->addData(count,temperature[i].temperature_value);
    }
    ui->widget->replot();

    for( i = 0;i<10;i++)
    {
        ui->widget_2->graph(i)->addData(count,devices_seven_in_one[i].temperature_value);
    }
    ui->widget_2->replot();
    for( i = 0;i<10;i++)
    {
        ui->widget_3->graph(i)->addData(count,devices_seven_in_one[i].humidity_value);
    }
    ui->widget_3->replot();
    for( i = 0;i<10;i++)
    {
        ui->widget_4->graph(i)->addData(count,devices_seven_in_one[i].illumination);
    }
    ui->widget_4->replot();
    for( i = 0;i<10;i++)
    {
        ui->widget_5->graph(i)->addData(count,devices_seven_in_one[i].dust);
    }
    ui->widget_5->replot();
    for( i = 0;i<10;i++)
    {
        ui->widget_6->graph(i)->addData(count,devices_seven_in_one[i].CO2);
    }
    ui->widget_6->replot();
    for( i = 0;i<10;i++)
    {
        ui->widget_7->graph(i)->addData(count,devices_seven_in_one[i].methanal);
    }
    ui->widget_7->replot();
    for( i = 0;i<10;i++)
    {
        ui->widget_8->graph(i)->addData(count,devices_seven_in_one[i].noise);
    }
    ui->widget_8->replot();
    /*for( i = 0;i<10;i++)
    {
        ui->widget_9->graph(i)->addData(count,temperature[i].temperature_value);
    }
    ui->widget_9->replot();
    for( i = 0;i<10;i++)
    {
        ui->widget_10->graph(i)->addData(count,temperature[i].temperature_value);
    }
    ui->widget->replot();
*/
    //time_out++;
}



void TemperatureWindows::UpdateQinJianTemperature(QString arg)
{
    QString contain_str1 =NULL;
    QString contain_str2 =NULL;
    QString contain_str3 ="F0 06";
    int index = 0;

    int temperature_temp = 0;

    temperature[0].zigbee_str = ui->lineEdit->text();
    temperature[1].zigbee_str = ui->lineEdit_2->text();
    temperature[2].zigbee_str = ui->lineEdit_3->text();
    temperature[3].zigbee_str = ui->lineEdit_4->text();
    temperature[4].zigbee_str = ui->lineEdit_5->text();
    temperature[5].zigbee_str = ui->lineEdit_6->text();
    temperature[6].zigbee_str = ui->lineEdit_7->text();
    temperature[7].zigbee_str = ui->lineEdit_8->text();
    temperature[8].zigbee_str = ui->lineEdit_9->text();
    temperature[9].zigbee_str = ui->lineEdit_10->text();


    int devices_type = ui->DevicesSelect->currentIndex();
    switch(devices_type)
    {
        case 1: //拉低新风
        contain_str1 = "23 04";
        contain_str2 = "24 03";
        index = 45;
        if(-1 != arg.indexOf(contain_str2))
        {
            index = 45;
        }
        break;
    case 0: //琴键温控器
        contain_str1 = "23 04";
        contain_str2 = "24 04";
        index = 51;
        if(-1 != arg.indexOf(contain_str2))
        {
            index = 54;
        }
        break;
     case 2:
        break;
    }



    if((-1 == arg.indexOf(contain_str1))\
     &&(-1 == arg.indexOf(contain_str2))\
     &&(-1 == arg.indexOf(contain_str3)))
    {
        return ;
    }
    if(2 != devices_type)// 不是七合一温控器
    {
        if(-1 == arg.indexOf(contain_str3))
        {
            for(int i = 0;i<7;i++)
            {
                if(-1 != arg.indexOf(temperature[i].zigbee_str)&&(0 != temperature[i].zigbee_str.length()))
                {
                    temperature[i].temperature_str = arg.mid(index,2);
                    bool ok;
                    temperature[i].temperature_value = temperature[i].temperature_str.toInt(&ok,16);
                }
            }
        }
        else
        {
            for(int i = 7;i<9;i++)
            {
                if(-1 != arg.indexOf(temperature[i].zigbee_str)&&(0 != temperature[i].zigbee_str.length()))
                {
                    bool ok;
                    temperature[i].temperature_str = arg.mid(30,2);
                    temperature_temp = temperature[i].temperature_str.toInt(&ok,16);
                    temperature[i].temperature_value = temperature_temp*256;

                    temperature[i].temperature_str = arg.mid(33,2);
                    temperature_temp = temperature[i].temperature_str.toInt(&ok,16);
                    temperature[i].temperature_value = (temperature[i].temperature_value+temperature_temp)/10;

                }
            }
        }
    }

}


void TemperatureWindows::UpdateSevenInOne(QString arg)
{
    QString contain_active_report ="24 01 06";//心跳
    QString contain_heart_beat ="03 F0 06";//主动上报数据

    int index = 0;
    bool ok = false;
    QString temp_str = NULL;
    int temp_data = 0;

    devices_seven_in_one[0].zigbee_str = ui->lineEdit->text();
    devices_seven_in_one[1].zigbee_str = ui->lineEdit_2->text();
    devices_seven_in_one[2].zigbee_str = ui->lineEdit_3->text();
    devices_seven_in_one[3].zigbee_str = ui->lineEdit_4->text();
    devices_seven_in_one[4].zigbee_str = ui->lineEdit_5->text();
    devices_seven_in_one[5].zigbee_str = ui->lineEdit_6->text();
    devices_seven_in_one[6].zigbee_str = ui->lineEdit_7->text();
    devices_seven_in_one[7].zigbee_str = ui->lineEdit_8->text();
    devices_seven_in_one[8].zigbee_str = ui->lineEdit_9->text();
    devices_seven_in_one[9].zigbee_str = ui->lineEdit_10->text();


    int devices_type = ui->DevicesSelect->currentIndex();
/*    switch(devices_type)
    {
        case 1: //qi
        contain_str1 = "23 04";
        contain_str2 = "24 03";
        index = 45;
        if(-1 != arg.indexOf(contain_str2))
        {
            index = 45;
        }
        break;
    case 0: //琴键温控器
        contain_str1 = "23 04";
        contain_str2 = "24 04";
        index = 51;
        if(-1 != arg.indexOf(contain_str2))
        {
            index = 54;
        }
        break;
     case 2:
        break;
    }
*/


    if((-1 == arg.indexOf(contain_active_report))\
     &&(-1 == arg.indexOf(contain_heart_beat)))
    {
        return ;
    }

    for(int i = 0;i<10;i++)
    {
        if((-1 != arg.indexOf(devices_seven_in_one[i].zigbee_str))\
         &&(0 != devices_seven_in_one[i].zigbee_str.length()))
        {
                if(-1 != arg.indexOf(contain_active_report))
                {
                    temp_str = arg.mid(30,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].temperature_value = temp_data*256;
                    temp_str = arg.mid(33,2);
                    temp_data = temp_str.toInt(&ok,16);

                    devices_seven_in_one[i].temperature_value = (devices_seven_in_one[i].temperature_value+temp_data)/10;


                    temp_str = arg.mid(36,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].humidity_value = temp_data*256;
                    temp_str = arg.mid(39,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].humidity_value = (devices_seven_in_one[i].humidity_value+temp_data)/10;


                    temp_str = arg.mid(42,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].illumination = temp_data*256;
                    temp_str = arg.mid(45,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].illumination = (devices_seven_in_one[i].humidity_value+temp_data);

                    temp_str = arg.mid(48,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].dust = temp_data*256;
                    temp_str = arg.mid(51,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].dust = (devices_seven_in_one[i].dust+temp_data);

                    temp_str = arg.mid(54,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].CO2 = temp_data*256;
                    temp_str = arg.mid(57,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].CO2 = (devices_seven_in_one[i].CO2+temp_data);

                    temp_str = arg.mid(60,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].methanal = temp_data*256;
                    temp_str = arg.mid(63,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].methanal = (devices_seven_in_one[i].methanal+temp_data);

                }
                else  if(-1 != arg.indexOf(contain_heart_beat))
                {
                    temp_str = arg.mid(30,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].temperature_value = temp_data*256;
                    temp_str = arg.mid(33,2);
                    temp_data = temp_str.toInt(&ok,16);

                    devices_seven_in_one[i].temperature_value = (devices_seven_in_one[i].temperature_value+temp_data)/10;


                    temp_str = arg.mid(36,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].humidity_value = temp_data*256;
                    temp_str = arg.mid(39,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].humidity_value = (devices_seven_in_one[i].humidity_value+temp_data)/10;


                    temp_str = arg.mid(42,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].illumination = temp_data*256;
                    temp_str = arg.mid(45,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].illumination = (devices_seven_in_one[i].illumination+temp_data);

                    temp_str = arg.mid(48,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].dust = temp_data*256;
                    temp_str = arg.mid(51,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].dust = (devices_seven_in_one[i].dust+temp_data);

                    temp_str = arg.mid(54,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].CO2 = temp_data*256;
                    temp_str = arg.mid(57,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].CO2 = (devices_seven_in_one[i].CO2+temp_data);

                    temp_str = arg.mid(60,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].methanal = temp_data*256;
                    temp_str = arg.mid(63,2);
                    temp_data = temp_str.toInt(&ok,16);
                    devices_seven_in_one[i].methanal = (devices_seven_in_one[i].methanal+temp_data);
                }
        }
    }
}




void TemperatureWindows::ReceiveDataOk(QString arg)
{

    UpdateQinJianTemperature(arg);
    UpdateSevenInOne(arg);

}
