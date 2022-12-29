#pragma once
#include <string>

class User
{
public:
	User() = default;
	User(unsigned int id, const std::string& name, const std::string& password);
	User(const User& user);
	User(User&& user) noexcept;

public:
	User& operator =(const User& user);
	User& operator =(User&& user) noexcept;
	bool operator ==(const User& user);

public:
	unsigned int GetId() const;
	const std::string& GetName() const;
	const std::string& GetPassword() const;

public:
	void SetId(unsigned int id);
	void SetName(const std::string& name);
	void SetPassword(const std::string& password);

protected:
	unsigned int m_id;
	std::string m_name;
	std::string m_password;
};
