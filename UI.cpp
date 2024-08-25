#include "UI.h"
#include "UserData.h"
#include "FileManager.h"
#include "DataManager.h"
#include "QueryParser.h"
#include "Search.h"
#include <iostream>
#include <conio.h>
#include <fstream>

void UI::run()
{
    while (true)
    {
        displayMainMenu();
        handleUserInput();
    }
}

void UI::displayMainMenu()
{
    std::cout << "==========================================" << std::endl;
    std::cout << "               User Interface             " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "[1] Add User    [2] Edit User    [3] Delete User" << std::endl;
    std::cout << "[4] Search User [0] Exit" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Input: ";
}

void UI::handleUserInput()
{
    int input;
    std::cin >> input;

    if (input >= 0 && input < commandMap.size())
    {
        (this->*commandMap[input])();
    }
    else
    {
        std::cout << "Invalid input." << std::endl;
    }
}

void UI::initCommandMap()
{
    commandMap = { &UI::exit, &UI::addUser, &UI::editUser, &UI::deleteUser, &UI::searchUsers };
}

void UI::exit()
{
    std::exit(0);
}

void UI::addUser()
{
    std::string name, address, phone;
    std::cin.ignore();
    std::cout << "Enter name: ";
    std::getline(std::cin, name);
    std::cout << "Enter address: ";
    std::getline(std::cin, address);
    std::cout << "Enter phone: ";
    std::getline(std::cin, phone);

    UserData newUser(name, address, phone);

    //데이터 추가(RAM, File)
    auto& dataManager = DataManager::getInstance();
    size_t offset;
    auto it = std::find(dataManager.deletedRecords.begin(), dataManager.deletedRecords.end(), true);
    if (it != dataManager.deletedRecords.end()) 
    {
        offset = std::distance(dataManager.deletedRecords.begin(), it);
    }
    else
    {
        offset = dataManager.deletedRecords.size();
        dataManager.deletedRecords.push_back(false);
    }
    dataManager.updateRAMUser(newUser, offset);
    std::cout << "User added successfully." << std::endl;
}

void UI::editUser()
{
    std::string query;
    std::cout << "Enter search query: ";
    std::cin.ignore();
    std::getline(std::cin, query);

    //Search
    Search search;
    search.searchUsers(query, DataManager::getInstance());
    //데이터 수정(RAM, File) deletedRecord 복구
    auto& users = DataManager::getInstance().users;
    if (users.empty())
    {
        std::cout << "No users found" << std::endl;
        return;
    }
    std::pair<UserData, size_t> selectedUser = navigateSearchResult(users);
    std::string name, address, phone;
    std::cout << "Enter new name (or press Enter to keep current: " << selectedUser.first.getName() << "): ";
    std::getline(std::cin, name);
    if (name.empty()) name = selectedUser.first.getName();

    std::cout << "Enter new address (or press Enter to keep current: " << selectedUser.first.getAddress() << "): ";
    std::getline(std::cin, address);
    if (address.empty()) address = selectedUser.first.getAddress();

    std::cout << "Enter new phone (or press Enter to keep current: " << selectedUser.first.getPhone() << "): ";
    std::getline(std::cin, phone);
    if (phone.empty()) phone = selectedUser.first.getPhone();
    UserData updatedUser(name, address, phone);
    DataManager::getInstance().updateRAMUser(updatedUser, selectedUser.second);

    std::cout << "User updated successfully." << std::endl;
}

void UI::deleteUser()
{
    std::string query;
    std::cout << "Enter search query: ";
    std::cin.ignore();
    std::getline(std::cin, query);
    //Search
    Search search;
    search.searchUsers(query, DataManager::getInstance());
    auto& users = DataManager::getInstance().users;
    if (users.empty()) {
        std::cout << "No users found." << std::endl;
        return;
    }
    //deletedRecord변경
    std::pair<UserData, size_t> selectedUser = navigateSearchResult(users);
    DataManager::getInstance().deletedRecords[selectedUser.second] = true;

    users.erase(std::remove_if(users.begin(), users.end(), [&](const std::pair<UserData, size_t>& user) {
        return user.second == selectedUser.second;
        }), users.end());
    std::cout << "User deleted successfully." << std::endl;
}

void UI::searchUsers()
{
    std::string query;
    std::cout << "Enter search query: ";
    std::cin.ignore();
    std::getline(std::cin, query);
    Search search;
    search.searchUsers(query, DataManager::getInstance());
    auto& users = DataManager::getInstance().users;
    displayUsers(users);
}

void UI::displayRecords(const std::vector<bool>& deletedRecords)
{
    std::cout << "Deleted Records:" << std::endl;
    bool hasDeletedRecords = false;

    for (size_t i = 0; i < deletedRecords.size(); ++i)
    {
        if (deletedRecords[i]) {
            std::cout << "Record " << i << " is deleted." << std::endl;
            hasDeletedRecords = true;
        }
    }
}


void UI::displayUsers(const std::vector<std::pair<UserData, size_t>>& users)
{
    DataManager::getInstance();
    if (users.empty()) {
        std::cout << "No users found." << std::endl;
        return;
    }
    std::cout << "==========================================" << std::endl;
    for (size_t i = 0; i < users.size(); i++)
    {
        const UserData& user = users[i].first;
        std::cout << "[" << i << "] "
            << "Name: " << user.getName()
            << " | Address: " << user.getAddress()
            << " | Phone: " << user.getPhone() << std::endl;
    }
    std::cout << "==========================================" << std::endl;
}

void UI::displayUsers(const std::vector<std::pair<UserData, size_t>>& users, size_t selectedIdx)
{
    std::cout << "             Search Results               " << std::endl;
    std::cout << "==========================================" << std::endl;
    for (size_t i = 0; i < users.size(); i++)
    {
        const UserData& user = users[i].first;
        std::cout << (i == selectedIdx ? "> " : "  ")
            << "[" << i << "] "
            << "Name: " << user.getName()
            << " | Address: " << user.getAddress()
            << " | Phone: " << user.getPhone() << std::endl;
    }
    std::cout << "==========================================" << std::endl;
}

std::pair<UserData, size_t> UI::navigateSearchResult(const std::vector<std::pair<UserData, size_t>>& users)
{
    if (users.empty())
    {
        std::cout << "No results found." << std::endl;
        return std::pair<UserData, size_t>();
    }

    size_t curIdx = 0;
    const int UP = 72;
    const int DOWN = 80;
    const int ENTER = 13;

    while (true)
    {
        system("cls");
        std::cout << "Search Results (Use arrow keys to navigate, Enter to select):\n";
        displayUsers(users, curIdx);

        int input = _getch();
        switch (input)
        {
        case UP:
            if (curIdx > 0) curIdx--;
            break;
        case DOWN:
            if (curIdx < users.size() - 1) curIdx++;
            break;
        case ENTER:
            return users[curIdx];
        default:
            break;
        }
    }
}
