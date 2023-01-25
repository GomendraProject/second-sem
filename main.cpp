#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

struct UserInput
{
    char Username[500];
    char Password[500];
};

class Authentication {
    string authFileName = "auth.dat";
public:
    bool Initialize() {
        string username;
        string password;
        int choice;

        cout << "1. Login \n";
        cout << "2. Register as a new user \n";
        cout << "3. Exit \n";
        cout << "Your choice: \t";
        cin >> choice;

        switch (choice) {
            case 1:
                return Login();
            case 2:
                Register();
                break;
            default:
                break;
        }
        return false;
    }

    bool Login() {
        UserInput user, tempUser;
        fstream authFile;
        authFile.open(authFileName, ios::in | ios::binary);
        cout << "Username: \t";
        cin >> user.Username;
        cout << "Password: \t";
        cin >> user.Password;

        while (authFile.read( (char *) &tempUser, sizeof(UserInput))) {
            bool userNameMatched = strcmp(user.Username, tempUser.Username) == 0;
            bool passwordMatched = strcmp(user.Password, tempUser.Password) == 0;
            if(userNameMatched && passwordMatched) {
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
        authFile.open(authFileName,   ios::app | ios::binary);

        cout << "Username: \t";
        cin >> user.Username;
        cout << "Password: \t";
        cin >> user.Password;
        // write to file
        // write ( (char *) &user, sizeof(UserInput) ) sizeof rollNo | sizeof(int)
        authFile.write((char *) &user, sizeof(UserInput));
    }
};


int main() {
    Authentication auth;
    bool loginSuccess = auth.Initialize();
    if(!loginSuccess) {
        std::exit(0);
    }

    cout << "Welcome!!!";
    return 0;
}
