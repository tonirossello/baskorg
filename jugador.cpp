#include "jugador.h"
#include <QDebug>

///GETTERS
QString jugador::getName(){
    return m_name;
}

int jugador::getNumber(){
    return m_number;
}

int jugador::getTeamId(){
    return m_idTeam;
}

QDate jugador::getBirthday(){
    return m_birthday;
}

QString jugador::getAddress(){
    return m_address;
}

QString jugador::getCity(){
    return m_city;
}

QString jugador::getPhoneNumber1(){
    return m_phoneNumber1;
}

QString jugador::getPhoneNumber2(){
    return m_phoneNumber2;
}

QString jugador::getSoci(){
    return m_soci;
}

QString jugador::getEmail(){
    return m_email;
}
QString jugador::getDni(){
    return m_dni;
}

int jugador::getIdGen(){
    return m_idGen;
}


///SETTERS

void jugador::setName(QString name){
    m_name = name;
}


void jugador::setNumber(int number){
    m_number = number;
}

void jugador::setTeamId(int teamId){
    m_idTeam = teamId;
}

void jugador::setBirthday(QDate birthday){
    m_birthday = birthday;
}

void jugador::setAddress(QString address){
    m_address = address;
}

void jugador::setCity(QString city){
    m_city = city;
}

void jugador::setPhoneNumber1(QString phoneNumber){
    m_phoneNumber1 = phoneNumber;
}

void jugador::setPhoneNumber2(QString phoneNumber){
    m_phoneNumber2 = phoneNumber;
}

void jugador::setSoci(QString soci){
    m_soci = soci;
}

void jugador::setEmail(QString email){
    m_email = email;
}

void jugador::setDni(QString dni){
    m_dni = dni;
}

void jugador::setIdGen(int idGen){
    m_idGen = idGen;
}




bool jugador::save(){

    QSqlQuery q;

    if (m_idJugador > 0)
    {
        ///UPDATE
        q.prepare("UPDATE jugadors SET Dorsal = :dorsal, Data_naixement = :data WHERE id = :idjugador");
        q.bindValue(":idjugador", m_idJugador);
        q.bindValue(":dorsal", m_number);
        q.bindValue(":data", m_birthday);

    }
    else
    {
        ///INSERT
        q.prepare("INSERT INTO jugadors (Dorsal,Data_naixement) VALUES (:dorsal, :data)");
        q.bindValue(":dorsal", m_number);
        q.bindValue(":data", m_birthday);
    } //end if

    bool result {q.exec()};

    qDebug() << result;
}

void jugador::load(int id){
    QSqlQuery q;

    q.prepare("SELECT * from jugadors WHERE id = :idJugador LIMIT 1")
}
