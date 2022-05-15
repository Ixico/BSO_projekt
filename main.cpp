#include <iostream>
#include <vector>
#include <array>
#include "Headers/FileController.h"
#include "Headers/FileHasher.h"
#include <filesystem>
#include "Headers/QuarantineController.h"
#include "Headers/FileManager.h"
#include <unistd.h>
#include "Headers/CLIController.h"
#include <csignal>
#include <thread>
#include "Socket/ServerSocket.h"
#include "Socket/SocketException.h"
#include "Headers/ClientHandler.h"


using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::array;
using std::cerr;
using std::filesystem::path;


int main() {
    try {
        ServerSocket server(30000);
        while (true) {
            ServerSocket new_sock;
            server.accept(new_sock);

            string command;
            new_sock << command;

            std::istringstream ss(command);
            string argument;
            vector<string> arguments;
            while (ss >> argument)
            {
                arguments.push_back(argument);
            }
            std::thread(run, arguments).join();
        }
    }
    catch (SocketException &e) {
        std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
        return EXIT_FAILURE;
    }
}
