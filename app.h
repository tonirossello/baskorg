#ifndef APP_H
#define APP_H

#include "usuari.h"
#include "jugador.h"
#include "club.h"
#include <json.hpp>
#include "ixwebsocket/IXWebSocketServer.h"
#include "ixwebsocket/IXWebSocket.h"
#include "ixwebsocket/IXConnectionState.h"
#include <QString>
#include <iostream>
#include <QDebug>

class app
{
    using JSON = nlohmann::json;
    QSqlDatabase *m_db;

public:

    app(QSqlDatabase &db, bool server_on = true);
    void login(ix::WebSocket *webSocket, JSON received);
    bool userCreate(ix::WebSocket *webSocket, JSON received);
    bool clubCreate(ix::WebSocket *webSocket, JSON received);
    void playerCreate(ix::WebSocket *webSocket, JSON received);
    void playersList(ix::WebSocket *webSocket, JSON received);
    void clubsList(ix::WebSocket *webSocket, JSON received);
    void logout(ix::WebSocket *webSocket);
    bool exists(const JSON& json, const std::string& key);
    bool getLogueado();
    bool string2bool (const std::string & v);
    void checkLoginStatus(ix::WebSocket *webSocket);
    void playerDelete(ix::WebSocket *webSocket, JSON received);




};

#endif // APP_H
