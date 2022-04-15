//
// Created by ixico on 17.03.2022.
//

#ifndef ANTIVIRUS_FILECONTROLLER_H
#define ANTIVIRUS_FILECONTROLLER_H
#include <iostream>
#include <unordered_set>
#include <vector>
#include <filesystem>

class FileController {
public:
    FileController();
    void init(const std::filesystem::path& database_path);
    bool isFileDangerous(std::filesystem::path file_path);
    std::vector<std::filesystem::path> findDangerousFiles(std::filesystem::path directory_path);

private:
    std::unordered_set <std::string> hashes;
};
#endif //ANTIVIRUS_FILECONTROLLER_H
