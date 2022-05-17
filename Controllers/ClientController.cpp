//
// Created by ixico on 16.05.22.
//

#include "../Headers/ClientController.h"

//
// Created by ixico on 12.05.22.
//

#include <iostream>
#include <vector>
#include <array>
#include "../Headers/FileController.h"
#include "../Headers/FileHasher.h"
#include <filesystem>
#include "../Headers/QuarantineController.h"
#include "../Headers/FileManager.h"
#include <unistd.h>
#include "../Headers/CLIController.h"
#include "../Socket/SocketException.h"
#include <csignal>
#include <algorithm>
#include <thread>
#include <cstring>

#define BUFSIZE 1024

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::array;
using std::cerr;
using std::filesystem::path;

void signalHandler(int signum) {
    cout << "Interrupt signal received." << endl;
    exit(EXIT_FAILURE);
}

void ClientController::initClientSession() {
    try {
        bool authenticated = ClientController::handshake();
        if (!authenticated) {
            *socket << "Wrong password!\n";
            delete (socket);
            return;
        }

        *socket << "Welcome to L_Antivirus\n";
        string command;
        *socket >> command;
        while (command != "-quit") {
            std::istringstream ss(command);
            string argument;

            vector<string> arguments;
            while (ss >> argument) {
                arguments.push_back(argument);
            }

            *socket << run(arguments);

            *socket >> command;
        }
        delete (socket);
        return;

    } catch (SocketException &e) {
        cout << e.description() << endl;
        return;
    }
}

bool ClientController::handshake() {

    *socket >> user;
    cout << user << endl;
    user.erase(std::remove(user.begin(), user.end(), '\n'),
               user.end());//remove newline character at the end
    string hash;
    try {
        hash = findInShadowFile(user);
    } catch (std::invalid_argument &e) {
        return false;
    }
    password_hash = hash;
    std::stringstream ss(hash);
    string salt;
    std::getline(ss, salt, '$');
    std::getline(ss, salt, '$');
    std::getline(ss, salt, '$');
    string communication_salt = "asdasdasdasdasda"; // TODO: random string
    *socket << salt << "\n";
    *socket << communication_salt << "\n";

    string valid_response = calculateValidResponse(hash, communication_salt);
    string client_response;
    *socket >> client_response;
    client_response.erase(std::remove(client_response.begin(), client_response.end(), '\n'),
                          client_response.end());//remove newline character at the end
    cout << client_response << endl;
    return valid_response == client_response;
}


string ClientController::run(std::vector<std::string> arguments) {
    signal(SIGINT, signalHandler);
    FileController file_controller(user);
    QuarantineController quarantine_controller;
    path app_path = "/root/L_Antivirus";
    try {
        quarantine_controller.init(app_path / "quarantine");
        file_controller.init(app_path / "database");
    } catch (std::exception &e) {
        return CLIController::internalProblem();
    }

    if (arguments.size() == 0) {
        return CLIController::printManVisit();
    }

    if (arguments.at(0) == string("-scanf")) {
        if (arguments.size() != 2) {
            return CLIController::printManVisit();
        }
        path file_path = arguments.at(1);
        if (!std::filesystem::exists(file_path)) return CLIController::notFound();
        try {
            if (file_controller.isFileDangerous(file_path)) {
                string password = password_hash;
                quarantine_controller.setPassword(password);
                quarantine_controller.imposeQuarantine(file_path);
                quarantine_controller.saveQuarantineRecords();
                return CLIController::printImposeSuccess();
            } else {
                return CLIController::printSafe();
            }
        } catch (std::exception &e) {
            cout << e.what() << endl;
            return CLIController::wrongType();
        }
    }
    if (arguments.at(0) == string("-scand")) {
        if (arguments.size() != 2) {
            return CLIController::printManVisit();
        }
        path directory_path = arguments.at(1);
        if (!std::filesystem::exists(directory_path)) return CLIController::notFound();
        if (!hasPermissions(directory_path, user)) return CLIController::permissionDenied();
        try {
            vector<path> dangerous;
            dangerous = file_controller.findDangerousFiles(directory_path);
            if (dangerous.empty()) return CLIController::printSafe();
            string password = password_hash;
            quarantine_controller.setPassword(password);
            for (const auto &item: dangerous) quarantine_controller.imposeQuarantine(item);
        } catch (std::exception &e) {
            cout << e.what() << endl;
            return CLIController::wrongType();
        }
        quarantine_controller.saveQuarantineRecords();
        return CLIController::printImposeSuccess();
    } else if (arguments.at(0) == string("-quarantine")) {
        if (arguments.size() != 1) {
            return CLIController::printManVisit();
        }
        return CLIController::printQuarantineRecords(
                quarantine_controller.getQuarantineRecords());
    } else if (arguments.at(0) == string("-restore")) {
        if (arguments.size() != 2) {
            return CLIController::printManVisit();
        }
        string password = password_hash;
        quarantine_controller.setPassword(password);
        try {
            bool success = quarantine_controller.removeQuarantine(arguments.at(1));
            quarantine_controller.saveQuarantineRecords();
            return success ? CLIController::printRestoreSuccess() : CLIController::printRestoreFailure();
        } catch (std::invalid_argument &e) {
            cout << e.what() << endl;
            return CLIController::internalProblem();
        }
    } else if (arguments.at(0) == string("-help")) {
        if (arguments.size() != 1) {
            return CLIController::printManVisit();
        }
        return CLIController::printHelp();
    } else {
        return CLIController::printManVisit();
    }
}

ClientController::ClientController(ServerSocket *socket) : socket(socket) {}

std::string ClientController::calculateValidResponse(std::string password_hash, std::string communication_salt) {
    char encrypted[BUFSIZE]; //For storing results

    string blank = "";
    char *key = &password_hash[0];
    char *salt_origin = &communication_salt[0];
    char *salt = &blank[0];

    sprintf(salt, "$6$%s", salt_origin); //Salt shaping, id specification (described later)
    strcpy(encrypted, crypt(key, salt)); //The encrypted variable

    return encrypted;
}
