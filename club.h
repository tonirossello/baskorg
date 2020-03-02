#ifndef CLUB_H
#define CLUB_H
#include <QString>
#include <QSqlDatabase>

/*! \file */

//! Clase club
/*! La clase club hace referencia a la tabla 'clubs' de la BBDD.
 * Encontramos las funciones correspodientes para poder cargar,
 * crear y actualizar nuevos registros en esta tabla.
 */
class club
{

private:
    QSqlDatabase *m_db;
    int m_idClub = 0;       /*!< Id del club. */
    QString m_name;         /*!< Nombre del club. */
    QString m_codi;         /*!< Código del club. */
    QString m_color;        /*!< Color del club. */
    int m_propietario;      /*!< Id del propietario del club. */


public:
    club(QSqlDatabase *db);
    //GETTERS
    QString getName();
    int getClubId();
    QString getCodi();
    QString getColor();
    int getIdPropietari();

    //SETTERS
    void setName(QString name);
    void setCodi(QString codi);
    void setColor(QString color);
    void setIdPropietario(int getIdPropietario);


    //BBDD Connectors
    bool save();
    void load(int id);
};

#endif // CLUB_H
