#pragma once
#include "DataManager.h"
#include "QueryParser.h"

class Search {
public:
    void searchUsers(const std::string& query, DataManager& dataManager);
private:
    void search(const FieldValue& fieldValue, DataManager& dataManager);
    void searchAnd(const std::vector<FieldValue>& fieldValues, DataManager& dataManager);
    void searchOr(const std::vector<FieldValue>& fieldValues, DataManager& dataManager);
};