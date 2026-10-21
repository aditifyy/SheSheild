#include "Contact.h"
#include <iostream>
using namespace std;
Contact::Contact(
    string name,
    string phone)
{
    this->name = name;

    this->phone = phone;
}
string Contact::getName()
{
    return name;
}
string Contact::getPhone()
{
    return phone;
}
void ContactManager::addContact(
    string name,
    string phone)
{
    contacts.push_back(
        Contact(name, phone)
    );

    cout << "\nContact added successfully.\n";
}
void ContactManager::removeContact(
    string name)
{
    for (int i = 0;
         i < contacts.size();
         i++)
    {
        if (contacts[i].getName() == name)
        {
            contacts.erase(
                contacts.begin() + i
            );

            cout << "\nContact removed.\n";

            return;
        }
    }
    cout << "\nContact not found.\n";
}
void ContactManager::displayContacts()
{
 cout << "\n TRUSTED CONTACTS \n";
 if (contacts.empty())
    {
    cout << "No trusted contacts available.\n";

        return;
    }
    for (int i = 0; i < contacts.size();i++)
    {
        cout << i + 1
             << ". "
             << contacts[i].getName()
             << " | "
             << contacts[i].getPhone()
             << endl;
    }
}
vector<Contact>& ContactManager::getContacts()
{
    return contacts;
}