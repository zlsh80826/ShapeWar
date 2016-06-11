#include "scene.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Scene scene;
    scene.show();

    return a.exec();
}
