#pragma once
#include "Observer.h"
#include "DataManager.h"
#include <string>
#include <vector>

class UI : public Observer
{
public:
	UI()
	{
		DataManager::getInstance().attach(this);
		initCommandMap();
	};
	~UI()
	{
		DataManager::getInstance().detach(this);
	}
	void run();
	void update() override
	{
		auto& dataManager = DataManager::getInstance();
		displayUsers(dataManager.users);
		displayRecords(dataManager.deletedRecords);
	}
private:
	void displayMainMenu();
	void handleUserInput();
	void initCommandMap();

	void exit();
	void addUser();
	void editUser();
	void deleteUser();
	void searchUsers();

	void displayRecords(const std::vector<bool>& deletedRecords);
	void displayUsers(const std::vector<std::pair<UserData, size_t>>& users);
	void displayUsers(const std::vector<std::pair<UserData, size_t>>& users, size_t selectedIdx);
	std::pair<UserData, size_t> navigateSearchResult(const std::vector<std::pair<UserData, size_t>>& users);

	std::vector<void (UI::*)()> commandMap;
};