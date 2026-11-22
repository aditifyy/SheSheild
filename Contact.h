
#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <vector>

using namespace std;

// Stores the basic details of one trusted contact.
class Contact
{
private:
    string name;
    string phone;

public:
    Contact(string name, string phone);

    string getName();
    string getPhone();
};

// Manages the list of trusted contacts.
class ContactManager
{
private:
    vector<Contact> contacts;

public:
    void addContact(string name, string phone);
    void removeContact(string name);
    void displayContacts();

    // Returns the list of saved contacts.
    vector<Contact>& getContacts();
};

#endif
