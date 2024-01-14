#include "Group.h"

GroupUser::GroupUser(
	std::string base64String, int id, std::string name, std::string password, std::string state, std::string role)
	: User(std::move(base64String), id, std::move(name), std::move(password), std::move(state)),
	  m_role{ std::move(role) } {}

std::string GroupUser::getRole() const {
	return m_role;
}

void GroupUser::setRole(const std::string& role) {
	m_role = role;
}


Group::Group(int id, std::string name, std::string desc)
	: m_id{ id }, m_name{ std::move(name) }, m_desc{ std::move(desc) } {}

int Group::getId() const {
	return m_id;
}

std::string Group::getName() const {
	return m_name;
}

std::string Group::getDesc() const {
	return m_desc;
}

void Group::setId(int id) {
	m_id = id;
}

void Group::setName(const std::string& name) {
	m_name = name;
}

void Group::setDesc(const std::string& desc) {
	m_desc = desc;
}

std::vector<GroupUser> Group::getUsers() const {
	return m_groupusers;
}

void Group::setUsers(std::vector<GroupUser> groupUsers) {
	m_groupusers = std::move(groupUsers);
}
