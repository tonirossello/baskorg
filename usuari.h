#ifndef USUARI_H
#define USUARI_H

#include <QString>
#include <QSqlQuery>
#include <QVariant>

class usuari
{

private:
    int m_idUser = 0;
    QString m_user;
    QString m_pass;

public:

    ///GETTERS
    int getUserId();
    QString getUser();
    QString getPass();

    ///SETTERS
    void setUser(QString user);
    void setPass(QString pass);


    ///BBDD Connectors
    bool save();
    void load(int id);
};

#endif // USUARI_H
