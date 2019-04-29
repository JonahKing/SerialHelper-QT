#ifndef AUTOREPLYWINDOWS_H
#define AUTOREPLYWINDOWS_H

#include <QMainWindow>

namespace Ui {
class AutoReplyWindows;
}

class AutoReplyWindows : public QMainWindow
{
    Q_OBJECT

public:
    explicit AutoReplyWindows(QWidget *parent = 0);
    ~AutoReplyWindows();

private:
    Ui::AutoReplyWindows *ui;
};

#endif // AUTOREPLYWINDOWS_H
