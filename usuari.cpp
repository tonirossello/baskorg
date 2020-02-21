#include "usuari.h"
#include <QDebug>
#include <QSqlError>


///GETTERS
QString usuari::getUser(){
    return m_user;
}

QString usuari::getPass(){
    return m_pass;
}

int usuari::getUserId(){
    return m_idUser;
}



///SETTERS

void usuari::setUser(QString idUser){
    m_user = idUser;
}

void usuari::setPass(QString pass){
    m_pass = pass;
}


bool usuari::save(){

    QSqlQuery q;

    if (m_idUser > 0)
    {
        qDebug() << "Update";
        ///UPDATE
        q.prepare("UPDATE jugadors SET usuari = :user, pass = :password WHERE id = :iduser");
        q.bindValue(":iduser", m_idUser);
        q.bindValue(":user", m_user);
        q.bindValue(":password", m_pass);

    }
    else
    {
        qDebug() << "Insert";

        ///INSERT
        q.prepare("INSERT INTO usuaris (usuari, pass) VALUES (:user, :password)");
        q.bindValue(":user", m_user);
        q.bindValue(":password", m_pass);
    } //end if

    bool result {q.exec()};

    qDebug() << result;
    qDebug() << q.lastError();
}

void usuari::load(int id){

    QSqlQuery q;

    q.prepare("SELECT * from usuaris where id = :iduser LIMIT 1");
    q.bindValue(":iduser", id);
    bool result {q.exec()};
    qDebug() << q.size();


    if (result)
    {
        q.next();
        m_idUser = id;
        m_user = q.value("usuari").toString();
        m_pass = q.value("pass").toString();

    } //end if

}
