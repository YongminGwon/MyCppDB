#include "UserData.h"
#include <string>
#include <cstring>

UserData::UserData(const std::string& name, const std::string& address, const std::string& phone) {
    setName(name);
    setAddress(address);
    setPhone(phone);
}

const std::string UserData::getName() const {
    return std::string(name);
}

const std::string UserData::getAddress() const {
    return std::string(address);
}

const std::string UserData::getPhone() const {
    return std::string(phone);
}

void UserData::setName(const std::string& name) {
    strncpy_s(this->name, sizeof(this->name), name.c_str(), _TRUNCATE);
}

void UserData::setAddress(const std::string& address) {
    strncpy_s(this->address, sizeof(this->address), address.c_str(), _TRUNCATE);
}

void UserData::setPhone(const std::string& phone) {
    strncpy_s(this->phone, sizeof(this->phone), phone.c_str(), _TRUNCATE);
}