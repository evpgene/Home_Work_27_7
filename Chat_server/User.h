#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Chat.h"
#include "SaveRestor.h"

using namespace std;

class User
{
public:
	User();
	User(const std::size_t id, const std::string& login, const std::string& pass);
	User(std::size_t id, std::string&& login, std::string&& pass);  
	~User();
	void setId(const std::size_t);
	void setLogin(const std::string&);
	void setPass(const std::string&);
	std::size_t getId() const;
	std::string getLogin() const;
	std::string getPass() const;
	void printUser() const;

friend std::string SaveRestor::saveUser(std::shared_ptr<User> user);
friend std::shared_ptr<User> restorUser(std::string& str);

friend bool operator==(const User& u1, const User& u2);
friend bool operator!=(const User& u1, const User& u2);

friend bool operator==(const User& u1, const std::string& u2);
friend bool operator!=(const User& u1, const std::string& u2);

friend bool operator==(const std::string& u1, const User& u2);
friend bool operator!=(const std::string& u1, const User& u2);

private:
	std::size_t _id{0};
	std::string _login;
	std::string _pass;
};
