#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile File(":/src/style.qss");
    File.open(QIODevice::ReadOnly|QIODevice::Text);
    Widget w;
    w.setStyleSheet(File.readAll());
    w.show();
    return a.exec();
}
