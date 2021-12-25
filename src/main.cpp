#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/res/freenet.png"));
    Dialog w;
    w.show();
    return a.exec();
}
