#include "jugador.h"
#include <QDebug>
#include <QSqlError>

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
        qDebug() << "Update";
        ///UPDATE
        q.prepare("UPDATE jugadors SET dorsal = :dorsal, naixement = :data, nom = :name, soci = :soci, dni = :dni, tfn1 = :tfn1, tfn2 = :tfn2, adresa = :adresa, poblacio = :poblacio, email = :email WHERE id = :idjugador");
        q.bindValue(":idjugador", m_idJugador);
        q.bindValue(":dorsal", m_number);
        q.bindValue(":name", m_name);
        q.bindValue(":soci", m_soci);
        q.bindValue(":dni", m_dni);
        q.bindValue(":tfn1", m_phoneNumber1);
        q.bindValue(":tfn2", m_phoneNumber2);
        q.bindValue(":adresa", m_address);
        q.bindValue(":poblacio", m_city);
        q.bindValue(":email", m_email);
        q.bindValue(":data", m_birthday);

    }
    else
    {
        qDebug() << "Insert";

        ///INSERT
        q.prepare("INSERT INTO jugadors (dorsal,naixement, nom, soci, dni, tfn1, tfn2,adresa, poblacio, email) VALUES (:dorsal, :data, :name, :soci, :dni, :tfn1, :tfn2, :adresa, :poblacio, :email)");
        q.bindValue(":idjugador", m_idJugador);
        q.bindValue(":dorsal", m_number);
        q.bindValue(":name", m_name);
        q.bindValue(":soci", m_soci);
        q.bindValue(":dni", m_dni);
        q.bindValue(":tfn1", m_phoneNumber1);
        q.bindValue(":tfn2", m_phoneNumber2);
        q.bindValue(":adresa", m_address);
        q.bindValue(":poblacio", m_city);
        q.bindValue(":email", m_email);
        q.bindValue(":data", m_birthday);
    } //end if

    bool result {q.exec()};

    qDebug() << result;
    qDebug() << q.lastError();
}

void jugador::load(int id){

    QSqlQuery q;

    q.prepare("SELECT * from jugadors where id = :idJugador LIMIT 1");
    q.bindValue(":idJugador", id);
    bool result {q.exec()};
    qDebug() << q.size();


    if (result)
    {
        q.next();
        m_idJugador = id;
        m_number = q.value("dorsal").toInt();
        m_name = q.value("nom").toString();
        m_soci = q.value("soci").toString();
        m_birthday = q.value("naixement").toDate();
        m_dni = q.value("dni").toString();
        m_phoneNumber1 = q.value("tfn1").toString();
        m_phoneNumber2 = q.value("tfn2").toString();
        m_address = q.value("adresa").toString();
        m_city = q.value("poblacio").toString();
        m_email = q.value("email").toString();
        m_idTeam = q.value("idequip").toInt();
        m_idGen = q.value("idgen").toInt();

      qDebug() <<  m_name + m_soci + m_birthday.toString() + m_dni + m_phoneNumber1 + m_phoneNumber2 + m_address + m_city + m_email;
    } //end if

}
