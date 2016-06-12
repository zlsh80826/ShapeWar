#include "scene.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    printf("main() start\n");
    QApplication a(argc, argv);
    Scene scene;
    scene.show();

    return a.exec();
}
