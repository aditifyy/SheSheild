auth.cpp 


#include "Auth.h"
#include <iostream>
#include <fstream>

using namespace std;

// Store the name of the file used for user details.
Auth::Auth(string fileName)
{
    this->fileName = fileName;
}

// Check if the User ID is already registered.
bool Auth::userExists(string userID)
{
    ifstream file(fileName);

    string savedID, savedPassword;

    while (file >> savedID >> savedPassword)
    {
        if (savedID == userID)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// Register a new user.
bool Auth::signUp()
{
    string userID, password, confirmPassword;

    cout << "\n--- Sign Up ---\n";
    cout << "Enter User ID: ";
    cin >> userID;

    // Do not allow the same User ID twice.
    if (userExists(userID))
    {
        cout << "\nThis User ID is already registered.\n";
        cout << "Please try another one.\n";
        return false;
    }

    cout << "Create Password: ";
    cin >> password;

    cout << "Confirm Password: ";
    cin >> confirmPassword;

    // Both passwords should match.
    if (password != confirmPassword)
    {
        cout << "\nPasswords do not match. Please try again.\n";
        return false;
    }

    // Save the new account without deleting old records.
    ofstream file(fileName, ios::app);

    if (!file)
    {
        cout << "\nCould not open the user data file.\n";
        return false;
    }

    file << userID << " " << password << endl;
    file.close();

    cout << "\nAccount created successfully!\n";
    return true;
}

// Compare the entered details with the saved account details.
bool Auth::checkLogin(string userID, string password)
{
    ifstream file(fileName);

    string savedID, savedPassword;

    while (file >> savedID >> savedPassword)
    {
        if (savedID == userID && savedPassword == password)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// Log in an existing user.
bool Auth::login()
{
    string userID, password;

    cout << "\n--- Login ---\n";
    cout << "Enter User ID: ";
    cin >> userID;

    cout << "Enter Password: ";
    cin >> password;

    if (checkLogin(userID, password))
    {
        cout << "\nLogin successful!\n";
        cout << "Welcome, " << userID << "!\n";
        return true;
    }

    cout << "\nIncorrect User ID or password.\n";
    return false;
}
