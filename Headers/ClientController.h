//
// Created by ixico on 16.05.22.
//

#ifndef L_ANTIVIRUS_CLIENTCONTROLLER_H
#define L_ANTIVIRUS_CLIENTCONTROLLER_H


#include <vector>
#include "../Socket/ServerSocket.h"

class ClientController {
public:
    void initClientSession();

private:
    int run(std::vector<std::string> arguments);
    bool handshake();
    ServerSocket *socket;
    std::string calculateValidResponse(std::string password_hash, std::string communication_salt);
public:
    ClientController(ServerSocket *socket);
};


#endif //L_ANTIVIRUS_CLIENTCONTROLLER_H
