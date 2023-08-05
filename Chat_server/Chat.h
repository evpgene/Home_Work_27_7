#pragma once
#include <string>
#include <vector>
#include <queue>
#include "Message.h"
#include "SaveRestor.h"

using User_t = std::shared_ptr<User>;  // указатель на юзера
using Chat_t = std::shared_ptr<Chat>;  // указатель на чат
using Message_t = std::shared_ptr<Message>;  // указатель на сообщение
using queue_message_t = std::shared_ptr<std::queue<Message>>;
using queue_user_t = std::shared_ptr<std::queue<User>>;

class Chat
{
public:
	Chat();
	Chat(const std::size_t id, const std::string &chatname);
	Chat(const std::string &chatname); // этот конструктор временно для совместимости с SaveRestor - его нужно удалить
	~Chat();
	void printMessages();
	void addMessage(const Message &message);
	void addMessage(Message &&message);
	void printChatName() const;
	std::size_t getId() const;
	std::string getChatName() const;

	queue_message_t getLastMessages();

	friend void SaveRestor::saveChat(std::shared_ptr<Chat> chat);
	friend void SaveRestor::restorChats(std::vector<std::shared_ptr<Chat>> &chats);

	friend bool operator==(const Chat& ch1, const Chat& ch2);
	friend bool operator!=(const Chat& ch1, const Chat& ch2);

	friend bool operator==(const Chat& ch1, const std::string& ch2);
	friend bool operator!=(const Chat& ch1, const std::string& ch2);

	friend bool operator==(const std::string& ch1, const Chat& ch2);
	friend bool operator!=(const std::string& ch1, const Chat& ch2);


protected:
	std::size_t _id;
	std::string _chatname;
	std::vector<Message> _messages;
};
