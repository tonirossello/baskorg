#define DOCTEST_CONFIG_IMPLEMENT
#include <QCoreApplication>
#include "apptest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    application application;

    return a.exec();
}


