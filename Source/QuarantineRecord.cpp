//
// Created by ixico on 10.04.2022.
//

#include "../Headers/QuarantineRecord.h"

#include <utility>

QuarantineRecord::QuarantineRecord(std::filesystem::path filePath, std::string digest) : file_path(std::move(
        filePath)), digest(std::move(digest)) {}

QuarantineRecord::QuarantineRecord() = default;

std::ostream &operator<<(std::ostream &os, const QuarantineRecord &record) {
    os << "file_path: " << record.file_path << " digest: " << record.digest;
    return os;
}
