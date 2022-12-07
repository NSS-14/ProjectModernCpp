#pragma once
#include <string>
class User
{
public:
	User(const std::string &name, const std::string &password);
public:
	const std::string& GetName() const;
	const std::string& GetPassword() const;
	void SetName(const std::string &name);
	void SetPassword(const std::string& password);

private:
	std::string m_name;
	std::string m_password;
};

