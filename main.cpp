#include "taskscheduler.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TaskScheduler w;
    w.show();
    return a.exec();
}
