#include <QSqlDatabase>
#include <QSqlRecord>
#include <QTranslator>
#include <QCoreApplication>
#include <QLocale>
#include "app.h"
#include <QSettings>
#include <QDir>

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

    //Conexión a la BBDD
    QSettings settings("../baskorg/config.prop", QSettings::IniFormat);

    qDebug() << QDir::currentPath();
    QString driver = settings.value("driver").toString();
    QString host = settings.value("host").toString();
    QString database = settings.value("database").toString();
    int port = settings.value("port").toInt();
    QString username = settings.value("username").toString();
    QString password = settings.value("password").toString();

    QSqlDatabase db = QSqlDatabase::addDatabase(driver);
    db.setHostName(host);
    db.setDatabaseName(database);
    db.setPort(port);
    db.setUserName(username);
    db.setPassword(password);
    db.open();


    app app(db, true);

}



