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
    ClientController(const ServerSocket &socket);

private:
    int run(std::vector<std::string> arguments);
    bool handshake();
    ServerSocket socket;
};


#endif //L_ANTIVIRUS_CLIENTCONTROLLER_H
