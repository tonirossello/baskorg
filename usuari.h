#ifndef USUARI_H
#define USUARI_H

#include <QString>
#include <QSqlQuery>
#include <QVariant>

/*! \file */

//! Clase usuario
/*! La clase usuario hace referencia a la tabla 'usuaris' de la BBDD.
 * Encontramos las funciones correspodientes para poder cargar,
 * crear y actualizar nuevos registros en esta tabla.
 */
class usuari
{

private:
    QSqlDatabase *m_db;
    int m_idUser = 0;           /*!< Id del usuario. */
    QString m_user;             /*!< Nombre del usuario. */
    QString m_pass;             /*!< Contraseña del usuario. */

public:
    usuari(QSqlDatabase *db);
    //GETTERS
    int getUserId();
    QString getUser();
    QString getPass();

    //SETTERS
    void setUser(QString user);
    void setPass(QString pass);

    //BBDD Connectors
    bool save();
    void load(int id);
};

#endif // USUARI_H
