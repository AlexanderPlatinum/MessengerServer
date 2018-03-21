#include <QCoreApplication>

#include "Application.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Application *app = new Application();
    app->Run();

    return a.exec();
}
