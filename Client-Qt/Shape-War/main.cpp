#include "scene.h"
#include "view.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    printf("main() start\n");
    QApplication a(argc, argv);
    Scene scene(0, QUrl(QStringLiteral("ws://localhost:8888/arena/dummy")));
    QObject::connect(&scene, &Scene::closed, &a, &QCoreApplication::quit);
    View view(&scene);
    view.show();

    return a.exec();
}
