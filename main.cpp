#include <iostream>
#include<string>
#include <vector>
using namespace std;

/**
* In the name of God
* Homework 2
* TODO: Complete this code using given TODO comments :). then remove TODOs
* feel free to edit code as you want and make it better
* Any questions? ask @devcom_support on telegram
* Good luck!
**/

enum UserType {
	ADMIN,
	MEMBER
};

class Exception {
	virtual string getMessage() const = 0;
};

class UserAlreadyExistsException :public Exception {
public:
	UserAlreadyExistsException(const string& msg) : msg_(msg) {}

	string getMessage() const { return(msg_); }
private:
	string msg_;
};

class PassORUserExecption :public Exception {
public:
	PassORUserExecption(const string& msg) : msg_(msg) {}

	string getMessage() const { return(msg_); }
private:
	string msg_;
};

class AbstractUser { // User structure
public:
	virtual bool authenticate(string username, string password) = 0;
	virtual void deleteAccount(vector<AbstractUser*> *users) = 0;
	string username;
protected:
	string password;
	UserType type;
};


class User : public AbstractUser {
public:

	User(string username, string password, UserType type) {
		this->usernameUser = username;
		this->AbstractUser::username = username;
		this->password = password;
		this->type = type;
	}

	bool authenticate(string username, string password) {
		return this->username == username && this->password == password;
	}

	static User* login(vector<AbstractUser*> *users, string username, string password) {
		for (auto user = users->begin(); user != users->end(); user++) {
			if ((*user)->authenticate(username, password)) {
				return (User*)*user;
			}
		}
		PassORUserExecption ex("Username or Password isn't correct!");
		throw ex;
	}

	static void signup(vector<AbstractUser*> *users, string username, string password) {

		for (auto user = users->begin(); user != users->end(); user++) {
			if ((*user)->username.compare(username) == 0) {
				UserAlreadyExistsException ex("User Already Exists");
				throw ex;
			}
		}
		//Create user and add it to vector
		users->push_back(new User(username, password, UserType::MEMBER));
	}
	void deleteAccount(vector<AbstractUser*> *users) {
		for (auto user = users->begin(); user != users->end(); user++) {
			if ((*user)->username == usernameUser) {
				users->erase(user);
				break;
			}
		}
	}
	string usernameUser;
};

enum MenuState {
	START,
	LOGGED_IN,
	END
};

class AppDatabase { //Just holds runtime data. doesn't save anything
public:
	vector<AbstractUser *> appUsers;

	AppDatabase() { //Load initial data
		appUsers.push_back(new User("admin", "NIMDA", UserType::ADMIN));
	}
};

int main() {
	User * loggedInUser = nullptr;
	AppDatabase appDatabase;
	MenuState menuState = MenuState::START;

	char choice;
	cout << "Welcome!" << endl;

	while (menuState != MenuState::END) {
		switch (menuState) {
		case MenuState::START: {

			cout << "1. login\n2. signup\ne. exit\n";
			cin >> choice;
			switch (choice) {
			case '1': {
				string username, password;
				cout << "Enter Username" << endl;
				cin >> username;
				cout << "Enter Password" << endl;
				cin >> password;
				try {
					loggedInUser = User::login(&appDatabase.appUsers, username, password);
					menuState = MenuState::LOGGED_IN;
				}
				catch (PassORUserExecption e) {
					cout << "Error:" << e.getMessage() << endl;
				}
				break;
			}
			case '2': {
				string username, password;
				cout << "Enter Username" << endl;
				cin >> username;
				cout << "Enter Password" << endl;
				cin >> password;
				try {
					User::signup(&appDatabase.appUsers, username, password);
					cout << "Account successfully created." << endl;
				}
				catch (UserAlreadyExistsException e) {
					cout << "Error:" << e.getMessage() << endl;
				}
				break;
			}
			case 'e': {
				menuState = MenuState::END;
				break;
			}
			default: {
				cout << "Unknown Input" << endl;
			}
			}
			break;
		}
		case MenuState::LOGGED_IN: {
			cout << "d.delete account\nl. logout\ne. exit\n";
			cin >> choice;
			switch (choice) {
			case 'd': {
				loggedInUser->deleteAccount(&appDatabase.appUsers);
				cout << "Account successfully deleted" << endl;
				loggedInUser = nullptr;
				menuState = MenuState::START;
				break;
			}
			case 'l': {
				loggedInUser = nullptr;
				menuState = MenuState::START;
				break;
			}
			case 'e': {
				menuState = MenuState::END;
				break;
			}
			default: {
				cout << "Unknown Input" << endl;
			}
			}
			break;
		}
		}
	}

	return 0;

}

