#ifndef AUTOREPLYWINDOWS_H
#define AUTOREPLYWINDOWS_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
namespace Ui {
class AutoReplyWindows;
}

class AutoReplyWindows : public QMainWindow
{
    Q_OBJECT
signals:
    void AutoReplyToWindows(QString arg);
public:

    QString fileName;

    explicit AutoReplyWindows(QWidget *parent = 0);
    void ParameterInit(void);
    ~AutoReplyWindows();
    void readFile(void);
    Ui::AutoReplyWindows *ui;

private:

private slots:
    void ReceiveDataOk(QString arg);
    void SaveUserSetting(int);
    void SaveUserSetting(QString arg);
    void send_button_fun(bool);
    void Excel_send_button_fun();
    void on_InsertExcel_clicked();
    void on_MacReplace_stateChanged(int arg1);
};

#endif // AUTOREPLYWINDOWS_H
