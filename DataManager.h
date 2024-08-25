#pragma once
#include "UserData.h"
#include <vector>
#include <fstream>
#include "Observer.h"

class DataManager : public Subject
{
public:
	static DataManager& getInstance()
	{
		static DataManager instance;
		return instance;
	}
	void attach(Observer* observer) override 
	{
		observers.push_back(observer);
	}

	void detach(Observer* observer) override 
	{
		observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	}

	void notify() override 
	{
		for (Observer* observer : observers) 
		{
			observer->update();
		}
	}

	// RAM ������ ����
	void updateRAMUser(const UserData& user, size_t offset);

	void loadDeletedRecords();
	void saveDeletedRecords();
	// �˻����۽ÿ� �ʱ�ȭ UI���� �˻��� ������ �ʱ�ȭ edit�̳� add�Ҷ� ����
	std::vector<std::pair<UserData, size_t>> users;
	// ������ ����, ������ �߰��ÿ��� ����
	std::vector<bool> deletedRecords;

private:
	DataManager() = default;
	~DataManager() = default;

	DataManager(const DataManager&) = delete;
	DataManager& operator=(const DataManager&) = delete;
	std::vector<Observer*> observers;
};