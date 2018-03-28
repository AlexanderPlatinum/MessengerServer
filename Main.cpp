#include <QCoreApplication>

#include "Application.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Application app;
    app.run();

    return a.exec();
}
