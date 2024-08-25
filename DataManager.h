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

	// RAM 데이터 수정
	void updateRAMUser(const UserData& user, size_t offset);

	void loadDeletedRecords();
	void saveDeletedRecords();
	// 검색시작시에 초기화 UI에서 검색이 끝나면 초기화 edit이나 add할때 변경
	std::vector<std::pair<UserData, size_t>> users;
	// 데이터 삭제, 데이터 추가시에만 변경
	std::vector<bool> deletedRecords;

private:
	DataManager() = default;
	~DataManager() = default;

	DataManager(const DataManager&) = delete;
	DataManager& operator=(const DataManager&) = delete;
	std::vector<Observer*> observers;
};