#ifndef CONTACT_H
#define CONTACT_H
#include <string>
#include <vector>
using namespace std;
class Contact
{
private:
 string name;
 string phone;
public:
 Contact(
    string name,
     string phone
    );
 string getName();
 string getPhone();
};
class ContactManager
{
private:
    vector<Contact> contacts;
public:
void addContact(
     string name,
     string phone
);
   void removeContact(
    string name
);
 void displayContacts();
    vector<Contact>& getContacts();
};
#endif