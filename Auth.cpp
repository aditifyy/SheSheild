#include "Auth.h"

#include <iostream>
#include <fstream>

using namespace std;

Auth::Auth(string fileName)
{
    this->fileName = fileName;
}

bool Auth::userExists(string userID)
{
    ifstream file(fileName);

    string storedID;
    string storedPassword;

    while (file >> storedID >> storedPassword)
    {
        if (storedID == userID)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

bool Auth::signUp()
{
    string userID;
    string password;
    string confirmPassword;

    cout << "\n============================================\n";
    cout << "                 SIGN UP\n";
    cout << "============================================\n";

    cout << "Enter User ID: ";
    cin >> userID;

    if (userExists(userID))
    {
        cout << "\nThis User ID already exists.\n";
        cout << "Please choose another User ID.\n";
        return false;
    }

    cout << "Enter Password: ";
    cin >> password;

    cout << "Confirm Password: ";
    cin >> confirmPassword;

    if (password != confirmPassword)
    {
        cout << "\nPasswords do not match.\n";
        return false;
    }

    ofstream file(fileName, ios::app);

    if (!file)
    {
        cout << "\nError opening user file.\n";
        return false;
    }

    file << userID << " "
         << password << endl;

    file.close();

    cout << "\n============================================\n";
    cout << "       ACCOUNT CREATED SUCCESSFULLY\n";
    cout << "============================================\n";

    return true;
}

bool Auth::checkLogin(
    string userID,
    string password)
{
    ifstream file(fileName);

    string storedID;
    string storedPassword;

    while (file >> storedID >> storedPassword)
    {
        if (storedID == userID &&
            storedPassword == password)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

bool Auth::login()
{
    string userID;
    string password;

    cout << "\n============================================\n";
    cout << "                  LOGIN\n";
    cout << "============================================\n";

    cout << "Enter User ID: ";
    cin >> userID;

    cout << "Enter Password: ";
    cin >> password;

    if (checkLogin(userID, password))
    {
        cout << "\n============================================\n";
        cout << "             LOGIN SUCCESSFUL\n";
        cout << "============================================\n";

        cout << "Welcome, " << userID << "!\n";

        return true;
    }

    cout << "\nInvalid User ID or Password.\n";

    return false;
}