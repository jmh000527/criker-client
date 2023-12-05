#include "UserManager.h"

User UserManager::g_currentUser;
std::vector<User> UserManager::g_currentUserFriendList;
std::vector<Group> UserManager::g_currentUserGroupList;

UserManager* UserManager::getInstance() {
    static UserManager userManager;
    return &userManager;
}

const User& UserManager::getCurrentUser() {
    return g_currentUser;
}

const std::vector<User>& UserManager::getCurrentUserFriendList() {
    return g_currentUserFriendList;
}

const std::vector<Group>& UserManager::getCurrentUserGroupList() {
    return g_currentUserGroupList;
}

void UserManager::setCurrentUser(const User& user) {
    g_currentUser = user;
}

void UserManager::setCurrentUserFriendList(const std::vector<User>& friends) {
    g_currentUserFriendList = friends;
}

void UserManager::setCurrentUserFriendList(std::vector<User>&& friends) {
    g_currentUserFriendList = std::move(friends);
}

void UserManager::setCurrentUserGroupList(const std::vector<Group>& groups) {
    g_currentUserGroupList = groups;
}

void UserManager::setCurrentUserGroupList(std::vector<Group>&& groups) {
    g_currentUserGroupList = std::move(groups);
}