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
        //this->temperature[i] = new GraphData();
        this->temperature[i].ZIgbee_ID = i;
    }

    this->temperature[0].pen_color = Qt::blue;
    this->temperature[1].pen_color = Qt::red;
    this->temperature[2].pen_color = Qt::green;
    this->temperature[3].pen_color = Qt::black;
    this->temperature[4].pen_color = Qt::yellow;
    this->temperature[5].pen_color = Qt::cyan;
    this->temperature[6].pen_color = Qt::magenta;


    this->temperature[7].pen_color = Qt::darkRed;
    this->temperature[8].pen_color = Qt::darkGreen;
   // darkBlue,
    for(int i = 0; i<10;i++)
    {
        ui->widget->addGraph();
        ui->widget->graph(i)->setPen(QPen(this->temperature[i].pen_color));
    }


    int x[10],y[10];
    for(int i = 0 ;i< 10;i++)
    {
        x[i] = i;
        y[i] = i;
      //  ui->widget->graph(0)->addData(x[i],y[i]);
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateTemperatureGraph()));
    timer->start(1000);

}

TemperatureWindows::~TemperatureWindows()
{
    delete ui;
}
void TemperatureWindows::UpdateTemperatureGraph(void)
{
    static int count = 0;
    count++;
    for(int i = 0;i<10;i++)
    {
        ui->widget->graph(i)->addData(count,temperature[i].temperature_value);
    }
    ui->widget->replot();
    //time_out++;
}

void TemperatureWindows::ReceiveTemperature(QString arg)
{
    QString contain_str1 =NULL;
    QString contain_str2 =NULL;
    QString contain_str3 ="F0 06";
    int index = 0;

    int temperature_temp = 0;
    if(1 == ui->DevicesSelect->currentIndex())
    {
        contain_str1 = "23 04";
        contain_str2 = "24 03";
        index = 45;
        if(-1 != arg.indexOf(contain_str2))
        {
            index = 45;
        }
    }
    else if(0 == ui->DevicesSelect->currentIndex())
    {
        contain_str1 = "23 04";
        contain_str2 = "24 04";
        index = 51;
        if(-1 != arg.indexOf(contain_str2))
        {
            index = 54;
        }
    }

    if((-1 == arg.indexOf(contain_str1))\
     &&(-1 == arg.indexOf(contain_str2))\
     &&(-1 == arg.indexOf(contain_str3)))
    {
        return ;
    }
    if(-1 == arg.indexOf(contain_str3))
    {
        if(0 == temperature[0].zigbee_str.length())
        {
             temperature[0].zigbee_str = arg.mid(6,12);
        }
        else if((-1 == arg.indexOf(temperature[0].zigbee_str))
              &&(0 == temperature[1].zigbee_str.length()))
        {
            temperature[1].zigbee_str = arg.mid(6,12);
        }
        else if((-1 == arg.indexOf(temperature[0].zigbee_str))
             && (-1 == arg.indexOf(temperature[1].zigbee_str))
             && (0 == temperature[2].zigbee_str.length()))
        {
            temperature[2].zigbee_str = arg.mid(6,12);
        }
        else if((-1 == arg.indexOf(temperature[0].zigbee_str))
             && (-1 == arg.indexOf(temperature[1].zigbee_str))
             && (-1 == arg.indexOf(temperature[2].zigbee_str))
             &&(0 == temperature[3].zigbee_str.length()))
        {
            temperature[3].zigbee_str = arg.mid(6,12);

        }
        else if((-1 == arg.indexOf(temperature[0].zigbee_str))
             && (-1 == arg.indexOf(temperature[1].zigbee_str))
             && (-1 == arg.indexOf(temperature[2].zigbee_str))
             && (-1 == arg.indexOf(temperature[3].zigbee_str))
             &&(0 == temperature[4].zigbee_str.length()))
        {
            temperature[4].zigbee_str = arg.mid(6,12);

        }
        else if((-1 == arg.indexOf(temperature[0].zigbee_str))
             && (-1 == arg.indexOf(temperature[1].zigbee_str))
             && (-1 == arg.indexOf(temperature[2].zigbee_str))
             && (-1 == arg.indexOf(temperature[3].zigbee_str))
             && (-1 == arg.indexOf(temperature[4].zigbee_str))
             &&(0 == temperature[5].zigbee_str.length()))
        {
            temperature[5].zigbee_str = arg.mid(6,12);

        }
        else if((-1 == arg.indexOf(temperature[0].zigbee_str))
             && (-1 == arg.indexOf(temperature[1].zigbee_str))
             && (-1 == arg.indexOf(temperature[2].zigbee_str))
             && (-1 == arg.indexOf(temperature[3].zigbee_str))
             && (-1 == arg.indexOf(temperature[4].zigbee_str))
             && (-1 == arg.indexOf(temperature[5].zigbee_str))
             &&(0 == temperature[6].zigbee_str.length()))
        {
            temperature[6].zigbee_str = arg.mid(6,12);

        }
    }

    else
    {
        if(0 == temperature[7].zigbee_str.length())
        {
            temperature[7].zigbee_str = arg.mid(6,12);

        }
        else if((-1 == arg.indexOf(temperature[7].zigbee_str))
         &&(0 == temperature[8].zigbee_str.length()))
        {
            temperature[8].zigbee_str = arg.mid(6,12);

        }
     }

    for(int i = 0;i<7;i++)
    {
        if(-1 != arg.indexOf(temperature[i].zigbee_str)&&(0 != temperature[i].zigbee_str.length()))
        {
            temperature[i].temperature_str = arg.mid(index,2);
            bool ok;
            temperature[i].temperature_value = temperature[i].temperature_str.toInt(&ok,16);
        }
    }
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
