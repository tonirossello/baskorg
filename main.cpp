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

//probamos cambios
using JSON = nlohmann::json;
bool g_logueado;

bool exists(const JSON& json, const std::string& key)
{
    return json.find(key) != json.end();
}

void login(ix::WebSocket *webSocket){

    ///TO DO comprobrar que user y password son correctos
    JSON jsonMessage = {
           {"type", "login"},
           {"operationSuccess", true},
           {"clubDefault", "Bàsquet Sa Cabaneta"}
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
    g_logueado = true;
}

void clubCreate(ix::WebSocket *webSocket){

    ///TO DO crear el club en la BBDD
    JSON jsonMessage = {
           {"type", "clubCreate"},
           {"operationSuccess", true},
           {"clubCode", "3J8Z7T"},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
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

void playersList(ix::WebSocket *webSocket){

    ///TO DO obtener todos los jugadores de la BBDD
    JSON jsonMessage = {
           {"type", "playersList"},
           {"operationSuccess", true},
           {"total", 3},
           {"payload", {
             {{"Name", "Name1"},
              {"Team", "Team1"},
              {"Number", "3"}},

             {{"Name", "Name2"},
              {"Team", "Team2"},
              {"Number", "40"}},

             {{"Name", "Name3"},
              {"Team", "Team1"},
              {"Number", "60"}},
           }}
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
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


/*QString encrypt(QByteArray msg, QByteArray const& key){

    if(!key.size())
        return msg;


    for (QString::size_type i = 0; i < msg.size(); ++i)
          msg[i] ^ key[i%key.size()];
    return msg;
}

QString decrypt(QByteArray const& msg, QByteArray const& key){
    return encrypt(msg, key);
}*/

int main()
{
    /*QByteArray key = "clave";
    QByteArray password[20];
    QByteArray passEncriptada[20];
    std::cout << "Introduce tu contraseña: " << std::endl;
    std::cin >> password;
    std::cout << "Tu contraseña es: " <<  password << std::endl;
    passEncriptada = encrypt(password, key);
    std::cout << "Tu contraseña encriptada es: " <<  passEncriptada << std::endl;*/

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

        QString fecha = d.toString(Qt::ISODate);


        //j.setName("Jordi Rosselló");
        //j.setZip(07141);
        //j.setCity("Palma");
        //j.setState("Balears");
        j.setNumber(4);
        //j.setTeamId(0);
        //j.setAddress("Calle");
        //j.setCountry("Spain");
        j.setBirthday(d);
        //j.setPhoneNumber("666999333");
        j.save();
        /*QSqlQuery q;

        q.prepare("INSERT INTO jugadors (Dorsal, Data_naixement) VALUES (:dorsal,:fecha)");
        q.bindValue(":dorsal", 14);
        q.bindValue(":fecha", fecha);
        bool result {q.exec()};*/

        /*QSqlRecord rec = q.record();
        while (q.next()){
            qDebug() << q.value("id") << q.value("Nom").toString() << q.value("Data_naixement").toString();
        }*/



        //qDebug() << "result: "<< result;

    } // end if */

    ix::WebSocketServer server(9990, "0.0.0.0");

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
                                    if (!g_logueado) login(webSocket.get());
                                    else std::cout << "Ya estás logueado" << std::endl;
                                }
                                else if (receivedObject["type"] == "clubCreate")
                                {
                                    if (g_logueado) clubCreate(webSocket.get());
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
                                    if (g_logueado) playersList(webSocket.get());
                                    else std::cout << "No estás logueado" << std::endl;
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



