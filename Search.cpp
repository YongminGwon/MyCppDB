#include "Search.h"
#include "QueryParser.h"
#include <fstream>
#include <iostream>
#include <algorithm>

std::string toLowerCase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

void Search::searchUsers(const std::string& query, DataManager& dataManager)
{
    QueryParser parser;
    std::vector<FieldValue> fieldValues = parser.parseQuery(query);
    Condition condition = parser.getCondition();

    dataManager.users.clear();

    if (condition == Condition::NONE)
    {
        search(fieldValues[0], dataManager);
    }
    else if (condition == Condition::AND)
    {
        searchAnd(fieldValues, dataManager);
    }
    else if (condition == Condition::OR)
    {
        searchOr(fieldValues, dataManager);
    }

    if (dataManager.users.empty()) {
        std::cout << "No matching users found." << std::endl;
    }
    else {
        std::cout << "Matched users:" << std::endl;
        for (const auto& user : dataManager.users) {
            std::cout << "Index: " << user.second
                << ", Name: " << user.first.getName()
                << ", Address: " << user.first.getAddress()
                << ", Phone: " << user.first.getPhone() << std::endl;
        }
    }
}

void Search::search(const FieldValue& fieldValue, DataManager& dataManager)
{
    std::ifstream userFile("users.dat", std::ios::binary);
    if (!userFile) {
        std::cerr << "Failed to open users.dat" << std::endl;
        return;
    }

    UserData user;
    size_t index = 0;

    while (userFile.read(reinterpret_cast<char*>(&user), sizeof(UserData))) {
        if (index < dataManager.deletedRecords.size() && dataManager.deletedRecords[index]) {
            ++index;
            continue;
        }

        std::string userValue;
        std::string searchValue = toLowerCase(fieldValue.value);

        switch (fieldValue.field) {
        case Field::NAME:
            userValue = toLowerCase(user.getName());
            break;
        case Field::ADDRESS:
            userValue = toLowerCase(user.getAddress());
            break;
        case Field::PHONE:
            userValue = toLowerCase(user.getPhone());
            break;
        default:
            break;
        }

        if (userValue.find(searchValue) != std::string::npos) {
            dataManager.users.emplace_back(user, index);
        }

        ++index;
    }

    userFile.close();
}

void Search::searchAnd(const std::vector<FieldValue>& fieldValues, DataManager& dataManager)
{
    std::ifstream userFile("users.dat", std::ios::binary);
    if (!userFile) {
        std::cerr << "Failed to open users.dat" << std::endl;
        return;
    }

    UserData user;
    size_t index = 0;

    while (userFile.read(reinterpret_cast<char*>(&user), sizeof(UserData))) {
        if (index < dataManager.deletedRecords.size() && dataManager.deletedRecords[index]) {
            ++index;
            continue;
        }

        bool matches = true;

        for (const auto& fieldValue : fieldValues) {
            std::string userValue;
            std::string searchValue = toLowerCase(fieldValue.value);

            switch (fieldValue.field) {
            case Field::NAME:
                userValue = toLowerCase(user.getName());
                break;
            case Field::ADDRESS:
                userValue = toLowerCase(user.getAddress());
                break;
            case Field::PHONE:
                userValue = toLowerCase(user.getPhone());
                break;
            default:
                matches = false;
                break;
            }

            if (userValue.find(searchValue) == std::string::npos) {
                matches = false;
                break;
            }
        }

        if (matches) {
            dataManager.users.emplace_back(user, index);
        }

        ++index;
    }

    userFile.close();
}

void Search::searchOr(const std::vector<FieldValue>& fieldValues, DataManager& dataManager)
{
    std::ifstream userFile("users.dat", std::ios::binary);
    if (!userFile) {
        std::cerr << "Failed to open users.dat" << std::endl;
        return;
    }

    UserData user;
    size_t index = 0;

    while (userFile.read(reinterpret_cast<char*>(&user), sizeof(UserData))) {
        if (index < dataManager.deletedRecords.size() && dataManager.deletedRecords[index]) {
            ++index;
            continue;
        }

        bool matches = false;

        for (const auto& fieldValue : fieldValues) {
            std::string userValue;
            std::string searchValue = toLowerCase(fieldValue.value);

            switch (fieldValue.field) {
            case Field::NAME:
                userValue = toLowerCase(user.getName());
                break;
            case Field::ADDRESS:
                userValue = toLowerCase(user.getAddress());
                break;
            case Field::PHONE:
                userValue = toLowerCase(user.getPhone());
                break;
            default:
                break;
            }

            if (userValue.find(searchValue) != std::string::npos) {
                matches = true;
                break;
            }
        }

        if (matches) {
            dataManager.users.emplace_back(user, index);
        }

        ++index;
    }

    userFile.close();
}
