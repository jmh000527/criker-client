#include "UserManager.h"

#include <stdexcept>

User UserManager::g_currentUser{ "" };
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

bool UserManager::isGroupChat(const QString& uid) {
	// 查找用户
	const auto iterUser = std::ranges::find_if(g_currentUserFriendList, [&uid](const auto& user) {
		return user.getId() == uid.toInt();
											   });

	// 查找群组
	const auto iterGroup = std::ranges::find_if(g_currentUserGroupList, [&uid](const auto& group) {
		return group.getId() == uid.toInt();
												});

	// 处理都找到的情况，抛出异常
	if (iterUser != g_currentUserFriendList.end() && iterGroup != g_currentUserGroupList.end()) {
		throw std::logic_error("Found in both user and group lists.");
	}

	// 处理都没找到的情况，抛出异常
	if (iterUser == g_currentUserFriendList.end() && iterGroup == g_currentUserGroupList.end()) {
		throw std::logic_error("Not found in user or group lists.");
	}

	// 返回结果
	return iterGroup != std::end(g_currentUserGroupList);
}

const User UserManager::getFriend(const QString& uid) {
	const auto iter = std::ranges::find_if(g_currentUserFriendList, [&uid](const auto& user) {
		return user.getId() == uid.toInt();
	});

	if (iter != std::end(g_currentUserFriendList)) {
		return *iter;
	} else {
		return User{};
	}
}

const Group UserManager::getGroup(const QString& uid) {
	const auto iter = std::ranges::find_if(g_currentUserGroupList, [&uid](const auto& group) {
		return group.getId() == uid.toInt();
	});

	if (iter != std::end(g_currentUserGroupList)) {
		return *iter;
	} else {
		return Group{};
	}
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
