#ifndef APPTEST_H
#define APPTEST_H

#include "doctest.h"
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>

class Db
{

public:
    QSqlDatabase m_db;
    Db();
    ~Db();
    bool init();
    void close();
    QSqlError lastError();
    bool insertClub(std::string nombre, std::string color);
    bool insertUser(std::string usuari, std::string pass);
};

class application : public QObject
{
    Q_OBJECT
    doctest::Context m_context;

public:
    application(QObject *parent = nullptr);

public slots:
    void run();
};

#endif // APPTEST_H
