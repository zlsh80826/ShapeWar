#include "scene.h"
#include "view.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    printf("main() start\n");
    QApplication a(argc, argv);
    Scene scene(0);
    QObject::connect(&scene, &Scene::closed, &a, &QCoreApplication::quit);
    View view(&scene, scene.m_webSocket);
    view.show();

    return a.exec();
}
