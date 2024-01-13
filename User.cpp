#include "user.h"

User::User(int id, std::string name, std::string password, std::string state)
    : m_id{ id }, m_name{ std::move(name) }, m_password{ std::move(password) }, m_state{ std::move(state) } {}

User::User(std::string base64, int id, std::string name, std::string password, std::string state)
    : m_base64ImageBinary{ std::move(base64) }, m_id{ id }, m_name{ std::move(name) },
    m_password{ std::move(password) }, m_state{ std::move(state) } {}

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

void User::setHeadImage(const std::string& base64ImageBinary) {
    m_base64ImageBinary = base64ImageBinary;
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

std::string User::getHeadImage() const {
    return m_base64ImageBinary;
}
