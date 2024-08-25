#include "DataManager.h"
#include "QueryParser.h"
#include "FileManager.h"
#include <fstream>
#include <iostream>

void DataManager::updateRAMUser(const UserData& user, size_t offset)
{
	FileManager fileManager;
	fileManager.updateFileData(user, offset);
    auto it = std::find_if(users.begin(), users.end(),
        [offset](const std::pair<UserData, size_t>& userPair) {
            return userPair.second == offset;
        });

    if (it != users.end()) 
    {
        it->first = user;
    }
    else {
        users.push_back({ user, offset });
    }
}

void DataManager::loadDeletedRecords()
{
	std::ifstream file("deleted.dat", std::ios::binary);
	bool status;
	deletedRecords.clear();
	while (file.read(reinterpret_cast<char*>(&status), sizeof(bool))) 
	{
		deletedRecords.emplace_back(status);
	}

	file.close();
}

void DataManager::saveDeletedRecords()
{
    std::ofstream file("deleted.dat", std::ios::binary | std::ios::trunc);

    for (bool status : deletedRecords) {
        file.write(reinterpret_cast<const char*>(&status), sizeof(bool));
    }

    file.close();
}