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

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::array;
using std::cerr;
using std::filesystem::path;


int main(int argc, char* argcv[]) {
    CLIController cli_controller;
    FileController file_controller;
    QuarantineController quarantine_controller;
    path app_path;
    if (geteuid()==0){
        app_path = "/root";
    } else {
        string user_name = getlogin();
        app_path = "/home/" +user_name+ "/L_Antivirus";
    }
    try {
        quarantine_controller.init(app_path/"quarantine");
        file_controller.init(app_path/"database");
    } catch (std::exception &e){
        cli_controller.printInitFailure();
        return EXIT_FAILURE;
    }
//    cli_controller.printHelp();
//    quarantine_controller.setPassword("pogchamp");
//    quarantine_controller.imposeQuarantine("/home/ixico/Desktop/dtest6");
//    quarantine_controller.saveQuarantineRecords();
    if (argc == 1) {
        cli_controller.printWelcomePage();
        return EXIT_SUCCESS;
    }

    if (argcv[1] == string("-scanf")){
        if (argc != 3){
            cli_controller.printScanfArgumentProblem();
            return EXIT_FAILURE;
        }
        path file_path = argcv[2];
        try {
            if (file_controller.isFileDangerous(file_path)) {
                cli_controller.printDangerous();
                cli_controller.printPasswordPrompt();
                string password;
                std::cin >> password;
                quarantine_controller.setPassword(password);
                quarantine_controller.imposeQuarantine(file_path);
                quarantine_controller.saveQuarantineRecords();
            } else {
                cli_controller.printSafe();
            }
            return EXIT_SUCCESS;
        } catch (std::exception &e) {
            cout << e.what() << endl;
            return EXIT_FAILURE;
        }
    }
    if (argcv[1] == string("-scand")){
        if (argc != 3){
            cli_controller.printScandArgumentProblem();
            return EXIT_FAILURE;
        }
        path directory_path = argcv[2];
        try {
            vector<path> dangerous;
            dangerous = file_controller.findDangerousFiles(directory_path);
            cli_controller.printDangerous(dangerous);
            cli_controller.printPasswordPrompt();
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
        return EXIT_SUCCESS;
    }

    else if (argcv[1] == string("-quarantine")){
        if (argc != 2){
            cli_controller.printQuarantineArgumentProblem();
            return EXIT_FAILURE;
        }
        cli_controller.printQuarantineRecords(quarantine_controller.getQuarantineRecords());
    }

    else if (argcv[1] == string("-restore")){
        if (argc != 3){
            cli_controller.printRestoreArgumentProblem();
            return EXIT_FAILURE;
        }
        string password;
        std::cin >> password;
        quarantine_controller.setPassword(password);
        try {
            quarantine_controller.removeQuarantine(argcv[2]) ? cli_controller.printRestoreSuccess() : cli_controller.printRestoreFailure();
        } catch (std::invalid_argument &e) {
            cout << e.what() << endl;
        }
        quarantine_controller.saveQuarantineRecords();
        return EXIT_SUCCESS;
    }

    else {
        cli_controller.printArgumentProblem();
        return EXIT_FAILURE;
    }
    return 0;
}
