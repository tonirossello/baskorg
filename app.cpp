#include "app.h"
using JSON = nlohmann::json;

bool g_logueado;
int g_id_usuario;

/**
 * @brief Getter de login
 * @param webSocket --> correspondiente conexión
 * @return valor de g_logueado
 */
bool app::getLogueado(){
    return g_logueado;
}

/**
 * @brief Comprobar si JSON contiene una clave
 * @param json --> correspondiente JSON
 * @param key --> clave a comprobar
 * @return booleano para saber si la clave está o no en el JSON
 */
bool app::exists(const JSON& json, const std::string& key)
{
    return json.find(key) != json.end();
}

/**
 * @brief Convierte un string en booleano
 * @param v --> string a convertir
 * @return bool --> booleano convertido
 */
bool app::string2bool (const std::string & v)
{
    return !v.empty () &&
        (strcasecmp (v.c_str (), "true") == 0 ||
         atoi (v.c_str ()) != 0);
}

/**
 * @brief Login en la aplicación
 * @param webSocket --> correspondiente conexión
 * @param received --> JSON recibido del cliente
 */
void app::login(ix::WebSocket *webSocket, JSON received){

    std::string user;
    std::string pass;
    QString error_message;

    received["user"].get_to(user);
    received["pass"].get_to(pass);

    //consulta para buscar el usuario solicitado en la BBDD
    QSqlQuery q;
    q.prepare("SELECT * FROM usuaris WHERE usuari = :user");
    q.bindValue(":user",  user.c_str());
    q.exec();


    JSON respuesta;
    respuesta["type"] = "login";


    //comprobamos si el usuario existe
    if (q.size() > 0){

        //consulta para encryptar la contraseña
        QSqlQuery q2;
        q2.prepare("SELECT ENCRYPT(:pass, :uuid)");
        q2.bindValue(":pass",  pass.c_str());
        if (q.next()){
            g_id_usuario = q.value("id").toInt();
            respuesta["id_user"] = q.value("id").toString().toStdString();
            q2.bindValue(":uuid",  q.value("uuid"));
            q2.exec();
        }

        std::string encryptedpass;

        if (q2.first()){
            encryptedpass = q2.value(0).toString().toStdString();
        } //end if

        //comprobamos si las contraseñas coinciden
        if (encryptedpass.compare(q.value("pass").toString().toStdString()) == 0){
            respuesta["operationSuccess"] = "true";
            g_logueado = true;
        } else {
            error_message = QObject::tr("La contrasenya no és correcta");
            respuesta["operationSuccess"] = "false";
            respuesta["errorMessage"] = error_message.toStdString();
        } //end if

    } else {
        error_message = QObject::tr("No existeix l'usuari");
        respuesta["operationSuccess"] = "false";
        respuesta["errorMessage"] = error_message.toStdString();
    } //end if

    std::string messageToSend = respuesta.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

/**
 * @brief Creamos un club en la BBDD
 * @param webSocket --> correspondiente conexión
 * @param received --> JSON recibido del cliente
 */
bool app::clubCreate(ix::WebSocket *webSocket, JSON received){

    std::string club_name;
    std::string id_user;
    std::string club_color;


    received["user"].get_to(id_user);
    received["club_name"].get_to(club_name);
    received["club_color"].get_to(club_color);

    club c(m_db);
    c.setName(club_name.c_str());
    c.setIdPropietario(std::stoi(id_user));
    c.setColor(club_color.c_str());

    JSON respuesta;
    respuesta["type"] = "clubCreate";

    if (c.save()){
         respuesta["operationSuccess"] = "true";
         respuesta["message"] = QObject::tr("Club creat correctament").toStdString();
    } else {
        respuesta["operationSuccess"] = "false";
        respuesta["message"] = QObject::tr("Hi hagut un problema creant el club").toStdString();
    } //end if

    std::string messageToSend = respuesta.dump(); //el dump lo convierte a JSON
    if (webSocket){
        webSocket->send(messageToSend); //envio el mensaje JSON al cliente
    }

    return string2bool(respuesta["operationSuccess"]);
}

/**
 * @brief Creamos un jugador en la BBDD
 * @param webSocket --> correspondiente conexión
 * @param received --> JSON recibido del cliente
 */
void app::playerCreate(ix::WebSocket *webSocket, JSON received){

    std::string player_name;
    std::string player_dni;
    std::string player_soci;
    std::string player_club;

    received["player_name"].get_to(player_name);
    received["player_dni"].get_to(player_dni);
    received["player_soci"].get_to(player_soci);
    received["player_club"].get_to(player_club);

    jugador j(m_db);
    j.setName(QString::fromUtf8(player_name.c_str()));
    j.setDni(QString::fromUtf8(player_dni.c_str()));
    j.setSoci(QString::fromUtf8(player_soci.c_str()));
    j.setIdClub(std::stoi(player_club));

    JSON respuesta;
    respuesta["type"] = "playerCreate";

    if (j.save()){
         respuesta["operationSuccess"] = "true";
    } else {
        respuesta["operationSuccess"] = "false";
    } //end if

    std::string messageToSend = respuesta.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

/**
 * @brief Respuesta JSON con los jugadores del club
 * @param webSocket --> correspondiente conexión
 * @param received --> JSON recibido del cliente
 */
void app::playersList(ix::WebSocket *webSocket, JSON received){

    std::string id_club;

    received["id_club"].get_to(id_club);

    QSqlQuery q2;
    q2.prepare("SELECT nom, dni, soci FROM jugadors where idclub = :club");
    q2.bindValue(":club",  id_club.c_str());
    q2.exec();

    club c(m_db);
    c.load(std::stoi(id_club));

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

    std::string messageToSend = respuesta.dump(); //el dump lo convierte a JSON
    webSocket->send(messageToSend); //envio el mensaje JSON al cliente
}

/**
 * @brief Respuesta JSON con los clubes del usuario
 * @param webSocket --> correspondiente conexión
 * @param received --> JSON recibido del cliente
 */
void app::clubsList(ix::WebSocket *webSocket, JSON received){

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

        //si el usuario tiene algun club
        if (q3.size() > 0){
            respuesta["has_club"] = "true";
            JSON element;
            while (q3.next())
            {
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

/**
 * @brief Logout del usuario
 * @param webSocket --> correspondiente conexión
 */
void app::logout(ix::WebSocket *webSocket){

    JSON jsonMessage = {
           {"type", "logout"},
           {"operationSuccess", true},
    };

    std::string messageToSend = jsonMessage.dump(); //el dump lo convierte a JSON
    if (webSocket)
    {
        webSocket->send(messageToSend); //envio el mensaje JSON al cliente
    } // end if
    g_logueado = false;
}

/**
 * @brief Creamos un usuario en la BBDD
 * @param webSocket --> correspondiente conexión
 * @param received --> JSON recibido del cliente
 * @return booleano que confirmar si la operación se ha podido realizar o no
 */
bool app::userCreate(ix::WebSocket *webSocket, JSON received){

    std::string user_name;
    std::string user_pass;

    received["usuari"].get_to(user_name);
    received["password"].get_to(user_pass);

    usuari u(m_db);
    u.setUser(QString::fromUtf8(user_name.c_str()));
    u.setPass(QString::fromUtf8(user_pass.c_str()));

    JSON respuesta;
    respuesta["type"] = "userCreate";

    //si la consulta se lleva a cabo con éxito
    if (u.save()){
         respuesta["operationSuccess"] = "true";
    } else {
        respuesta["operationSuccess"] = "false";
    } //end if

    std::string messageToSend = respuesta.dump(); //el dump lo convierte a JSON
    if (webSocket)
    {
        webSocket->send(messageToSend); //envio el mensaje JSON al cliente
    } // end if

    return string2bool(respuesta["operationSuccess"]);
}

app::app(QSqlDatabase &db, bool server_on) : m_db (&db)
{
    ix::WebSocketServer server(9990, "0.0.0.0");
    g_logueado = false;

    //certicados
    ix::SocketTLSOptions tlsOptions;

    /*tlsOptions.tls = true;
    tlsOptions.certFile = "./cert/localhost.crt";
    tlsOptions.keyFile = "./cert/localhost.key";
    tlsOptions.caFile = "NONE";

    if (tlsOptions.isValid()){
        std::cerr << "SSL Valid" << std::endl;
    }


    server.setTLSOptions(tlsOptions);*/

    if (server_on){
        server.setOnConnectionCallback(
            [&server, this](std::shared_ptr<ix::WebSocket> webSocket,
                      std::shared_ptr<ix::ConnectionState> connectionState)
            {
                webSocket->setOnMessageCallback(
                    [webSocket, connectionState, &server, this](const ix::WebSocketMessagePtr msg)
                    {
                        //si recibimos un mensaje de apertura de conexión
                        if (msg->type == ix::WebSocketMessageType::Open)
                        {
                            std::cout << "Nueva conexión" << std::endl;
                        }

                        //si recibimos un mensaje de cierre de conexión
                        else if (msg->type == ix::WebSocketMessageType::Close)
                        {
                            std::cout << "Desconexión" << std::endl;
                        }

                        //si recibimos otro tipo de mensaje
                        else if (msg->type == ix::WebSocketMessageType::Message)
                        {

                            if (!msg->binary)
                            {
                                // Text format
                                std::cout << "Mensaje recibido: " << msg->str << std::endl;
                            }

                           // 1) Decodificar JSON

                            JSON receivedObject = JSON::parse(msg->str, nullptr, false); //cast a JSON

                           // 2) Decidir acción en base al 'type'

                            if ((receivedObject.is_discarded())) { //comprobramos si el JSON es válido
                                std::cout << "Error: el json no es válido" << std::endl;
                            } else {

                                //comprobar si el JSON contiene el campo type
                                if (exists(receivedObject, "type")){

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
                                 } //end if

                            } //end if

                        } //end if
                    }
                );
            }
        );

        //iniciamos el servidor
        auto res = server.listen();
        /*if (!res.first)
        {
            // Error handling
            return 1;
        }*/



        server.start();
        server.wait();
        server.stop();


   } //end if server_on
}
