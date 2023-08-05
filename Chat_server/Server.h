#pragma once
#include "Message.h"
#include "User.h"
#include "iostream"
#include "ReceivedData.h"
#include "Keywords.h"
//add new includs
#include <mysql/mysql.h>

class Server
{
private:
    Message_t _message;
    User_t _user;
    

public:
    Server();
    ~Server();

    const Keywords key;

    ReceivedData interpretString(const std::string& str);

    void main();
    User_t retrieveUser(const std::string& str);
    Message_t retrieveMessage(const std::string& msg);
    const std::string getUsernamesEnd() ;
    const std::string getMessagesEnd();
    const std::string getMessageString(const Message& message);
    const std::string getUsernamesString(const std::string& username);

    //new functions for database
    // int createTable_users();
    // int createTable_chats();
    // int createTable_chat_user();
    // int createTable_messages();

};
