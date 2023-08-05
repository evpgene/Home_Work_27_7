#include "SaveRestor.h"
#include "User.h"
#include "Message.h"

SaveRestor::SaveRestor()
{
}

SaveRestor::~SaveRestor()
{
}

void SaveRestor::createPath(const fs::path &Path, const fs::path &savePath)
{
	if (!fs::exists(Path))
		fs::create_directory(Path);
	if (!fs::exists(savePath))
		fs::create_directory(savePath);
	if (fs::exists(savePath))
	{
		fs::permissions(
			savePath,
			fs::perms::owner_all,
			fs::perm_options::add);
		fs::permissions(
			savePath,
			fs::perms::others_all | fs::perms::group_all,
			fs::perm_options::remove);
	}
	else
	{
	}
};

std::string SaveRestor::saveUser(std::shared_ptr<User> user_ptr)
{
	return log + sep + user_ptr->_login + sep + pas + sep + user_ptr->_pass;
};

void SaveRestor::saveUsers(std::vector<std::shared_ptr<User>> &users)
{
	// write
	std::string filename{savePath / "Users"};
	std::fstream s{filename, s.binary | s.trunc | s.in | s.out};

	s.clear();

	if (!s.is_open())
		std::cout << "failed to open " << filename << '\n';
	else
	{
		for (const auto user_ptr : users)
		{
			s << saveUser(user_ptr) << endl;
		}
	}
	s.close();
}

std::shared_ptr<User> SaveRestor::restorUser(std::string &str)
{

	bool nextIsID{false}, doneID{false}, nextIsLogin{false}, doneLogin{false}, nextIsPass{false}, donePass{false};
	std::string word;
	shared_ptr<User> user_ptr = make_shared<User>(User());
	std::istringstream iss(str);
	while (iss >> word)
	{
		//std::cout << word << std::endl; // для диагностики - можно убрать
		if (nextIsLogin)
		{
			user_ptr->setLogin(word);
			nextIsLogin = false;
			doneLogin = true;
		};
		if (nextIsPass)
		{
			user_ptr->setPass(word);
			nextIsPass = false;
			donePass = true;
		};
		if (word == log)
		{
			nextIsLogin = true;
		}
		if (word == pas)
		{
			nextIsPass = true;
		}
	}
	if (doneLogin /*  && donePass */) // donePass может отсутствовать для общего чата
	{
		return user_ptr;
	}
	return nullptr;
}

void SaveRestor::restorUsers(std::vector<std::shared_ptr<User>> &users)
{
	// read
	std::string filename{savePath / "Users"};
	std::fstream s{filename};

	if (!s.is_open())
		std::cout << "failed to open " << filename << '\n';
	else
	{
		s.seekg(0);
		std::cout << "opened " << filename << '\n';
		for (std::string line; std::getline(s, line);)
		{
			//std::cout << line << '\n'; // для диагностики - можно убрать
			shared_ptr<User> restorUser_ptr = restorUser(line);
			if (restorUser_ptr)
				users.push_back(restorUser_ptr);
			else
				std::cout << "User one not restored";
		}
	}
	s.close();
}

std::string SaveRestor::saveMessage(const Message &message)
{
	return timesend + sep + message._timeSend + '\n' + name + sep + message._userName + '\n' + mess + sep + message._message;
};

void SaveRestor::saveChat(std::shared_ptr<Chat> chat_ptr)
{
	// write
	//std::cout << savePathChats / chat_ptr->getChatName() << std::endl; // для диагностики - можно убрать
	std::string filename{savePathChats / chat_ptr->getChatName()};
	std::fstream s{filename, s.binary | s.trunc | s.out};

	s.clear();

	if (!s.is_open())
		std::cout << "failed to open " << filename << '\n';
	else
	{
		for (auto const &message : chat_ptr->_messages)
		{
			//std::cout << saveMessage(message) << std::endl; // для диагностики - можно убрать
			s << saveMessage(message) << endl;
		}
	}
	s.close();
};


void SaveRestor::restorChats(std::vector<std::shared_ptr<Chat>>& chats)
{
	for (auto const &dir_entry : std::filesystem::directory_iterator{savePathChats})
	{
	fs::path filename = dir_entry;	
	//std::string filename{file};
	std::fstream t{filename};

	if (!t.is_open())
		std::cout << "failed to open " << filename << '\n';
	else
	{
		t.seekg(0);
		//std::cout << "opened " << filename << '\n';		   // для диагностики - можно убрать
		//std::cout << "opened " << path.filename() << '\n'; // для диагностики - можно убрать
		std::string isTime, isName, isMessage;

		shared_ptr<Chat> restoringChat = make_shared<Chat>(Chat(dir_entry.path().filename()));
		bool doneTime{false}, doneName{false}, doneMessage{false};
		for (std::string line; std::getline(t, line);)
		{

			std::size_t pos = line.find(name);
			pos = line.find(timesend);
			if (pos != std::string::npos)
			{
				isTime = line.substr(timesend.size() + sep.size());
				doneTime = true;
				// std::cout << "isTime: " << isTime << std::endl;  // для диагностики - можно убрать
			};

			if (pos != std::string::npos)
			{
				isName = line.substr(name.size() + sep.size());
				doneName = true;
				//std::cout << "isName: " << isName << std::endl; // для диагностики - можно убрать
			};

			pos = line.find(mess);
			if (pos != std::string::npos)
			{
				isMessage = line.substr(mess.size() + sep.size());
				doneMessage = true;
				//std::cout << "isMessage: " << isMessage << std::endl; // для диагностики - можно убрать
			};

			if (doneTime && doneName && doneMessage)
			{
				restoringChat->addMessage((Message(isTime, isName, isMessage)));
				chats.push_back(restoringChat);
				doneName = false;
				doneTime = false;
				doneMessage = false;

			}
		}
		t.close();
	}
	}
}

void SaveRestor::saveChats(std::vector<std::shared_ptr<Chat>> chats)
{
	for (const auto chat_ptr : chats)
		saveChat(chat_ptr);
};