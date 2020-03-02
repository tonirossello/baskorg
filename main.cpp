#include <QSqlDatabase>
#include <QSqlRecord>
#include <QTranslator>
#include <QCoreApplication>
#include <QLocale>
#include "app.h"

/*! \file */

/*! \mainpage Documentación de Baskorg
 * Baskorg es una aplicación de gestión de clubes.
 *
 * Actualmente se encuentra en fase de desarrollo.
 *
 * \version 0.01
 *
 **/


/**
 * @brief Función principal del programa
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //cargamos archivo de traducción
    QTranslator myappTranslator;
    myappTranslator.load("myapp_" + QLocale::system().name());
    a.installTranslator(&myappTranslator);

    //conexión a la base de datos
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("cbsacabaneta");
    db.setPort(3306);
    db.setUserName("pitu");
    db.setPassword("pitu");
    db.open();

    app app(db, true);

}



