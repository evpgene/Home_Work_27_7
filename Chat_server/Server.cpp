#include "Server.h"

Server::Server() {}
Server::~Server() {}

User_t Server::retrieveUser(const std::string& str) {
  bool nextIsLogin{false}, doneLogin{false}, nextIsPass{false}, donePass{false};
  std::string word;
  User_t user = make_shared<User>(User());
  std::istringstream iss(str);
  while (iss >> word) {
    if (nextIsLogin) {
      user->setLogin(word);
      nextIsLogin = false;
      doneLogin = true;
    };
    if (nextIsPass) {
      user->setPass(word);
      nextIsPass = false;
      donePass = true;
    };

    if (word == key.log) {
      nextIsLogin = true;
    }
    if (word == key.pas) {
      nextIsPass = true;
    }
  }
  if (doneLogin && donePass)  // donePass может отсутствовать для общего чата
  {
    return user;
  }
  return nullptr;
}

Message_t Server::retrieveMessage(const std::string& msg) {
  std::string isTime, isName, isMessage;
  std::size_t length{0};
  std::size_t pos_time = msg.find(key.timesend);
  std::size_t pos_name = msg.find(key.name);
  std::size_t pos_message = msg.find(key.mess);
  if (pos_time == std::string::npos || pos_name ==
                     std::string::npos || pos_message == std::string::npos)
    return nullptr;

  pos_time += key.timesend.size() + key.sep.size();
  length = pos_name - pos_time - key.sep.size();
  isTime = msg.substr(pos_time, length);

  pos_name += key.name.size() + key.sep.size();
  length = pos_message - pos_name - key.sep.size();
  isName = msg.substr(pos_name, length);

  pos_message += key.mess.size() + key.sep.size();
  isMessage = msg.substr(pos_message);

  return make_shared<Message>(Message(std::move(isTime), std::move(isName), std::move(isMessage)));
}

ReceivedData Server::interpretString(const std::string& str) {
  std::string_view str_view{str};  // для чего это??
  std::string_view first_word{str_view.substr(0, str_view.find(key.sep))};

  if (first_word == key.itLogon) {
    str_view.remove_prefix(key.itLogon.size() + key.sep.size());
    return ReceivedData(ReceivedType(LOGON), str_view);
  };

  if (first_word == key.itRegistration) {
    str_view.remove_prefix(key.itRegistration.size() + key.sep.size());
    return ReceivedData(ReceivedType(REGISTRATION), str_view);
  };

  if (first_word == key.itMessage) {
    str_view.remove_prefix(key.itMessage.size() + key.sep.size());
    return ReceivedData(ReceivedType(MESSAGE), str_view);
  };

  if (first_word == key.itCompName) {
    str_view.remove_prefix(key.itCompName.size() + key.sep.size());
    return ReceivedData(ReceivedType(COMPANION), str_view);
  };

  if (first_word == key.itGetUsernames) {
    str_view.remove_prefix(key.itGetUsernames.size() + key.sep.size());
    return ReceivedData(ReceivedType(GET_USERNAMES), "getusernames");
  };

  if (first_word == key.itContinueUsernames) {
    str_view.remove_prefix(key.itContinueUsernames.size() + key.sep.size());
    return ReceivedData(ReceivedType(CONTINUE_USERNAMES), "continuegetusernames");
  };

  if (first_word == key.itGetMessages) {
    str_view.remove_prefix(key.itGetMessages.size() + key.sep.size());
    return ReceivedData(ReceivedType(GET_MESSAGES), "getmessages");
  };

  if (first_word == key.itContinueMassages) {
    str_view.remove_prefix(key.itContinueMassages.size() + key.sep.size());
    return ReceivedData(ReceivedType(CONTINUE_MESSAGES), "continuegetmessages");
  };

  if (first_word == key.itLogout) {
    str_view.remove_prefix(key.itLogout.size() + key.sep.size());
    return ReceivedData(ReceivedType(LOGOUT), str_view);
  };

    if (first_word == key.itExit) {
    str_view.remove_prefix(key.itExit.size() + key.sep.size());
    return ReceivedData(ReceivedType(EXIT), str_view);
  };


  

  return ReceivedData(ReceivedType(NOTHING), "nothing");
};

const std::string Server::getUsernamesEnd() { return key.itUsernamesEnd + key.sep; }
const std::string Server::getMessagesEnd() { return key.itMessagesEnd + key.sep; }
const std::string Server::getUsernamesString(const std::string& username) { return key.itUsernames + key.sep + username; }

const std::string Server::getMessageString(const Message& message) {
  return key.itMessages + ' ' + message.getTimeSend() + ' ' + message.getUserLogin() + ' ' +
         message.getMessageText();
};