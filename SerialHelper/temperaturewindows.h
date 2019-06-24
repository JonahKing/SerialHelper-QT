#ifndef TEMPERATUREWINDOWS_H
#define TEMPERATUREWINDOWS_H

#include <QMainWindow>
#include <QTimer>
namespace Ui {
class TemperatureWindows;
}

class GraphData
{
public:
   QColor pen_color;
   int ZIgbee_ID;
   int temperature_value;
   QString zigbee_str;
   QString temperature_str;
};


class SevenInOne
{
public:
   QColor pen_color;
   QString zigbee_str;
   int ZIgbee_ID;
   float temperature_value;
   float humidity_value;
   float illumination;
   float dust;
   float CO2;
   float methanal;
   float noise;

};








class TemperatureWindows : public QMainWindow
{
    Q_OBJECT

public:
    explicit TemperatureWindows(QWidget *parent = 0);
    ~TemperatureWindows();
    QTimer *timer;
    GraphData temperature[10];

    SevenInOne devices_seven_in_one[10];

    void UpdateQinJianTemperature(QString arg);
    void UpdateSevenInOne(QString arg);


private:
    Ui::TemperatureWindows *ui;
private slots:
        void UpdateTemperatureGraph(void);
        void ReceiveDataOk(QString arg);
};

#endif // TEMPERATUREWINDOWS_H
