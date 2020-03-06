#include "jugador.h"
#include <QDebug>
#include <QSqlError>

/*! \file */

jugador::jugador(QSqlDatabase *db) : m_db(db)
{
    qDebug() << "Jugador " << m_db->database();
}

//GETTERS
/**
 * @brief Obtener el nombre del jugador
 * @return Nombre del jugador
 */
QString jugador::getName(){
    return m_name;
}

/**
 * @brief Obtener el dorsal del jugador
 * @return Dorsal del jugador
 */
int jugador::getNumber(){
    return m_number;
}

/**
 * @brief Obtener el id del equipo del jugador
 * @return Id del equipo del jugador
 */
int jugador::getTeamId(){
    return m_idTeam;
}

/**
 * @brief Obtener el día de nacimiento del jugador
 * @return Día de nacimiento del jugador
 */
QDate jugador::getBirthday(){
    return m_birthday;
}

/**
 * @brief Obtener la dirección del jugador
 * @return Dirección del jugador
 */
QString jugador::getAddress(){
    return m_address;
}

/**
 * @brief Obtener la ciudad del jugador
 * @return Ciudad del jugador
 */
QString jugador::getCity(){
    return m_city;
}

/**
 * @brief Obtener el móvil del jugador
 * @return Móvil del jugador
 */
QString jugador::getPhoneNumber1(){
    return m_phoneNumber1;
}

/**
 * @brief Obtener el móvil 2 del jugador
 * @return Móvil 2 del jugador
 */
QString jugador::getPhoneNumber2(){
    return m_phoneNumber2;
}

/**
 * @brief Obtener el nombre del jugador
 * @return Nombre del jugador
 */
QString jugador::getSoci(){
    return m_soci;
}

/**
 * @brief Obtener el email del jugador
 * @return Email del jugador
 */
QString jugador::getEmail(){
    return m_email;
}

/**
 * @brief Obtener el dni del jugador
 * @return Dni del jugador
 */
QString jugador::getDni(){
    return m_dni;
}

/**
 * @brief Obtener el id del género del jugador
 * @return Género del jugador
 */
int jugador::getIdGen(){
    return m_idGen;
}

/**
 * @brief Obtener el club del jugador
 * @return Club del jugador
 */
int jugador::getIdClub(){
    return m_idClub;
}

/**
 * @brief Obtener el id del jugador
 * @return Id del jugador
 */
int jugador::getId(){
    return m_idJugador;
}


//SETTERS

/**
 * @brief Modificar el nombre del jugador
 * @param name --> Nuevo nombre del jugador
 */
void jugador::setName(QString name){
    m_name = name;
}

/**
 * @brief Modificar el dorsal del jugador
 * @param number --> Nuevo dorsal del jugador
 */
void jugador::setNumber(int number){
    m_number = number;
}

/**
 * @brief Modificar el equipo del jugador
 * @param teamId --> Nuevo equipo del jugador
 */
void jugador::setTeamId(int teamId){
    m_idTeam = teamId;
}

/**
 * @brief Modificar la fecha de nacimiento del jugador
 * @param birthday --> Nueva fecha de nacimiento del jugador
 */
void jugador::setBirthday(QDate birthday){
    m_birthday = birthday;
}

/**
 * @brief Modificar la dirección del jugador
 * @param address --> Nueva dirección del jugador
 */
void jugador::setAddress(QString address){
    m_address = address;
}

/**
 * @brief Modificar la ciudad del jugador
 * @param city --> Nueva ciudad de nacimiento del jugador
 */
void jugador::setCity(QString city){
    m_city = city;
}

/**
 * @brief Modificar el móvil del jugador
 * @param phoneNumber --> Nueva móvil del jugador
 */
void jugador::setPhoneNumber1(QString phoneNumber){
    m_phoneNumber1 = phoneNumber;
}

/**
 * @brief Modificar el móvil 2 del jugador
 * @param phoneNumber --> Nueva móvil 2 del jugador
 */
void jugador::setPhoneNumber2(QString phoneNumber){
    m_phoneNumber2 = phoneNumber;
}

/**
 * @brief Modificar el socio del jugador
 * @param soci --> Nuevo soci del jugador
 */
void jugador::setSoci(QString soci){
    m_soci = soci;
}

/**
 * @brief Modificar el email del jugador
 * @param email --> Nuevo email del jugador
 */
void jugador::setEmail(QString email){
    m_email = email;
}

/**
 * @brief Modificar el dni del jugador
 * @param dni --> Nuevo dni del jugador
 */
void jugador::setDni(QString dni){
    m_dni = dni;
}

/**
 * @brief Modificar el género del jugador
 * @param idGen --> Nuevo género del jugador
 */
void jugador::setIdGen(int idGen){
    m_idGen = idGen;
}

/**
 * @brief Modificar el club del jugador
 * @param idClub --> Nuevo club del jugador
 */
void jugador::setIdClub(int idClub){
    m_idClub = idClub;
}


//Operaciones de la BBDD

/**
 * @brief Actualiza o inserta un nuevo jugador a la BBDD
 * @return Si se ha realizado la consulta sin errores
 */
bool jugador::save(){

    QSqlQuery q(*m_db);

    if (m_idJugador > 0)
    {

        //UPDATE
        q.prepare("UPDATE jugadors SET dorsal = :dorsal, naixement = :data, nom = :name, soci = :soci, dni = :dni, tfn1 = :tfn1, tfn2 = :tfn2, adresa = :adresa, poblacio = :poblacio, email = :email, idclub = :club WHERE id = :idjugador");
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
        q.bindValue(":club", m_idClub);

    }
    else
    {

        //INSERT
        q.prepare("INSERT INTO jugadors (dorsal,naixement, nom, soci, dni, tfn1, tfn2,adresa, poblacio, email, idclub) VALUES (:dorsal, :data, :name, :soci, :dni, :tfn1, :tfn2, :adresa, :poblacio, :email, :club)");
        //q.prepare("INSERT INTO jugadors (nom, soci, dni, idclub) VALUES (:name, :soci, :dni, :club)");
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
        q.bindValue(":club", m_idClub);
    } //end if

    bool result {q.exec()};

    qDebug() << result;
    qDebug() << q.lastError();

    return result;
}

/**
 * @brief Carga los datos de un jugador en el objeto jugador
 * @param id --> id del jugador a cargar
 */
void jugador::load(int id){

    QSqlQuery q(*m_db);

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
        m_idClub = q.value("idclub").toInt();

      qDebug() <<  m_name + m_soci + m_birthday.toString() + m_dni + m_phoneNumber1 + m_phoneNumber2 + m_address + m_city + m_email;
    } //end if

}

/**
 * @brief Borra los datos de un jugador en el objeto jugador
 * @param id --> id del jugador a borrar
 */
bool jugador::delete_from_bd(){

    QSqlQuery q(*m_db);

    q.prepare("DELETE FROM jugadors WHERE id=:idJugador;");
    q.bindValue(":idJugador", m_idJugador);
    bool result {q.exec()};
    qDebug() << q.lastError();

    return result;

}
