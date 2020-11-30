#include "serveurwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Serveurwidget w;
    w.show();
    return a.exec();
}
