#include "scene.h"
#include "view.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);

    QApplication a(argc, argv);
    // create a scene

    Scene scene(0);
    // if the scene close, quit this application
    QObject::connect(&scene, &Scene::closed, &a, &QCoreApplication::quit);

    // create a view
    View view(&scene, scene.m_webSocket);
    // shoe the view
    view.show();

    return a.exec();
}
