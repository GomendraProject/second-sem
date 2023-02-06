#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

struct UserInput {
    char Username[500];
    char Password[500];
};

class UserNotFoundException {

};

class Authentication {
    string authFileName = "auth.dat";
public:
    bool Initialize() {
        while (true) {
            int choice;
            bool isSuccess;

            cout << "1. Login \n";
            cout << "2. Register as a new user \n";
            cout << "3. Exit \n";
            cout << "Your choice: \t";
            cin >> choice;

            switch (choice) {
                case 1:
                    isSuccess = Login();
                    if (isSuccess) {
                        return true;
                    } else {
                        cout << "Invalid login credentials" << endl;
                    }
                    break;
                case 2:
                    Register();
                    break;
                default:
                    break;
            }
            char shouldContinue;
            cout << "Press Y/y to continue. Any other key to exit: \t";
            cin >> shouldContinue;
            if (shouldContinue != 'Y' && shouldContinue != 'y') {
                return false;
            }
        }
    }

    bool Login() {
        UserInput user, tempUser;
        fstream authFile;
        authFile.open(authFileName, ios::in | ios::binary);
        cout << "Username: \t";
        cin >> user.Username;
        cout << "Password: \t";
        cin >> user.Password;

        while (authFile.read((char *) &tempUser, sizeof(UserInput))) {
            bool userNameMatched = strcmp(user.Username, tempUser.Username) == 0;
            bool passwordMatched = strcmp(user.Password, tempUser.Password) == 0;
            if (userNameMatched && passwordMatched) {
                return true;
            }
        }
        return false;
    }

    void Register() {
        // Register User.
        UserInput user;
        fstream authFile; // fstream => file stream
        // open ( fileName
        authFile.open(authFileName, ios::app | ios::binary);

        cout << "Username: \t";
        cin >> user.Username;
        cout << "Password: \t";
        cin >> user.Password;
        // write to file
        // write ( (char *) &user, sizeof(UserInput) ) sizeof rollNo | sizeof(int)
        authFile.write((char *) &user, sizeof(UserInput));
    }
};

class Application {
    string authFileName = "auth.dat";

protected:

    void DisplayPassword(int length) {
        for (int i = 0; i < length; i++) {
            cout << "*";
        }
    }

    void DisplayUsers() {
        cout << "S.N \t Username \t Password \n";
        cout << "********************************************************* \n";
        fstream file;
        UserInput user{};
        file.open(authFileName, ios::in | ios::binary);
        int count = 0;
        while (file.read((char *) &user, sizeof(UserInput))) {
            int passwordLength = strlen(user.Password);
            cout << ++count << "\t" << user.Username << "\t";
            DisplayPassword(passwordLength);
            cout << endl;
        }
    }

    void UpdateUser() {
        UserInput userInput, fileData;
        bool found = false;
        cout << "Enter username: \t";
        cin >> userInput.Username;
        cout << "Enter new password: \t";
        cin >> userInput.Password;

        fstream authFile, tempFile;

        authFile.open(authFileName, ios::in | ios::binary);
        tempFile.open("temp.dat", ios::out | ios::binary | ios::trunc);
        while (authFile.read((char *) &fileData, sizeof(UserInput))) {
            if (::strcmp(fileData.Username, userInput.Username) == 0) {
                found = true;
                tempFile.write((char *) &userInput, sizeof(UserInput));
            } else {
                tempFile.write((char *) &fileData, sizeof(UserInput));
            }
        }
        authFile.close();
        tempFile.close();
        ::remove(authFileName.c_str());
        ::rename("temp.dat", authFileName.c_str());
        if (found) {
            cout << "Record Updated" << endl;
        } else {
            cout << "No matching record found" << endl;
        }
    }

    void DeleteUser() {
        UserInput userInput, fileData;
        bool found = false;
        cout << "Enter username: \t";
        cin >> userInput.Username;

        fstream authFile, tempFile;

        authFile.open(authFileName, ios::in | ios::binary);
        tempFile.open("temp.dat", ios::out | ios::binary | ios::trunc);
        while (authFile.read((char *) &fileData, sizeof(UserInput))) {
            if (::strcmp(fileData.Username, userInput.Username) != 0) {
                tempFile.write((char *) &fileData, sizeof(UserInput));
            } else {
                found = true;
            }
        }
        authFile.close();
        tempFile.close();
        ::remove(authFileName.c_str());
        ::rename("temp.dat", authFileName.c_str());
        if (found) {
            cout << "Record deleted" << endl;
        } else {
            cout << "No matching record found";
        }
    }

    /*
     * Vector Section
     */

    vector<UserInput> GetUsers() {
        vector<UserInput> users{};
        fstream authFile;
        authFile.open(authFileName, ios::in | ios::binary);
        UserInput input;
        while (authFile.read((char *) &input, sizeof(UserInput))) {
            users.push_back(input);
        }
        return users;
    }

    UserInput GetUser(char username[50]) {
        vector<UserInput> users = GetUsers();
        for (UserInput user: users) {
            if (::strcmp(user.Username, username) == 0) return user;
        }
        throw UserNotFoundException();
    }

    void UpdateUserVector() {
        UserInput userInput;
        bool found = false;
        cout << "Enter username: \t";
        cin >> userInput.Username;
        UserInput fromFile;
        vector<UserInput> users;
        try {
            fromFile = GetUser(userInput.Username);
        }
        catch (UserNotFoundException e) {
            cout << "Error: No matching Record found";
            return;
        }

        cout << "************************ From File ****************" << endl;

        cout << "Username: " << fromFile.Username << endl;
        cout << "Password: ";
        DisplayPassword(::strlen(fromFile.Password));
        cout << endl;
        cout << "**************************************************" << endl;

        cout << "Enter new password: \t";
        cin >> userInput.Password;

        fstream authFile, tempFile;
        users = GetUsers();

        tempFile.open("temp.dat", ios::out | ios::binary | ios::trunc);
        for (UserInput user: users) {
            if (::strcmp(user.Username, userInput.Username) == 0) {
                tempFile.write((char *) &userInput, sizeof(UserInput));
            } else {
                tempFile.write((char *) &user, sizeof(UserInput));
            }
        }
        tempFile.close();
        ::remove(authFileName.c_str());
        ::rename("temp.dat", authFileName.c_str());
        cout << "Record Updated" << endl;
    }

    void DeleteUserVector() {
        UserInput userInput;
        cout << "Enter username: \t";
        cin >> userInput.Username;
        UserInput fromFile;
        vector<UserInput> users;
        try {
            fromFile = GetUser(userInput.Username);
        }
        catch (UserNotFoundException e) {
            cout << "Error: No matching record found";
            return;
        }

        cout << "************************ From File ****************" << endl;

        cout << "Username: " << fromFile.Username << endl;
        cout << "Password: ";
        DisplayPassword(::strlen(fromFile.Password));
        cout << endl;
        cout << "**************************************************" << endl;

        fstream authFile, tempFile;
        users = GetUsers();

        tempFile.open("temp.dat", ios::out | ios::binary | ios::trunc);
        for (UserInput user: users) {
            if (::strcmp(user.Username, userInput.Username) != 0) {
                tempFile.write((char *) &user, sizeof(UserInput));
            } else {
            }
        }
        tempFile.close();
        ::remove(authFileName.c_str());
        ::rename("temp.dat", authFileName.c_str());
        cout << "Record Deleted" << endl;
    }

    /**
     * End Vector Section
     */


public:
    void Initialize() {
        while (true) {
            char shouldContinue;
            int choice;

            cout << "---------------" << endl;

            cout << "1. List Users" << endl;
            cout << "2. Update User" << endl;
            cout << "3. Delete User" << endl;
            cout << "4. Update User (Vector)" << endl;
            cout << "5. Delete User (Vector)" << endl;

            cin >> choice;

            switch (choice) {
                case 1: {
                    DisplayUsers();
                    break;
                }
                case 2: {
                    UpdateUser();
                    break;
                }
                case 3: {
                    DeleteUser();
                    break;
                }
                case 4: {
                    UpdateUserVector();
                    break;
                }
                case 5 : {
                    DeleteUserVector();
                    break;
                }
                default:
                    break;
            }

            cout << endl << "Press Y/y to continue. Any other key to exit: \t";
            cin >> shouldContinue;
            if (shouldContinue != 'Y' && shouldContinue != 'y') {
                break;
            }
        }
    }
};


int main() {
    Authentication auth;
    bool loginSuccess = auth.Initialize();
    if (!loginSuccess) {
        std::exit(0);
    }
    Application app;
    app.Initialize();
    return 0;
}
