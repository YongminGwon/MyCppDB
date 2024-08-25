#pragma once

class FileManager
{
public:
	FileManager();
	~FileManager();
	
	void updateFileData(const UserData& user, size_t fileOffset);

};