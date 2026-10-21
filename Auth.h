#ifndef AUTH_H
#define AUTH_H

#include <string>

using namespace std;

class Auth
{
private:
    string fileName;

public:
    Auth(string fileName);

    bool signUp();

    bool login();

private:
    bool userExists(string userID);

    bool checkLogin(
        string userID,
        string password
    );
};
htc
#endi
