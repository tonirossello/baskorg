#include "apptest.h"
#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QSqlQuery>
#include "app.h"

using JSON = nlohmann::json;

extern bool g_logueado;
extern int g_id_usuario;

application::application(QObject *parent) : QObject(parent)
{
    QTimer::singleShot(0, this, SLOT(run()));
}


void application::run()
{
    qDebug() << "running...";

    m_context.run(); // run
    if (m_context.shouldExit())
    {
        QCoreApplication::quit();
    } // end if

}

Db::~Db()
{
    if (m_db.isOpen())
    {
        m_db.close();
    } // end if
}

bool Db::insertClub(std::string nombre, std::string color)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO clubs (nom, color) VALUES (:nom, :color)");
    if (nombre.length()!=0) query.bindValue(":nom", nombre.c_str());
    if (color.length()!=0) query.bindValue(":color", color.c_str());

    bool result = query.exec();
    qDebug() << query.lastError().text();

    return result;
}

bool Db::insertUser(std::string usuari, std::string pass)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO clubs (nom, color) VALUES (:nom, :color)");
    if (usuari.length()!=0) query.bindValue(":nom", pass.c_str());
    if (pass.length()!=0) query.bindValue(":color", pass.c_str());

    bool result = query.exec();
    qDebug() << query.lastError().text();

    return result;
}


Db::Db()
{
    if (!QSqlDatabase::contains( "testDB"))
     {
         m_db = QSqlDatabase::addDatabase("QMYSQL", "testDB");
     }
     else
     {
         m_db = QSqlDatabase::database("testDB");
     } // end if
     m_db.setHostName("localhost");
     m_db.setPort(3306);
     m_db.setUserName("pitu");
     m_db.setPassword("pitu");

}

void Db::close()
{
    m_db.close();
}

bool Db::init()
{
    bool result = 0;

        m_db.close();
        m_db.setDatabaseName("testDB");
        bool ok = m_db.open();
        if (ok)
        {
            qDebug() << "Borrando...";
            QSqlQuery q0("DROP DATABASE IF EXISTS testDB", m_db);
            if (q0.lastError().type() == QSqlError::NoError)
            {
                qDebug() << "Creando...";
                QSqlQuery q1("CREATE DATABASE testDB", m_db);
                if (q1.lastError().type() == QSqlError::NoError)
                {
                    m_db.close();
                    m_db.setDatabaseName("testDB");
                    m_db.open();
                    qDebug() << "ini" << m_db.databaseName();
                    /// Crea estructura en la base de datos
                    QString sql {"CREATE TABLE `clubs` ( \
                                 `id` int(8) AUTO_INCREMENT PRIMARY KEY, \
                                 `nom` varchar(30) NOT NULL, \
                                 `codi` varchar(8), \
                                 `color` varchar(6) NOT NULL, \
                                 `propietari` int(10) \
                               );"};

                                 QString sql2 {"CREATE TABLE `usuaris` ( \
                                              `id` int(10) AUTO_INCREMENT PRIMARY KEY, \
                                              `usuari` varchar(255) NOT NULL, \
                                              `pass` varchar(255) \
                                            );"};


                    qDebug() << "Iniciando...";
                    QSqlQuery q2(sql, m_db);
                    QSqlQuery q3(sql2, m_db);
                    if (q2.lastError().type() == QSqlError::NoError)
                    {
                        result = true;
                    } // end if
                }
                else
                {
                    qDebug() << "BB:" << m_db.lastError().text();
                } // end if

            }
            else
            {
                qDebug() << "AA:" << m_db.lastError().text();
            } // end if

       } // end if

        return result;
}

QSqlError Db::lastError()
{
    return m_db.lastError();
}


TEST_CASE("Exists campo en JSON")
{
    Db db;
    db.init();
    app app(db.m_db,false);

    JSON respuesta;
    respuesta["type"] = "type";
    respuesta["exists"] = "";

    CHECK( app.exists(respuesta, "type") == true );
    CHECK( app.exists(respuesta, "exists") == true );
    CHECK( app.exists(respuesta, "false") == false );

}

TEST_CASE("Logout")
{
    Db db;
    db.init();
    //mirar si la variable g_logueado está en false
    app app(db.m_db,false);
    app.logout(nullptr);

    CHECK(app.getLogueado() == false);

}

TEST_CASE("Crear club")
{
    Db db;
    db.init();
    app app(db.m_db,false);

    JSON test;  test["type"] = "clubCreate"; test["club_name"] = "nom"; test["club_color"] = "000000"; test["user"] = "0";
    JSON test2;  test2["type"] = "clubCreate"; test2["club_name"] = "nom"; test2["club_color"] = "00000011"; test2["user"] = "0";

    CHECK ( app.clubCreate(nullptr, test) == true);
    CHECK ( app.clubCreate(nullptr, test2) == false);
}

TEST_CASE("Crear usuario")
{
    Db db;

    int ok = db.init();
    qDebug() << "TEST " << db.m_db.databaseName();
    if (ok)
    {
        db.m_db.open();
        app app(db.m_db,false);


        JSON test3;  test3["type"] = "userCreate"; test3["usuari"] = ""; test3["password"] = "";
        JSON test4;  test4["type"] = "userCreate"; test4["usuari"] = "test"; test4["password"] = "test";
        JSON test5;  test5["type"] = "userCreate";  test5["usuari"] = ""; test5["password"] = "prova";
        JSON test6;  test6["type"] = "userCreate";  test6["usuari"] = "prova"; test6["password"] = "";

        CHECK ( app.userCreate(nullptr, test3) == true);
        CHECK ( app.userCreate(nullptr, test4) == true);
        CHECK ( app.userCreate(nullptr, test5) == true);
        CHECK ( app.userCreate(nullptr, test6) == true);
    } //end if

}


TEST_CASE("base datos")
{
    Db db;
    int ok = db.init();

    if (ok)
    {
        qDebug() << "DB UP AND RUNNING";

        SUBCASE("Insertar Club #0")
        {
            CHECK( db.insertClub("prueba", "ffffff") == true );
        }

        SUBCASE("Insertar Club #1")
        {
            CHECK( db.insertClub("no_color", "") == false );
        }

        SUBCASE("Insertar Club #2")
        {
            CHECK( db.insertClub("", "000000") == false );
        }

        SUBCASE("Insertar Club #3")
        {
            CHECK( db.insertClub("color_too_large", "fffffffffffff") == false );
        }

        SUBCASE("Insertar Club #4")
        {
            CHECK( db.insertClub("", "") == false );
        }

        SUBCASE("Insertar Usuario #0")
        {
            CHECK( db.insertUser("", "") == false );
        }

        SUBCASE("Insertar Usuario #1")
        {
            CHECK( db.insertUser("", "sinusuario") == false );
        }

        SUBCASE("Insertar Usuario #2")
        {
            CHECK( db.insertUser("sinpass", "") == false );
        }

        SUBCASE("Insertar Usuario #3")
        {
            CHECK( db.insertUser("test", "test") == true );
        }
    }
    else
    {
        qDebug() << db.lastError().text();

    } // end if

}




