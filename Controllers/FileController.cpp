//
// Created by ixico on 17.03.2022.
//

#include "../Headers/FileController.h"
#include "../Headers/FileManager.h"
#include "../Headers/FileHasher.h"
#include <iostream>
#include <vector>
#include <filesystem>

using std::string;
using std::unordered_set;
using std::vector;
using std::filesystem::path;


bool FileController::isFileDangerous(path file_path) {
    if (!std::filesystem::exists(file_path)) throw std::invalid_argument("File does not exist!"); //existing file or not broken symlink
    file_path = followSymlinks(file_path); //follow symlinks
    if (!std::filesystem::is_regular_file(file_path)) throw std::invalid_argument("File is not regular one!"); //check if regular file
    if (!inRightFilesystem(file_path)) throw std::invalid_argument("File in unsupported filesystem!"); //check filesystem
    string digest = calculateFileHash(file_path);
    return hashes.find(digest) != hashes.end();
}

vector<path> FileController::findDangerousFiles(path directory_path){
    directory_path = followSymlinks(directory_path);
    if(!std::filesystem::is_directory(directory_path)) throw std::invalid_argument("It is not directory path!");
    vector<path> files = findFilesInDirectory(directory_path);
    vector<path> dangerous_files;
    int iterator = 1;
    for (const auto &file : files){
        std::cout << "Scanned files: " << iterator << "/" << files.size() << "\r";
        try {
            if (isFileDangerous(file)) dangerous_files.push_back(file);
        } catch (std::invalid_argument &e){} //skip files that cannot be scanned
        iterator++;
    }
    return dangerous_files;
}


FileController::FileController() = default;

void FileController::init(const path& database_path) {
    hashes = readDatabaseRecords(database_path);
}
