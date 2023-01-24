#include <iostream>
#include <fstream>

using namespace std;

struct UserInput
{
    string Username;
    string Password;
};

class Authentication {
    string authFileName = "auth.dat";
public:
    void Initialize() {
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
                Login();
                break;
            case 2:
                Register();
                break;
            default:
                break;
        }
    }

    void Login() {
        // Login user
    }

    void Register() {
        // Register User.
        UserInput user;
        fstream authFile;
        authFile.open(authFileName,   ios::app | ios::binary);

        cout << "Username: \t";
        cin >> user.Username;
        cout << "Password: \t";
        cin >> user.Password;
        // write to file
        authFile.write((char *) &user, sizeof(UserInput));
    }
};

int main() {
    Authentication auth;
    auth.Initialize();
    return 0;
}
