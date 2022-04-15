cmake_minimum_required(VERSION 3.22)
project(L_Antivirus)

set(CMAKE_CXX_STANDARD 17)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

add_executable(L_Antivirus main.cpp Controllers/CLIController.cpp Headers/CLIController.h Controllers/FileController.cpp Headers/FileController.h Controllers/QuarantineController.cpp Headers/QuarantineController.h Source/FileHasher.cpp Headers/FileHasher.h Source/FileManager.cpp Headers/FileManager.h Headers/ConfigurationVariables.h Source/QuarantineRecord.cpp Headers/QuarantineRecord.h)

target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)