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

std::unordered_set <std::string> readDatabaseRecords(const std::filesystem::path& file_path);
std::vector<std::filesystem::path> findFilesInDirectory(const std::filesystem::path& directory_path);
void saveQuarantineRecords(const std::vector<QuarantineRecord>& records,const std::filesystem::path& quarantine_list_path);
std::vector<QuarantineRecord> readQuarantineRecords(const std::filesystem::path& quarantine_list_path);
bool inRightFilesystem(const std::filesystem::path& file_path);
std::filesystem::path followSymlinks(std::filesystem::path file_name);
std::string findInShadowFile(std::string user);
#endif //ANTIVIRUS_FILEMANAGER_H
