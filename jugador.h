#ifndef JUGADOR_H
#define JUGADOR_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QVariant>


/*socket.send('{"type":"playerCreate","payload": {"name":"NombreJugador","State":"Balearican Islands","country":"Spain","number":"42",
                        "teamId":"passedByDefault","birthday":"1/1/1999", "address":"olesa 4", "city":"Sa Cabaneta", "zip":"07141", "phoneNumber":"654321987"}}')*/
class jugador
{
    private:
        int m_idJugador = 0;
        int m_number = 0;
        QString m_name;
        QString m_soci;
        QDate m_birthday;
        QString m_dni;
        QString m_phoneNumber1;
        QString m_phoneNumber2;
        QString m_address;
        QString m_city;
        QString m_email;
        int m_idTeam;
        int m_idGen;
        int m_idClub;


    public:

        ///GETTERS
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

        ///SETTERS
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


        ///BBDD Connectors
        bool save();
        void load(int id);

};

#endif // JUGADOR_H
