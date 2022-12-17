#pragma once
#include <string>

class WrongAnswer {
public:
	WrongAnswer();

private:
	std::string m_answer;
	unsigned int m_id;
};

class User
{
public:
	User(unsigned int id, const std::string &name, const std::string &password);
public:
	const std::string& GetName() const;
	const std::string& GetPassword() const;
	unsigned int GetId() const;
	void SetId(unsigned int id);
	void SetName(const std::string &name);
	void SetPassword(const std::string& password);
	
private:
	unsigned int m_id;
	std::string m_name;
	std::string m_password;
};

