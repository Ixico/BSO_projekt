//
// Created by ixico on 15.04.2022.
//

#include "../Headers/CLIController.h"
#include <iostream>
using std::cout;
using std::endl;

void CLIController::printWelcomePage() {
    cout << "Welcome in Linux Antivirus" << endl;
    cout << "Use -help flag for usage details:" << endl;
    cout << "./Antivirus -help" << endl;
}

void CLIController::printHelp() {
    cout << "Linux Antivirus help page" << endl;
    cout << "USAGE" << endl;
    cout << "./Antivirus -flag [argument]" << endl;
    cout << "FLAGS" << endl;
    cout << "-scanf [file_path]" << " -> scans specified file" << endl;
    cout << "-scand [directory_path]" << " -> scans specified directory recursively" << endl;
    cout << "-quarantine" << " -> shows files on quarantine" <<  endl;
    cout << "-restore [file_name]" << " -> restores file from quarantine" << endl;
    cout << endl;
    cout << "After all commands (except -quarantine) you will be asked about password." << endl;
    cout << "Password is used to encrypt and decrypt file. You may use various password, but once you encrypt file with one, it can be only decrypted with it." <<endl;
    cout << "Warning: L_Antivirus scans only file(s) if you have sufficient privileges!" << endl;
    cout << "Files with no read permission will be omitted" << endl;

}

void CLIController::printDangerous() {
    cout << "This file my be dangerous!" << endl;
}

void CLIController::printSafe() {
    cout << "File is considered to be safe" << endl;
}

void CLIController::printInitFailure() {
    cout << "Initialization fatal error!" << endl;
    cout << "Check if directory /home/user/L_Antivirus contains database file!" << endl;
}

void CLIController::printDangerous(const std::vector<std::filesystem::path>& vector1) {
    if (vector1.empty()){
        cout << "No dangerous files found" << endl;
        return;
    }
    cout << "Found " << vector1.size() << " dangerous files:" << endl;
    for (const auto &item: vector1)
        cout << item << endl;
}

void CLIController::printScanfArgumentProblem() {
    cout << "Misuse of arguments" << endl;
    cout << "SYNTAX:" << endl;
    cout << "./L_Antivirus -scanf file_name" << endl;
}

void CLIController::printScandArgumentProblem() {
    cout << "Misuse of arguments" << endl;
    cout << "SYNTAX:" << endl;
    cout << "./L_Antivirus -scand directory_name" << endl;
}

void CLIController::printPasswordPrompt() {
    cout << "Put password to secure file in your quarantine." << endl;
    cout << "You will need to use it again if you want to restore your files." << endl;
    cout << "Password:" << endl;
}

void CLIController::printQuarantineRecords(const std::vector<QuarantineRecord> &vector) {
    cout << "Files on quarantine" << endl;
    cout << "relative path | file name" << endl;
    cout << endl;
    for (const auto &item: vector) {
        cout << item.file_path << " | " << item.file_path.stem() << endl;
    }
}

void CLIController::printRestoreArgumentProblem() {
    cout << "Misuse of arguments" << endl;
    cout << "SYNTAX:" << endl;
    cout << "./L_Antivirus -restore file_name" << endl;
}

void CLIController::printQuarantineArgumentProblem() {
    cout << "Misuse of arguments" << endl;
    cout << "SYNTAX:" << endl;
    cout << "./L_Antivirus -quarantine" << endl;
}

void CLIController::printRestoreFailure() {
    cout << "Wrong password! Could not restore file form quarantine." << endl;
}

void CLIController::printRestoreSuccess() {
    cout << "File successfully restored from quarantine!" << endl;
}

void CLIController::printArgumentProblem() {
    cout << "Invalid argument" << endl;
    cout << "Look at the help page for more details:" << endl;
    cout << "./L_Antivirus -help" << endl;
}

void CLIController::printHelpArgumentProblem() {
    cout << "Misuse of arguments" << endl;
    cout << "SYNTAX:" << endl;
    cout << "./L_Antivirus -help" << endl;
}

void CLIController::printImposeSuccess() {
    cout << "File(s) successfully imposed on quarantine." << endl;
}

void CLIController::printPasswordPromptRestore() {
    cout << "Put password to restore file from your quarantine." << endl;
    cout << "You have to use exactly the same you've used when imposing on quarantine" << endl;
    cout << "Password:" << endl;
}
