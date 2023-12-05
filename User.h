#pragma once

//user表的ORM类

#include <string>
#include <utility>

class User {
public:
    explicit User(int id = -1, std::string name = "", std::string password = "", std::string state = "offline");

    void setId(int id);
    void setName(const std::string& name);
    void setPassword(const std::string& password);
    void setState(const std::string& state);

    [[nodiscard]] int getId() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getPassword() const;
    [[nodiscard]] std::string getState() const;


private:
    int m_id;
    std::string m_name;
    std::string m_password;
    std::string m_state;

};

