//
// Created by ixico on 15.04.2022.
//

#include "../Headers/CLIController.h"
#include <iostream>
using std::cout;
using std::endl;

std::string CLIController::printWelcomePage() {
    return "Visit man page for usage details.\n";
}


std::string CLIController::printDangerous() {
    return "This file my be dangerous!\n";
}

std::string CLIController::printSafe() {
    return "File(s) considered to be safe\n";
}

std::string CLIController::printInitFailure() {
    return "Initialization fatal error! Contact server administrator.\n";
}

std::string CLIController::printDangerous(const std::vector<std::filesystem::path>& vector1) {
    if (vector1.empty()){
        return "No dangerous files found\n";
    }
    return "Found dangerous files\n";
}


std::string CLIController::printPasswordPrompt() {
    cout << "Put password to secure file in your quarantine." << endl;
    cout << "You will need to use it again if you want to restore your files." << endl;
    cout << "If you do not want to impose quarantine on file, use CTRL+C to leave" << endl;
    cout << "Password:" << endl;
    return "";
}

std::string CLIController::printQuarantineRecords(const std::vector<QuarantineRecord> &vector) {
    std::string response;
    if (vector.empty()) return "No quarantine records.\n";
    for (const auto &item: vector) {
        response.append(item.file_path.stem());
        response.append(" | ");
    }
    return response+"\n";
}



std::string CLIController::printRestoreFailure() {
    return "Wrong password! Could not restore file form quarantine.\n";
}

std::string CLIController::printRestoreSuccess() {
    return "File successfully restored from quarantine!\n";
}


std::string CLIController::printImposeSuccess() {
    return "Found some dangerous files and imposed them on quarantine.\n";
}


std::string CLIController::unallowedDirectoryScan() {
    return "Cannot scan this directory.\n";
}

std::string CLIController::printManVisit() {
    return "Invalid command! Visit man page for details of usage.\n";
}

std::string CLIController::internalProblem() {
    return "Internal fatal error! Contact server administrator.\n";
}

std::string CLIController::permissionDenied() {
    return "Permission denied! You can only work with files within your home/user directory.\n";
}

std::string CLIController::printHelp() {
    return "Visit L_Antivirus page ... \n";
}

std::string CLIController::notFound() {
    return "File/directory nor found!\n";
}

std::string CLIController::wrongType() {
    return "Path specifies wrong type of file.\n";
}
