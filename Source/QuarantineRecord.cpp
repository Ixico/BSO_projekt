//
// Created by ixico on 10.04.2022.
//

#include "../Headers/QuarantineRecord.h"

QuarantineRecord::QuarantineRecord(const std::filesystem::path &filePath, const std::string &digest) : file_path(
        filePath), digest(digest) {}

QuarantineRecord::QuarantineRecord() {}

std::ostream &operator<<(std::ostream &os, const QuarantineRecord &record) {
    os << "file_path: " << record.file_path << " digest: " << record.digest;
    return os;
}
