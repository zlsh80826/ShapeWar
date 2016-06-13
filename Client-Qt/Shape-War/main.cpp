#include "scene.h"
#include "view.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    printf("main() start\n");
    QApplication a(argc, argv);
    Scene scene;
    View view(&scene);
    view.show();

    return a.exec();
}
