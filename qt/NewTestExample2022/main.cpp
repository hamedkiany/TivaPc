//QT4:#include <QtGui/QApplication>
#include <QtWidgets/QApplication>
#include "guipanel.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUIPanel w;
    w.show();
    
    return a.exec();
}
