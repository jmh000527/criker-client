#pragma once

#include <QString>

#include "user.h"
#include "group.h"
#include <vector>

class UserManager {
public:
    static UserManager* getInstance();

    static const User& getCurrentUser();
    static const std::vector<User>& getCurrentUserFriendList();
    static const std::vector<Group>& getCurrentUserGroupList();

    static bool isGroupChat(const QString& uid);

    static const User getFriend(const QString& uid);
    static const Group getGroup(const QString& uid);

    static void setCurrentUser(const User& user);
    static void setCurrentUserFriendList(const std::vector<User>& friends);
    static void setCurrentUserFriendList(std::vector<User>&& friends); // 右值引用版本

    static void setCurrentUserGroupList(const std::vector<Group>& groups);
    static void setCurrentUserGroupList(std::vector<Group>&& groups); // 右值引用版本

private:
    UserManager() = default;

    // 记录当前系统登录的用户信息
    static User g_currentUser;

    // 记录当前登录用户的好友列表信息
    static std::vector<User> g_currentUserFriendList;

    // 记录当前登录用户的群组列表信息
    static std::vector<Group> g_currentUserGroupList;
};
