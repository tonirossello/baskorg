#ifndef JUGADOR_H
#define JUGADOR_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QVariant>

/*! \file */

//! Clase jugador
/*! La clase jugador hace referencia a la tabla 'jugadors' de la BBDD.
 * Encontramos las funciones correspodientes para poder cargar,
 * crear y actualizar nuevos registros en esta tabla.
 */
class jugador
{
    private:
        QSqlDatabase *m_db;
        int m_idJugador = 0;        /*!< Id del jugador. */
        int m_number = 0;           /*!< Dorsal del jugador. */
        QString m_name;             /*!< Nombre del jugador. */
        QString m_soci;             /*!< Socio del jugador. */
        QDate m_birthday;           /*!< Cumpleaños del jugador. */
        QString m_dni;              /*!< Dni del jugador. */
        QString m_phoneNumber1;     /*!< Móvil del jugador. */
        QString m_phoneNumber2;     /*!< Móvil 2 del jugador. */
        QString m_address;          /*!< Dirección del jugador. */
        QString m_city;             /*!< Ciudad del jugador. */
        QString m_email;            /*!< Email del jugador. */
        int m_idTeam;               /*!< Id del equipo del jugador. */
        int m_idGen;                /*!< Id del género del jugador. */
        int m_idClub;               /*!< Id del club del jugador. */


    public:
        jugador(QSqlDatabase *db);
        //GETTERS
        QString getName();
        int getNumber();
        int getTeamId();
        QDate getBirthday();
        QString getAddress();
        QString getCity();
        QString getPhoneNumber1();
        QString getPhoneNumber2();
        QString getSoci();
        QString getEmail();
        QString getDni();
        int getIdGen();
        int getIdClub();
        int getId();

        //SETTERS
        void setName(QString name);
        void setNumber(int number);
        void setTeamId(int idTeam);
        void setBirthday(QDate birthday);
        void setAddress(QString address);
        void setCity(QString city);
        void setPhoneNumber1(QString phoneNumber);
        void setPhoneNumber2(QString phoneNumber);
        void setSoci(QString soci);
        void setEmail(QString email);
        void setDni(QString dni);
        void setIdGen(int idGen);
        void setIdClub(int idClub);


        //BBDD Connectors
        bool save();
        void load(int id);
        bool delete_from_bd();

};

#endif // JUGADOR_H
