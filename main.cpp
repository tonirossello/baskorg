#include <iostream>
#include <string>
#include "ixwebsocket/IXWebSocketServer.h"
#include "ixwebsocket/IXWebSocket.h"
#include "ixwebsocket/IXConnectionState.h"
#include <json.hpp>
#include <QString>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include "jugador.h"
#include "club.h"
#include "usuari.h"
#include <QSqlRecord>

/*! \file */

//probamos cambios
using JSON = nlohmann::json;
bool g_logueado;
int g_id_usuario;

bool exists(const JSON& json, const std::string& key)
{
    return json.find(key) != json.end();
}

/**
 * Función para loguearse en la aplicación
 */
void login(ix::WebSocket *webSocket, JSON received){

    std::string user;
    std::string pass;

    received["user"].get_to(user);
    received["pass"].get_to(pass);

    //qDebug() << "estoy en login" << user.c_str();
    //qDebug() << "PASS" << pass.c_str();

    ///consulta para buscar el usuario solicitado en la BBDD
    QSqlQuery q;
    q.prepare("SELECT * FROM usuaris WHERE usuari = :user");
    q.bindValue(":user",  user.c_str());
    q.exec();


    JSON respuesta;
    respuesta["type"] = "login";


    ///comprobamos si el usuario existe
    if (q.size() > 0){

        ///consulta para encryptar la contraseña
        QSqlQuery q2;
        q2.prepare("SELECT ENCRYPT(:pass, :uuid)");
        q2.bindValue(":pass",  pass.c_str());
        if (q.next()){          
            g_id_usuario = q.value("id").toInt();
            respuesta["id_user"] = q.value("id").toString().toStdString();
            qDebug() << "Existe el usuario, id = " << g_id_usuario;
            q2.bindValue(":uuid",  q.value("uuid"));
            q2.exec();
        }

        std::string encryptedpass;

        if (q2.first()){
            encryptedpass = q2.value(0).toString().toStdString();
        } //end if

        ///comprobamos si las contraseñas coinciden
        if (encryptedpass.compare(q.value("pass").toString().toStdString()) == 0){
            respuesta["operationSuccess"] = "true";
            g_logueado = true;
        } else {
            respuesta["operationSuccess"] = "false";
            respuesta["errorMessage"] = "La contrasenya no és correcta";
        } //end if

    } else {
        qDebug() << "No existe el usuario";
        respuesta["operationSuccess"] = "false";
        respuesta["errorMessage"] = "No existeix l'usuari";
    } //end if

    ///si nos hemos logueado, comprobamos si el usuario dispone de un club creado
    if (g_logueado == true){
        QSqlQuery q3;
        q3.prepare("SELECT * from clubs where propietari = :user ");
        q3.bindValue(":user",  g_id_usuario);
        q3.exec();

        ///si el usuario tiene algun club
        if (q3.size() > 0){
            respuesta["has_club"] = "true";
            JSON element;
            while (q3.next())
            {
                //respuesta["club_id"] = q3.value("id").toInt();
                element["id_club"] = q3.value("id").toInt();
                element["nom_club"] = q3.value("nom").toString().toStdString();
                element["codi_club"] = q3.value("codi").toString().toStdString();
                element["color_club"] = q3.value("color").toString().toStdString();

                respuesta["payload"].push_back(element);
            }

        } else {
            respuesta["has_club"] = "false";
        } //end if

    }

    std::string messageToSend = respuesta.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void clubCreate(ix::WebSocket *webSocket, JSON received){


    std::string club_name;
    std::string id_user;
    std::string club_color;


    received["user"].get_to(id_user);
    received["club_name"].get_to(club_name);
    received["club_color"].get_to(club_color);

    ///TO DO crear el club en la BBDD
    QSqlQuery q;
    q.prepare("INSERT into clubs (nom, propietari,color) values (:nom_club, :id_propietari, :color_club)");
    q.bindValue(":nom_club",  club_name.c_str());
    q.bindValue(":id_propietari",  id_user.c_str());
    q.bindValue(":color_club",  club_color.c_str());

    JSON respuesta;
    respuesta["type"] = "clubCreate";

    if (q.exec()){
         respuesta["operationSuccess"] = "true";
    } else {
        respuesta["operationSuccess"] = "false";
    } //end if

    std::string messageToSend = respuesta.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void playerCreate(ix::WebSocket *webSocket, JSON received){

    std::string player_name;
    std::string player_dni;
    std::string player_soci;
    std::string player_club;

    received["player_name"].get_to(player_name);
    received["player_dni"].get_to(player_dni);
    received["player_soci"].get_to(player_soci);
    received["player_club"].get_to(player_club);

    jugador j;
    j.setName(QString::fromUtf8(player_name.c_str()));
    j.setDni(QString::fromUtf8(player_dni.c_str()));
    j.setSoci(QString::fromUtf8(player_soci.c_str()));
    j.setIdClub(std::stoi(player_club));

    j.save();


    ///TO DO crear el jugador en la BBDD
    JSON jsonMessage = {
           {"type", "playerCreate"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void playersList(ix::WebSocket *webSocket, JSON received){

    std::string id_club;

    received["id_club"].get_to(id_club);

    QSqlQuery q2;
    q2.prepare("SELECT nom, dni, soci FROM jugadors where idclub = :club");
    q2.bindValue(":club",  id_club.c_str());
    q2.exec();

    club c;
    c.load(std::stoi(id_club));
    qDebug() << "Nombre: " << c.getName();
    qDebug() << "Color: #" << c.getColor();

    JSON respuesta;

    respuesta["type"] = "playersList";
    respuesta["club_name"] = c.getName().toStdString();
    respuesta["club_color"] = c.getColor().toStdString();
    respuesta["total"] = q2.size();

    JSON element;

    while (q2.next())
    {       
        element["nom"] = q2.value("nom").toString().toStdString();
        element["dni"] = q2.value("dni").toString().toStdString();
        element["soci"] = q2.value("soci").toString().toStdString();

        respuesta["payload"].push_back(element);
    }

    if (q2.size() != -1) respuesta["operationSuccess"] = true;
    else respuesta["operationSuccess"] = false;

    qDebug() << respuesta.dump(4).c_str();

    std::string messageToSend = respuesta.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void clubsList(ix::WebSocket *webSocket, JSON received){

    std::string user;
    received["id_user"].get_to(user);
    JSON respuesta;
    respuesta["type"] = "clubsList";
    if (g_logueado == true){
        QSqlQuery q3;
        q3.prepare("SELECT * from clubs where propietari = :user ");
        q3.bindValue(":user",  user.c_str());
        q3.exec();
        respuesta["total"] = q3.size();

        ///si el usuario tiene algun club
        if (q3.size() > 0){
            respuesta["has_club"] = "true";
            JSON element;
            while (q3.next())
            {
                //respuesta["club_id"] = q3.value("id").toInt();
                element["id_club"] = q3.value("id").toInt();
                element["nom_club"] = q3.value("nom").toString().toStdString();
                element["codi_club"] = q3.value("codi").toString().toStdString();
                element["color_club"] = q3.value("color").toString().toStdString();
                respuesta["payload"].push_back(element);
                qDebug() << "while";
            }
        } else {
            respuesta["has_club"] = "false";
        } //end if
    }
    std::string messageToSend = respuesta.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void logout(ix::WebSocket *webSocket){

    ///TO DO comprobrar que user y password son correctos
    JSON jsonMessage = {
           {"type", "logout"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
    g_logueado = false;
}

void userCreate(ix::WebSocket *webSocket, JSON received){

    std::string user_name;
    std::string user_pass;

    received["usuari"].get_to(user_name);
    received["password"].get_to(user_pass);

    usuari u;
    u.setUser(QString::fromUtf8(user_name.c_str()));
    u.setPass(QString::fromUtf8(user_pass.c_str()));

    u.save();

    JSON jsonMessage = {
           {"type", "userCreate"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

int main()
{

    g_logueado = false;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("cbsacabaneta");
    db.setPort(3306);
    db.setUserName("pitu");
    db.setPassword("pitu");
    bool ok = db.open();

    qDebug() << ok <<db.lastError().text();

   if (ok)
    {

        QDate d {QDate(1999,12,20)};

        //QString fecha = d.toString(Qt::ISODate);

        /*QSqlRecord rec = q.record();
        while (q.next()){
            qDebug() << q.value("id") << q.value("Nom").toString() << q.value("Data_naixement").toString();
        }*/

        //qDebug() << "result: "<< result;

    } // end if */

    ix::WebSocketServer server(9990, "0.0.0.0");

    ix::SocketTLSOptions tlsOptions;

    tlsOptions.tls = true;
    tlsOptions.certFile = "./cert/localhost.crt";
    tlsOptions.keyFile = "./cert/localhost.key";
    tlsOptions.caFile = "NONE";

    if (tlsOptions.isValid()){
        std::cerr << "SSL Valid" << std::endl;
    }

    server.setTLSOptions(tlsOptions);


    server.setOnConnectionCallback(
        [&server](std::shared_ptr<ix::WebSocket> webSocket,
                  std::shared_ptr<ix::ConnectionState> connectionState)
        {
            webSocket->setOnMessageCallback(
                [webSocket, connectionState, &server](const ix::WebSocketMessagePtr msg)
                {
                    if (msg->type == ix::WebSocketMessageType::Open)
                    {
                        std::cout << "Nueva conexión" << std::endl;

                    }
                    else if (msg->type == ix::WebSocketMessageType::Close)
                    {
                        std::cout << "Desconexión" << std::endl;
                    }
                    else if (msg->type == ix::WebSocketMessageType::Message)
                    {

                        if (!msg->binary)
                        {
                            /// Text format
                            std::cout << "Mensaje recibido: " << msg->str << std::endl;
                        }

                       //// 1) Decodificar JSON

                        JSON receivedObject = JSON::parse(msg->str, nullptr, false); //cast a JSON

                       /// 2) Decidir acción en base al 'type'

                        if ((receivedObject.is_discarded())) { //comprobramos si el JSON es válido
                            std::cout << "Error: el json no es válido" << std::endl;
                        } else {
                            //comprobar si el JSON contiene el campo type
                            if (exists(receivedObject, "type")){
                                //std::cout << "Existe type -> Tipo:" << receivedObject["type"] << std::endl;
                                if (receivedObject["type"] == "login")
                                {
                                    if (!g_logueado) login(webSocket.get(), receivedObject);
                                    else std::cout << "Ya estás logueado" << std::endl;
                                }
                                else if (receivedObject["type"] == "clubCreate")
                                {
                                    if (g_logueado) clubCreate(webSocket.get(), receivedObject);
                                    else std::cout << "No estás logueado" << std::endl;
                                }
                                else if (receivedObject["type"] == "playerCreate")
                                {
                                    if (g_logueado) playerCreate(webSocket.get(), receivedObject);
                                    else std::cout << "No estás logueado" << std::endl;
                                }
                                else if (receivedObject["type"] == "playersList")
                                {
                                    if (g_logueado) {


                                        playersList(webSocket.get(), receivedObject);
                                    } else {
                                        std::cout << "No estás logueado" << std::endl;
                                    }
                                }
                                else if (receivedObject["type"] == "logout")
                                {
                                    if (g_logueado) logout(webSocket.get());
                                    else std::cout << "No estás logueado" << std::endl;
                                }
                                else if (receivedObject["type"] == "clubsList")
                                {
                                    if (g_logueado) clubsList(webSocket.get(), receivedObject);
                                    else std::cout << "No estás logueado" << std::endl;
                                }

                                else if (receivedObject["type"] == "userCreate")
                                {
                                    userCreate(webSocket.get(), receivedObject);
                                }

                             } else {
                                std::cout << "Error: JSON no contiene el campo type" << std::endl;
                             }
                        }

                        ///MENSAJES DEL CLIENTE AL SERVIDOR
                        //socket.send('{"type":"login", "user":"toni", "pass":"toni123"}');

                        //socket.send('{"type":"clubCreate","payload": {"name":"Bàsquet Sa Cabaneta","state":"Balearican Islands","country":"Spain","mainColor":"Red"}}');

                        //socket.send('{"type":"clubUpdate","payload": {"name":"Bàsquet Sa Cabaneta","state":"Balearican Islands","country":"Spain","mainColor":"Blue"}}');

                        //socket.send('{"type":"clubDelete","clubID":"DEFAULT"}');

                        /*socket.send('{"type":"playerCreate","payload": {"name":"NombreJugador","State":"Balearican Islands","country":"Spain","number":"42",
                        "teamId":"passedByDefault","birthday":"1/1/1999", "address":"olesa 4", "city":"Sa Cabaneta", "zip":"07141", "phoneNumber":"654321987"}}');*/

                        /*socket.send('{"type":"playerUpdate","payload": {"name":"NombreJugador","State":"Catalonia","country":"Spain","number":"42",
                                                "teamId":"passedByDefault","birthday":"1/1/1999", "address":"olesa 4", "city":"Sa Cabaneta", "zip":"07141", "phoneNumber":"654321987"}}');*/

                        //socket.send('{"type":"playerDelete","playerID":"DEFAULT"}');

                        //socket.send('{"type":"teamCreate","payload": {"name":"Infantil","genre":"Masculí","year":"2006","league":"preferent"}}');

                        //socket.send('{"type":"teamUpdate","payload": {"name":"Infantil","genre":"Masculí","year":"2005","league":"preferent"}}');

                        //socket.send('{"type":"teamDelete","teamID":"DEFAULT"}');

                        //socket.send('{"type":"playerDelete","playerID":"DEFAULT"}');

                        //socket.send('{"type":"playersList","club":"DEFAULT"}');

                        //socket.send('{"type":"logout", "idConexion":"DEFAULT"}');



                    }
                }
            );
        }
    );

    auto res = server.listen();
    if (!res.first)
    {
        // Error handling
        return 1;
    }

    server.start();

    server.wait();
    server.stop();

}



