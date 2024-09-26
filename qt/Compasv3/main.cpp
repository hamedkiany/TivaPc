#include <QApplication>
#include "guipanel.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUIPanel w;  // Crea un elemento con la clase del interfaz gráfico deseado (GUIPanel)
    w.show();    // Y lo hace aparecer en pantalla

    return a.exec();
}

