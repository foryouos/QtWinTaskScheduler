#include "wintimetaskapi.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WinTimeTaskAPI w;
    w.show();
    return a.exec();
}
