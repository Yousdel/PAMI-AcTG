#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    widget w (argc, argv);
    w.show();
    return a.exec();
}
