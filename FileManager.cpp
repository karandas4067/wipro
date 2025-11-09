#include "FileManager.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem;

void FileManager::run() {
    currentPath = fs::current_path().string();
    int choice;

    while (true) {
        std::cout << "\n📁 Current Directory: " << currentPath << "\n";
        std::cout << "1. List Files\n2. Navigate\n3. Manipulate Files\n4. Search\n5. Permissions\n6. Exit\nChoice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: listFiles(); break;
            case 2: navigate(); break;
            case 3: manipulateFiles(); break;
            case 4: searchFiles(); break;
            case 5: managePermissions(); break;
            case 6: return;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

void FileManager::listFiles() {
    for (const auto& entry : fs::directory_iterator(currentPath)) {
        std::cout << (entry.is_directory() ? "[DIR] " : "[FILE] ") << entry.path().filename().string() << "\n";
    }
}

void FileManager::navigate() {
    std::string dir;
    std::cout << "Enter directory name (.. to go up): ";
    std::cin >> dir;

    fs::path newPath = (dir == "..") ? fs::path(currentPath).parent_path() : fs::path(currentPath) / dir;

    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        currentPath = newPath.string();
    } else {
        std::cout << "Invalid directory.\n";
    }
}

void FileManager::manipulateFiles() {
    std::string action, src, dest;
    std::cout << "Action (copy/move/delete/rename): ";
    std::cin >> action;

    if (action == "copy" || action == "move" || action == "rename") {
        std::cout << "Source file: ";
        std::cin >> src;
        std::cout << "Destination: ";
        std::cin >> dest;

        fs::path source = fs::path(currentPath) / src;
        fs::path destination = fs::path(currentPath) / dest;

        if (fs::exists(source)) {
            if (action == "copy") fs::copy(source, destination, fs::copy_options::overwrite_existing);
            else if (action == "move") fs::rename(source, destination);
            else if (action == "rename") fs::rename(source, destination);
        } else {
            std::cout << "Source file does not exist.\n";
        }
    } else if (action == "delete") {
        std::cout << "File to delete: ";
        std::cin >> src;
        fs::path target = fs::path(currentPath) / src;
        if (fs::exists(target)) fs::remove(target);
        else std::cout << "File not found.\n";
    } else {
        std::cout << "Unknown action.\n";
    }
}

void FileManager::searchFiles() {
    std::string keyword;
    std::cout << "Enter keyword to search: ";
    std::cin >> keyword;

    for (const auto& entry : fs::recursive_directory_iterator(currentPath)) {
        if (entry.path().filename().string().find(keyword) != std::string::npos) {
            std::cout << entry.path().string() << "\n";
        }
    }
}

void FileManager::managePermissions() {
#ifdef _WIN32
    std::cout << "Permission management is limited on Windows.\n";
#else
    std::string file;
    std::cout << "Enter file name: ";
    std::cin >> file;
    fs::path target = fs::path(currentPath) / file;

    if (fs::exists(target)) {
        std::string perms;
        std::cout << "Enter permissions (e.g., 755): ";
        std::cin >> perms;
        std::string command = "chmod " + perms + " \"" + target.string() + "\"";
        system(command.c_str());
    } else {
        std::cout << "File not found.\n";
    }
#endif
}