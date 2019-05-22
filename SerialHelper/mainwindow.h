#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "serialconfig.h"
#include "autoreplywindows.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void BuffReceivefinished(QString);
public:
    explicit MainWindow(QWidget *parent = 0);
    void ParameterSave(QString Type, QString p);
    ~MainWindow();
private:
    QSerialPort *port;
    Ui::MainWindow *ui;
    SerialConfig *serial_config;
    QTimer *timer;
    QTimer *frame_duration_timer;
     QString fileName;
     QString fileContent;//读到的文件内容

     AutoReplyWindows *p_auto_reply_windows;

     void readFile();
     void saveTextToFile();
     void ParameterInit(void);


    inline QSerialPort::DataBits getbit(int bit)        //
    {
        switch (bit)
        {
        case 5:
            return QSerialPort::Data5;
        case 6:
            return QSerialPort::Data6;
        case 7:
            return QSerialPort::Data7;
        default:
            return QSerialPort::Data8;
        }
    };


private slots:
    void AutoSend(QString str);
    void readread();
    void on_SerialOnoffBUtton_clicked();

    void on_REceiveStopButton_clicked();

    void on_FrameDuration_editingFinished();

    void on_SendDataTimming_stateChanged(int);
    void on_SendDataButton_clicked();
    void on_ReceiveDataBrowser_textChanged();

    void TimerSend(void);
    void openFileSlot();
    void saveFileSlot();
    void ForceHexAlign(const QString &arg1);
    void SaveUserSetting(const QString &arg1);
    void on_FrameDuration_textChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
