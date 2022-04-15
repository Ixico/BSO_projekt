//
// Created by ixico on 23.03.2022.
//

#ifndef ANTIVIRUS_QUARANTINECONTROLLER_H
#define ANTIVIRUS_QUARANTINECONTROLLER_H

#include <iostream>
#include <filesystem>
#include <vector>
#include "QuarantineRecord.h"


class QuarantineController {
public:

    QuarantineController();
    void init(const std::filesystem::path& quarantine_directory_path);
    void imposeQuarantine(std::filesystem::path file_path);
    bool removeQuarantine(const std::string& file_name);
    void saveQuarantineRecords();

    void setPassword(const std::string &pwd);

    const std::vector<QuarantineRecord> &getQuarantineRecords() const;

private:
    std::vector<QuarantineRecord> quarantine_records;
    const int AES_BLOCK_SIZE = 16;
    std::string password;
    void useCipher(const std::filesystem::path& file_path, const std::filesystem::path& destination_path);
    std::filesystem::path quarantine_path;
    std::filesystem::path quarantine_list_path;
};


#endif //ANTIVIRUS_QUARANTINECONTROLLER_H
