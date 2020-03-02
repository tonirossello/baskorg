#include "club.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

/*! \file */
club::club(QSqlDatabase *db) : m_db(db)
{
    qDebug() << "Club " << m_db->database();
}

//GETTERS
/**
 * @brief Obtener el nombre del club
 * @return Nombre del club
 */
QString club::getName(){
    return m_name;
}

/**
 * @brief Obtener el código del club
 * @return Código del club
 */
QString club::getCodi(){
    return m_codi;
}

/**
 * @brief Obtener el propietario del club
 * @return Id del propietario del club
 */
int club::getIdPropietari(){
    return m_propietario;
}

/**
 * @brief Obtener el color del club
 * @return Color del club
 */
QString club::getColor(){
    return m_color;
}

/**
 * @brief Obtener el id del club
 * @return Id del club
 */
int club::getClubId(){
    return m_idClub;
}


//SETTERS
/**
 * @brief Modificar el nombre del club
 * @param name --> Nuevo nombre del club
 */
void club::setName(QString name){
    m_name = name;
}

/**
 * @brief Modificar el código del club
 * @param codi --> Nuevo código del club
 */
void club::setCodi(QString codi){
    m_codi = codi;
}

/**
 * @brief Modificar el propietario del club
 * @param idPropietario --> Nuevo propietario del club
 */
void club::setIdPropietario(int idPropietario){
    m_propietario = idPropietario;
}

/**
 * @brief Modificar el color del club
 * @param color --> Nuevo color del club
 */
void club::setColor(QString color){
    m_color = color;
}


//Operaciones de la BBDD
/**
 * @brief Actualiza o inserta un nuevo club a la BBDD
 * @return Si se ha realizado la consulta sin errores
 */
bool club::save(){

    QSqlQuery q(*m_db);

    if (m_idClub > 0)
    {
        qDebug() << "Update";
        //UPDATE
        q.prepare("UPDATE clubs SET nom = :name, codi = :codi, color = :color, propietario = :propietari");
        q.bindValue(":name", m_name);
        q.bindValue(":codi", m_codi);
        q.bindValue(":color", m_color);
        q.bindValue(":propietari", m_propietario);

    }
    else
    {
        qDebug() << "Insert";

        //INSERT
        q.prepare("INSERT INTO clubs (nom , color, propietari) VALUES (:name, :color, :propietari)");
        q.bindValue(":name", m_name);
        q.bindValue(":codi", m_codi);
        q.bindValue(":color", m_color);
        q.bindValue(":propietari", m_propietario);
    } //end if

    bool result {q.exec()};

    qDebug() << result;
    qDebug() << q.lastError();

    return result;
}

/**
 * @brief Carga los datos de un club en el objeto club
 * @param id --> id del club a cargar
 */
void club::load(int id){

    QSqlQuery q(*m_db);

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
