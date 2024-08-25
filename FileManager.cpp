#include <fstream>
#include <vector>
#include <iostream>
#include "UserData.h"
#include "FileManager.h"

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

void FileManager::updateFileData(const UserData& user, size_t offset)
{
	std::fstream file("users.dat", std::ios::in | std::ios::out | std::ios::binary);

	size_t recordSize = sizeof(UserData);
	file.seekp(offset * recordSize);
	file.write(reinterpret_cast<const char*>(&user), recordSize);
	file.close();
}
