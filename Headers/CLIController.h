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
    static void printWelcomePage();
    static void printHelp();

    static void printDangerous();

    static void printSafe();

    static void printInitFailure();

    static void printDangerous(const std::vector<std::filesystem::path>& vector1);

    static void printScanfArgumentProblem();

    static void printScandArgumentProblem();

    static void printPasswordPrompt();

    static void printQuarantineRecords(const std::vector<QuarantineRecord> &vector);

    static void printQuarantineArgumentProblem();

    static void printRestoreArgumentProblem();

    static void printRestoreSuccess();

    static void printRestoreFailure();

    static void printArgumentProblem();

    static void printHelpArgumentProblem();

    static void printImposeSuccess();

    static void printPasswordPromptRestore();

    static void unallowedDirectoryScan();
};


#endif //ANTIVIRUS_CLICONTROLLER_H
