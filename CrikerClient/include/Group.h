#pragma once

//all_group表的ORM类

#include <string>
#include <vector>

#include "User.h"

class GroupUser : public User {
public:
    explicit GroupUser(std::string base64String, int id, std::string name, std::string password, std::string state, std::string role);

    std::string getRole() const;
    void setRole(const std::string& role);

private:
    std::string m_role;

};

class Group {
public:
    explicit Group(int id = -1, std::string name = "", std::string desc = "");

    int getId() const;
    std::string getName() const;
    std::string getDesc() const;

    void setId(int id);
    void setName(const std::string& name);
    void setDesc(const std::string& desc);

    std::vector<GroupUser> getUsers() const;
    void setUsers(std::vector<GroupUser> groupUsers);
private:
    int m_id;
    std::string m_name;
    std::string m_desc;
    std::vector<GroupUser> m_groupusers;
};
