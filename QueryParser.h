#pragma once
#include <string>
#include <vector>

enum class Condition
{
	NONE, AND, OR
};

enum class Field
{
	NAME, ADDRESS, PHONE, NONE
};

struct FieldValue
{
	Field field;
	std::string value;
	FieldValue(Field f, const std::string& v) : field(f), value(v) {}
};

class QueryParser
{
public:
	QueryParser();
	std::vector<FieldValue> parseQuery(const std::string& query);
	Condition getCondition() const;
private:
	Condition condition = Condition::NONE;
	Field stringToField(const std::string& fieldStr);
	std::string trim(const std::string& str);
};