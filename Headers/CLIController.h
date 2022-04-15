//
// Created by ixico on 15.04.2022.
//

#ifndef ANTIVIRUS_CLICONTROLLER_H
#define ANTIVIRUS_CLICONTROLLER_H


#include <vector>
#include <filesystem>
#include "QuarantineRecord.h"

class CLIController {
public:
    void printWelcomePage();
    void printHelp();

    void printDangerous();

    void printSafe();

    void printInitFailure();

    void printDangerous(std::vector<std::filesystem::path> vector1);

    void printScanfArgumentProblem();

    void printScandArgumentProblem();

    void printPasswordPrompt();

    void printQuarantineRecords(const std::vector<QuarantineRecord> &vector);

    void printQuarantineArgumentProblem();

    void printRestoreArgumentProblem();

    void printRestoreSuccess();

    void printRestoreFailure();

    void printArgumentProblem();
};


#endif //ANTIVIRUS_CLICONTROLLER_H
