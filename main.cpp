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

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::array;
using std::cerr;
using std::filesystem::path;

void signalHandler( int signum ) {
    cout << "Interrupt signal received." << endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char* argcv[]) {
    signal(SIGINT, signalHandler);
    FileController file_controller;
    QuarantineController quarantine_controller;
    path app_path;
    if (geteuid()==0){
        app_path = "/root/L_Antivirus";
    } else {
        string user_name = getlogin();
        app_path = "/home/" +user_name+ "/L_Antivirus";
    }
    try {
        quarantine_controller.init(app_path/"quarantine");
        file_controller.init(app_path/"database");
    } catch (std::exception &e){
        CLIController::printInitFailure();
        return EXIT_FAILURE;
    }

    if (argc == 1) {
        CLIController::printWelcomePage();
        return EXIT_SUCCESS;
    }

    if (argcv[1] == string("-scanf")){
        if (argc != 3){
            CLIController::printScanfArgumentProblem();
            return EXIT_FAILURE;
        }
        path file_path = argcv[2];
        try {
            if (file_controller.isFileDangerous(file_path)) {
                CLIController::printDangerous();
                CLIController::printPasswordPrompt();
                string password;
                std::cin >> password;
                quarantine_controller.setPassword(password);
                quarantine_controller.imposeQuarantine(file_path);
                quarantine_controller.saveQuarantineRecords();
                CLIController::printImposeSuccess();
            } else {
                CLIController::printSafe();
            }
            return EXIT_SUCCESS;
        } catch (std::exception &e) {
            cout << e.what() << endl;
            return EXIT_FAILURE;
        }
    }
    if (argcv[1] == string("-scand")){
        if (argc != 3){
            CLIController::printScandArgumentProblem();
            return EXIT_FAILURE;
        }
        path directory_path = argcv[2];
        try {
            vector<path> dangerous;
            dangerous = file_controller.findDangerousFiles(directory_path);
            CLIController::printDangerous(dangerous);
            CLIController::printPasswordPrompt();
            string password;
            std::cin >> password;
            quarantine_controller.setPassword(password);
            for (const auto &item: dangerous) quarantine_controller.imposeQuarantine(item);
        } catch (std::exception &e) {
            cout << e.what() << endl;
            quarantine_controller.saveQuarantineRecords();
            return EXIT_FAILURE;
        }
        quarantine_controller.saveQuarantineRecords();
        CLIController::printImposeSuccess();
        return EXIT_SUCCESS;
    }

    else if (argcv[1] == string("-quarantine")){
        if (argc != 2){
            CLIController::printQuarantineArgumentProblem();
            return EXIT_FAILURE;
        }
        CLIController::printQuarantineRecords(quarantine_controller.getQuarantineRecords());
        return EXIT_SUCCESS;
    }

    else if (argcv[1] == string("-restore")){
        if (argc != 3){
            CLIController::printRestoreArgumentProblem();
            return EXIT_FAILURE;
        }
        CLIController::printPasswordPromptRestore();
        string password;
        std::cin >> password;
        quarantine_controller.setPassword(password);
        try {
            quarantine_controller.removeQuarantine(argcv[2]) ? CLIController::printRestoreSuccess() : CLIController::printRestoreFailure();
        } catch (std::invalid_argument &e) {
            cout << e.what() << endl;
        }
        quarantine_controller.saveQuarantineRecords();
        return EXIT_SUCCESS;
    }

    else if (argcv[1] == string("-help")) {
        if (argc != 2){
            CLIController::printHelpArgumentProblem();
            return EXIT_FAILURE;
        }
        CLIController::printHelp();
        return EXIT_SUCCESS;
    }

    else {
        CLIController::printArgumentProblem();
        return EXIT_FAILURE;
    }
}
