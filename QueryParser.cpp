#include "QueryParser.h"
#include <iostream>
#include <sstream>
#include <algorithm>

QueryParser::QueryParser() :condition(Condition::NONE)
{
}

std::vector<FieldValue> QueryParser::parseQuery(const std::string& query) {
    std::vector<FieldValue> result;
    std::istringstream iss(query);
    std::string token;
    condition = Condition::NONE;

    while (std::getline(iss, token, ' ')) {
        if (token == "and") {
            condition = Condition::AND;
            continue;
        }
        else if (token == "or") {
            condition = Condition::OR;
            continue;
        }

        size_t pos = token.find('=');
        if (pos != std::string::npos) {
            std::string fieldStr = trim(token.substr(0, pos));
            std::string value = trim(token.substr(pos + 1));

            if (value.front() == '\'' && value.back() == '\'') {
                value = value.substr(1, value.length() - 2);
            }

            Field field = stringToField(fieldStr);
            if (field != Field::NONE) {
                result.emplace_back(field, value);
            }
        }
    }

    return result;
}

Condition QueryParser::getCondition() const
{
    return condition;
}

Field QueryParser::stringToField(const std::string& fieldStr)
{
    if (fieldStr == "이름") return Field::NAME;
    else if (fieldStr == "주소") return Field::ADDRESS;
    else if (fieldStr == "전화번호") return Field::PHONE;
    else
    {
        std::cerr << "No Field Found" << std::endl;
        return Field::NONE;
    }
}

std::string QueryParser::trim(const std::string& str) {
    auto start = str.find_first_not_of(' ');
    auto end = str.find_last_not_of(' ');

    if (start == std::string::npos || end == std::string::npos)
        return "";

    return str.substr(start, end - start + 1);
}