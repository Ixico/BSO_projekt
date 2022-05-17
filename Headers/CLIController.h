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
    static std::string printWelcomePage();
    static std::string printHelp();

    static std::string printDangerous();

    static std::string printSafe();

    static std::string printInitFailure();

    static std::string printDangerous(const std::vector<std::filesystem::path>& vector1);

    static std::string printScanfArgumentProblem();

    static std::string printScandArgumentProblem();

    static std::string printPasswordPrompt();

    static std::string printQuarantineRecords(const std::vector<QuarantineRecord> &vector);

    static std::string printQuarantineArgumentProblem();

    static std::string printRestoreArgumentProblem();

    static std::string printRestoreSuccess();

    static std::string printRestoreFailure();

    static std::string printArgumentProblem();

    static std::string printHelpArgumentProblem();

    static std::string printImposeSuccess();

    static std::string printPasswordPromptRestore();

    static std::string unallowedDirectoryScan();

    static std::string printManVisit();

    static std::string internalProblem();

    static std::string permissionDenied();

    static std::string notFound();

    static std::string wrongType();
};


#endif //ANTIVIRUS_CLICONTROLLER_H
