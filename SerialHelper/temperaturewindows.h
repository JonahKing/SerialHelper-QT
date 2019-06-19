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



class TemperatureWindows : public QMainWindow
{
    Q_OBJECT

public:
    explicit TemperatureWindows(QWidget *parent = 0);
    ~TemperatureWindows();
    QTimer *timer;
    GraphData temperature[10];


private:
    Ui::TemperatureWindows *ui;
private slots:
        void UpdateTemperatureGraph(void);
        void ReceiveTemperature(QString arg);
};

#endif // TEMPERATUREWINDOWS_H
