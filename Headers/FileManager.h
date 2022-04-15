//
// Created by ixico on 17.03.2022.
//
#include <iostream>
#include <unordered_set>
#include <vector>
#include <filesystem>
#include "QuarantineRecord.h"

#ifndef ANTIVIRUS_FILEMANAGER_H
#define ANTIVIRUS_FILEMANAGER_H

std::unordered_set <std::string> readDatabaseRecords(std::filesystem::path file_path);
std::vector<std::filesystem::path> findFilesInDirectory(std::filesystem::path directory_path);
void saveQuarantineRecords(std::vector<QuarantineRecord> records,std::filesystem::path quarantine_list_path);
std::vector<QuarantineRecord> readQuarantineRecords(std::filesystem::path quarantine_list_path);
bool inRightFilesystem(std::filesystem::path file_path);
std::filesystem::path followSymlinks(std::filesystem::path file_name);
#endif //ANTIVIRUS_FILEMANAGER_H
