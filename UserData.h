#pragma once
#include <string>

class UserData
{
public:
    UserData() = default;
    UserData(const std::string& name, const std::string& address, const std::string& phone);
    ~UserData() = default;

    const std::string getName() const;
    const std::string getAddress() const;
    const std::string getPhone() const;

    void setName(const std::string& name);
    void setAddress(const std::string& address);
    void setPhone(const std::string& phone);

private:
    char name[32]{};
    char address[32]{};
    char phone[32]{};
};