#include "User.h"

User::User(int id, std::string name, std::string password, std::string state)
    : m_id{ id }, m_name{ std::move(name) }, m_password{ std::move(password) }, m_state{ std::move(state) } {}

void User::setId(int id) {
    m_id = id;
}

void User::setName(const std::string& name) {
    m_name = name;
}

void User::setPassword(const std::string& password) {
    m_password = password;
}

void User::setState(const std::string& state) {
    m_state = state;
}

int User::getId() const {
    return m_id;
}

std::string User::getName() const {
    return m_name;
}

std::string User::getPassword() const {
    return m_password;
}

std::string User::getState() const {
    return m_state;
}