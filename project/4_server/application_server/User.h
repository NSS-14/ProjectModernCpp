#pragma once
#include <string>

class User
{
public:
	User() = default;
	User(unsigned int id, const std::string &name, const std::string &password);
	User(const User& user);

public:
	User& operator =(const User& user);
	bool operator ==(const User& user);

public:
	const std::string& GetName() const;
	const std::string& GetPassword() const;
	unsigned int GetId() const;

public:
	void SetId(unsigned int id);
	void SetName(const std::string &name);
	void SetPassword(const std::string& password);

protected:
	unsigned int m_id;
	std::string m_name;
	std::string m_password;
};

