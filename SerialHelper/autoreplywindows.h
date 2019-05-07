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
private slots:
    void settingTableChanged(int row, int col);
    void test(void);
};

#endif // AUTOREPLYWINDOWS_H