#pragma once

//user表的ORM类

#include <string>
#include <utility>
#include <vector>

class User {
public:
	explicit User(int id = -1, std::string name = "", std::string password = "", std::string state = "offline");
	explicit User(
		std::string base64, int id = -1, std::string name = "", std::string password = "",
		std::string state = "offline");

	void setId(int id);
	void setName(const std::string& name);
	void setPassword(const std::string& password);
	void setState(const std::string& state);
	void setHeadImage(const std::string& base64ImageBinary);

	[[nodiscard]] int getId() const;
	[[nodiscard]] std::string getName() const;
	[[nodiscard]] std::string getPassword() const;
	[[nodiscard]] std::string getState() const;
	[[nodiscard]] std::string getHeadImage() const;

private:
	int m_id;
	std::string m_name;
	std::string m_password;
	std::string m_state;
	std::string m_base64ImageBinary;
};
