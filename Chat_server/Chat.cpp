#include "Chat.h"
#include <iostream>
#include <fstream>
Chat::Chat()
{
}

Chat::Chat(const std::size_t id, const std::string &chatname) : _id(id), _chatname(chatname)
{
}

// этот конструктор временно для совместимости с SaveRestor - его нужно удалить
Chat::Chat(const std::string &chatname) : _chatname(chatname)
{

}

Chat::~Chat()
{
}

void Chat::printMessages()
{
	for (const auto& message : _messages)
	{
		message.printMessage();
		std::cout << std::endl;
	}
}

void Chat::addMessage(const Message &message)
{
	_messages.emplace_back(message);
}

void Chat::addMessage(Message &&message)
{
	_messages.emplace_back(std::move(message));
}


void Chat::printChatName() const
{
	std::cout << " " << _chatname << std::endl;
}

std::size_t Chat::getId() const { return std::size_t(_id); }

std::string Chat::getChatName() const
{
	return _chatname;
}


queue_message_t Chat::getLastMessages() {
        std::queue<Message> lastMessages;
        for (const auto& message : _messages) {
                lastMessages.push(message);
                std::cout << "one message push	" << std::endl;
        }
        return std::make_shared<std::queue<Message>>(lastMessages);
}

bool operator==(const Chat &ch1, const Chat &ch2)
{
	return ch1._chatname == ch2._chatname;
}
bool operator!=(const Chat &ch1, const Chat &ch2)
{
	return !(ch1 == ch2);
}


bool operator==(const Chat& ch1, const std::string& ch2)
{
	return ch1._chatname == ch2;
}
bool operator!=(const Chat& ch1, const std::string& ch2)
{
	return !(ch1 == ch2);
}
bool operator==(const std::string& ch1, const Chat& ch2)
{
	return ch1 == ch2._chatname;
}
bool operator!=(const std::string& ch1, const Chat& ch2)
{
	return !(ch1 == ch2);
}