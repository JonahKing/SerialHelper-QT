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
    explicit AutoReplyWindows(QWidget *parent = 0);
    void ParameterInit(void);
    ~AutoReplyWindows();

private:
    Ui::AutoReplyWindows *ui;
private slots:
    void ReceiveDataOk(QString arg);
    void SaveUserSetting(int);
    void SaveUserSetting(QString arg);
    void send_button_fun(bool);

};

#endif // AUTOREPLYWINDOWS_H
