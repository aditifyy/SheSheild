
#include "Contact.h"
#include <iostream>

using namespace std;

// Create a contact using the person's name and phone number.
Contact::Contact(string name, string phone)
{
    this->name = name;
    this->phone = phone;
}

string Contact::getName()
{
    returnname;
}

string Contact::getPhone()
{
    return phone;
}

// Add a new person to the trusted contacts list.
void ContactManager::addContact(string name, string phone)
{
    Contact newContact(name, phone);
    contacts.push_back(newContact);

    cout << "\nContact added successfully!\n";
}

// Remove a contact by searching for their name.
void ContactManager::removeContact(string name)
{
    for (int i = 0; i < contacts.size(); i++)
    {
        if (contacts[i].getName() == name)
        {
            contacts.erase(contacts.begin() + i);

            cout << "\nContact removed successfully!\n";
            return;
        }
    }

    cout << "\nContact not found. Please check the name.\n";
}

// Show all the contacts saved by the user.
void ContactManager::displayContacts()
{
    cout << "\n--- Trusted Contacts ---\n";

    if (contacts.empty())
    {
        cout << "No trusted contacts added yet.\n";
        return;
    }

    for (int i = 0; i < contacts.size(); i++)
    {
        cout << i + 1 << ". "
             << contacts[i].getName()
             << " - "
             << contacts[i].getPhone()
             << endl;
    }
}

// Give access to the saved contacts list.
vector<Contact>& ContactManager::getContacts()
{
    return contacts;
}
