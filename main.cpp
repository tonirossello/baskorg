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

    qDebug() << "1";
    std::string club_name;
    std::string id_user;
    std::string club_color;

    qDebug() << "2";
    received["user"].get_to(id_user);
    received["club_name"].get_to(club_name);
    received["club_color"].get_to(club_color);
    qDebug() << "3";
    ///TO DO crear el club en la BBDD
    QSqlQuery q;
    q.prepare("INSERT into clubs (nom, propietari,color) values (:nom_club, :id_propietari, :color_club)");
    q.bindValue(":nom_club",  club_name.c_str());
    q.bindValue(":id_propietari",  id_user.c_str());
    q.bindValue(":color_club",  club_color.c_str());
    q.exec();
    qDebug() << "4";

    JSON respuesta;
    respuesta["type"] = "clubCreate";

    if (q.next()){
        qDebug() << "5";
        qDebug() << "Creando club";
        JSON element;
        element["id_club"] = q.value("id").toInt();
        element["nom_club"] = q.value("nom").toString().toStdString();
        element["codi_club"] = q.value("codi").toString().toStdString();
        element["color_club"] = q.value("color").toString().toStdString();

        respuesta["payload"].push_back(element);
    }
    qDebug() << "6";

    std::string messageToSend = respuesta.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void clubUpdate(ix::WebSocket *webSocket){

    ///TO DO actualizar el club en la BBDD
    JSON jsonMessage = {
           {"type", "clubUpdate"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void clubDelete(ix::WebSocket *webSocket){

    ///TO DO eliminar el club en la BBDD
    JSON jsonMessage = {
           {"type", "clubDelete"},
           {"clubName","clubName"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void playerCreate(ix::WebSocket *webSocket){

    ///TO DO crear el jugador en la BBDD
    JSON jsonMessage = {
           {"type", "playerCreate"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void playerUpdate(ix::WebSocket *webSocket){

    ///TO DO actualizar el jugador en la BBDD
    JSON jsonMessage = {
           {"type", "playerUpdate"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void playerDelete(ix::WebSocket *webSocket){

    ///TO DO eliminar el jugador en la BBDD
    JSON jsonMessage = {
           {"type", "playerDelete"},
           {"playerName","playerName"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void teamCreate(ix::WebSocket *webSocket){

    ///TO DO crear el equipo en la BBDD
    JSON jsonMessage = {
           {"type", "teamCreate"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void teamUpdate(ix::WebSocket *webSocket){

    ///TO DO actualizar el equipo en la BBDD
    JSON jsonMessage = {
           {"type", "teamUpdate"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void teamDelete(ix::WebSocket *webSocket){

    ///TO DO eliminar el equipo en la BBDD
    JSON jsonMessage = {
           {"type", "teamDelete"},
           {"teamName","teamName"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

void playersList(ix::WebSocket *webSocket, JSON received){

    std::string club;

    received["id_club"].get_to(club);


    QSqlQuery q2;
    q2.prepare("SELECT nom, dni, soci FROM jugadors where idclub = :club");
    q2.bindValue(":club",  club.c_str());
    q2.exec();


    JSON respuesta;

    respuesta["type"] = "playersList";
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

        jugador j;
        QDate d {QDate(1999,12,20)};

        //QString fecha = d.toString(Qt::ISODate);

        j.load(237);
        //j.setName("PROVA");
        //j.setName("Jordi Rosselló");
        //j.setCity("Palma");
        //j.setNumber(4);
        //j.setTeamId(0);
        //j.setAddress("Calle");
        j.setBirthday(d);
        //j.setPhoneNumber1("666999333");
        j.save();

        //j.save();
        /*QSqlQuery q;
        q.prepare("SELECT * from jugadors");
        q.exec();
        qDebug() << q.size();*/
        //q.prepare("INSERT INTO jugadors (Dorsal, Data_naixement) VALUES (:dorsal,:fecha)");
         //q.bindValue(":dorsal", 14);
        //q.bindValue(":fecha", fecha);
       // bool result {q.exec()};

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
                                else if (receivedObject["type"] == "clubUpdate")
                                {
                                    if (g_logueado) clubUpdate(webSocket.get());
                                    else std::cout << "No estás logueado" << std::endl;
                                }
                                else if (receivedObject["type"] == "clubDelete")
                                {
                                    if (g_logueado) clubDelete(webSocket.get());
                                    else std::cout << "No estás logueado" << std::endl;
                                }
                                else if (receivedObject["type"] == "playerCreate")
                                {
                                    if (g_logueado) playerCreate(webSocket.get());
                                    else std::cout << "No estás logueado" << std::endl;
                                }
                                else if (receivedObject["type"] == "playerUpdate")
                                {
                                    if (g_logueado) playerUpdate(webSocket.get());
                                    else std::cout << "No estás logueado" << std::endl;
                                }
                                else if (receivedObject["type"] == "playerDelete")
                                {
                                    if (g_logueado) playerDelete(webSocket.get());
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
                                else if (receivedObject["type"] == "teamCreate")
                                {
                                    if (g_logueado) teamCreate(webSocket.get());
                                    else std::cout << "No estás logueado" << std::endl;
                                }
                                else if (receivedObject["type"] == "teamUpdate")
                                {
                                    if (g_logueado) teamUpdate(webSocket.get());
                                    else std::cout << "No estás logueado" << std::endl;
                                }
                                else if (receivedObject["type"] == "teamDelete")
                                {
                                    if (g_logueado) teamDelete(webSocket.get());
                                    else std::cout << "No estás logueado" << std::endl;
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



