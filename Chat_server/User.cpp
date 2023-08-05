#include "User.h"
#include <iostream>

User::User()
{
}

User::User(const std::size_t id, const std::string& login, const std::string& pass) : _id(id), _login(login), _pass(pass)
{
}

User::User(std::size_t id, std::string&& login, std::string&& pass) : _id(id), _login(std::move(login)), _pass(std::move(pass))
{
}

User::~User()
{
}

void User::setId(const std::size_t id) {
    _id = id;
}

void User::setLogin(const std::string &login)
{
    _login = login;
}
void User::setPass(const std::string &pass)
{
    _pass = pass;
}

std::size_t User::getId() const
{
return std::size_t(_id);
}

std::string User::getLogin() const
{
    return _login;
}
std::string User::getPass() const
{
    return _pass;
}
void User::printUser() const
{
    std::cout << _id << ". " <<_login; /*  << std::endl; */
}

// Далее перегрузка операторов
bool operator==(const User &u1, const User &u2)
{
    return u1._login == u2._login && u1._pass == u2._pass;
}
bool operator!=(const User &u1, const User &u2)
{
    return !(u1 == u2);
}

bool operator==(const User &u1, const std::string &u2)
{
    return u1._login == u2;
}
bool operator!=(const User &u1, const std::string &u2)
{
    return !(u1 == u2);
}

bool operator==(const std::string &u1, const User &u2)
{
    return u1 == u2._login;
}
bool operator!=(const std::string &u1, const User &u2)
{
    return !(u1 == u2);
}