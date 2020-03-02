#include "usuari.h"
#include <QDebug>
#include <QSqlError>

/*! \file */
usuari::usuari(QSqlDatabase *db) : m_db(db)
{
    qDebug() << "Usuari " << m_db->databaseName();
}

//GETTERS

/**
 * @brief Obtener el nombre del usuario
 * @return Código del usuario
 */
QString usuari::getUser(){
    return m_user;
}

/**
 * @brief Obtener la contraseña del usuario
 * @return Contraseña del usuario
 */
QString usuari::getPass(){
    return m_pass;
}

/**
 * @brief Obtener el id del usuario
 * @return Id del usuario
 */
int usuari::getUserId(){
    return m_idUser;
}



//SETTERS
/**
 * @brief Modificar el nombre de un usuario
 * @param username --> Nuevo nombre de usuario
 */
void usuari::setUser(QString username){
    m_user = username;
}

/**
 * @brief Modificar la contraseña de un usuario
 * @param pass --> Nueva contraseña
 */
void usuari::setPass(QString pass){
    m_pass = pass;
}


//Operaciones de la BBDD

/**
 * @brief Actualiza o inserta un nuevo usuario a la BBDD
 * @return Si se ha realizado la consulta sin errores
 */
bool usuari::save(){

    QSqlQuery q(*m_db);

    if (m_idUser > 0)
    {
        qDebug() << "Update";
        //UPDATE
        q.prepare("UPDATE jugadors SET usuari = :user, pass = :password WHERE id = :iduser");
        q.bindValue(":iduser", m_idUser);
        q.bindValue(":user", m_user);
        q.bindValue(":password", m_pass);

    }
    else
    {
        qDebug() << "Insert";

        //INSERT
        q.prepare("INSERT INTO usuaris (usuari, pass) VALUES (:user, :password)");
        q.bindValue(":user", m_user);
        q.bindValue(":password", m_pass);
    } //end if

    bool result {q.exec()};

    qDebug() << result;
    qDebug() << q.lastError();

    return result;
}

/**
 * @brief Carga los datos de un usuario en el objeto usuario
 * @param id --> id del usuario a cargar
 */
void usuari::load(int id){

    QSqlQuery q(*m_db);

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
