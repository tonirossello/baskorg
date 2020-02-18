#include "club.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

///GETTERS
QString club::getName(){
    return m_name;
}

QString club::getCodi(){
    return m_codi;
}

int club::getIdPropietari(){
    return m_propietario;
}

QString club::getColor(){
    return m_color;
}

int club::getClubId(){
    return m_idClub;
}


///SETTERS

void club::setName(QString name){
    m_name = name;
}

void club::setCodi(QString codi){
    m_codi = codi;
}

void club::setIdPropietario(int idPropietario){
    m_propietario = idPropietario;
}

void club::setColor(QString color){
    m_color = color;
}


bool club::save(){

    QSqlQuery q;

    if (m_idClub > 0)
    {
        qDebug() << "Update";
        ///UPDATE
        q.prepare("UPDATE clubs SET nom = :name, codi = :codi, color = :color, propietario = :propietari");
        q.bindValue(":name", m_name);
        q.bindValue(":codi", m_codi);
        q.bindValue(":color", m_color);
        q.bindValue(":propietari", m_propietario);

    }
    else
    {
        qDebug() << "Insert";

        ///INSERT
        q.prepare("INSERT INTO clubs (nom , color, propietari) VALUES (:name, :color, :propietari)");
        q.bindValue(":name", m_name);
        q.bindValue(":codi", m_codi);
        q.bindValue(":propietari", m_propietario);
    } //end if

    bool result {q.exec()};

    qDebug() << result;
    qDebug() << q.lastError();
}

void club::load(int id){

    QSqlQuery q;

    q.prepare("SELECT * from clubs where id = :idClub LIMIT 1");
    q.bindValue(":idClub", id);
    q.exec();
    qDebug() << "ID: " << id << "SIZE: " << q.size();


    if (q.next())
    {
        m_idClub = id;
        m_name = q.value("nom").toString();
        m_codi = q.value("codi").toString();
        m_color = q.value("color").toString();
        m_propietario = q.value("propietari").toInt();

      qDebug() <<  m_name + m_codi + m_color + m_propietario ;
    } //end if

}
