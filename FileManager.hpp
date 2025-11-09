#pragma once
#include <string>

class FileManager {
public:
    void run();
private:
    std::string currentPath;
    void listFiles();
    void navigate();
    void manipulateFiles();
    void searchFiles();
    void managePermissions();
};