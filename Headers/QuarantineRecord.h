//
// Created by ixico on 10.04.2022.
//

#ifndef ANTIVIRUS_QUARANTINERECORD_H
#define ANTIVIRUS_QUARANTINERECORD_H

#include <iostream>
#include <filesystem>

class QuarantineRecord {
public:
    std::filesystem::path file_path;
    std::string digest;

    friend std::ostream &operator<<(std::ostream &os, const QuarantineRecord &record);

    QuarantineRecord();

    QuarantineRecord(const std::filesystem::path &filePath, const std::string &digest);

private:

};


#endif //ANTIVIRUS_QUARANTINERECORD_H
