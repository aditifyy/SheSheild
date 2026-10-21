#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "Auth.h"
#include "Graph.h"
#include "SafetyRoute.h"
#include "Contact.h"
#include "SOS.h"
using namespace std;

// Login or Signup
void authenticationMenu(Auth& authentication)
{
    int choice;
    while (true)
    {
        cout << "\n...SHESHIELD...\n";
        cout << "Women Safety Navigation System\n\n";
        cout << "1.Login\n";
        cout << "2.Sign Up\n";
        cout << "3.Exit\n";
        cout << "\nENTER YOUR CHOICE: ";
        cin >> choice;
        switch (choice)
        {
            case 1:
                if (authentication.login())
                    return;
                break;

            case 2:
                authentication.signUp();
                break;

            case 0:
                cout << "\nBye! Stay safe.\n";
                exit(0);

            default:
                cout << "\nOOPS Wrong choice. Try again.\n";
        }
    }
}
// Emergency contacts
void emergencyContacts(ContactManager& contacts)
{
    int choice;
    do
    {
        cout << "\n--- EMERGENCY CONTACTS ---\n";
        cout << "1.Add contact\n";
        cout << "2.View contacts\n";
        cout << "3.Remove contact\n";
        cout << "0.Back\n";
        cout << "\nChoice: ";
        cin >> choice;
        cin.ignore();
        switch (choice)
        {
            case 1:
            {
                string name, phone;

                cout << "Contact name: ";
                getline(cin, name);

                cout << "Phone number: ";
                getline(cin, phone);
                contacts.addContact(name, phone);
                break;
            }
            case 2:
                contacts.displayContacts();
                break;
            case 3:
            {
                string name;
                cout << "Name to remove: ";
                getline(cin, name);

                contacts.removeContact(name);
                break;
            } 
            case 0:
                break;

            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}
// for location sharing

void liveLocationSharing(ContactManager& contacts)
{
    string location;
    cout << "\n---LOCATION SHARING---\n";
    cout << "Enter current location: ";
    getline(cin, location);
    cout << "\nLocation: " << location << endl;
    vector<Contact>& contactList = contacts.getContacts();
    if (contactList.empty())
    {
        cout << "No contacts added yet.\n";
        return;
    }
    cout << "\nSharing with these :\n";
    for (int i = 0; i < contactList.size(); i++)
    {
        cout << contactList[i].getName()
             << "-" << contactList[i].getPhone() << endl;
    }
    cout << "\nCOMPLETED.\n";
}
int main()
{
    // Login first
  Auth authentication("../data/users.txt");
    authenticationMenu(authentication);
    // Locations and roads
    Graph graph(7);
    graph.addLocation(0, "College Gate");
    graph.addLocation(1, "Main Market");
    graph.addLocation(2, "City Mall");
    graph.addLocation(3, "Bus Stand");
    graph.addLocation(4, "Railway Station");
    graph.addLocation(5, "Hospital");
    graph.addLocation(6, "Safe Shelter");

    graph.addRoad(0, 1, 2, 8);
    graph.addRoad(0, 2, 5, 2);
    graph.addRoad(1, 2, 2, 7);
    graph.addRoad(1, 3, 3, 8);
    graph.addRoad(2, 3, 3, 2);
    graph.addRoad(2, 5, 4, 1);
    graph.addRoad(3, 4, 2, 9);
    graph.addRoad(4, 5, 3, 7);
    graph.addRoad(5, 6, 2, 1);
    graph.addRoad(3, 6, 5, 3);
    // Objects
    SafetyRoute safetyRoute(&graph);
    ContactManager contacts;
    SOS sosSystem;
    // Default emergency contacts
    contacts.addContact("Mom", "9876543210");
    contacts.addContact("Dad", "9876501234");


    int choice;
    do
    {
        cout << "\n...SHESHIELD...\n\n";
        cout << "1.Safe Navigation\n";
        cout << "2.SOS Alert\n";
        cout << "3.Share Location\n";
        cout << "4.Emergency Contacts\n";
        cout << "0.Exit\n";
        cout << "\nEnter Choice: ";
        cin >> choice;
        cin.ignore();
        switch (choice)
        {
            case 1:
            {
                int source, destination;

                graph.displayLocations();

                cout << "\nCurrent location ID: ";
                cin >> source;

                cout << "Destination ID: ";
                cin >> destination;

                if (source < 0 || source >= 7 ||
                    destination < 0 || destination >= 7)
                {
                    cout << "Invalid location ID.\n";
                }
                else
                {
                    safetyRoute.findSafestRoute(source, destination);
                }
                break;
            }
            case 2:
            {
                string location;
                cout << "\n!!!SOS!!!\n";
                cout << "Enter current location: ";
                getline(cin, location);
                sosSystem.triggerSOS(location);
                cout << "\nEmergency contacts:\n";
                contacts.displayContacts();
                break;
            }
            case 3:
                liveLocationSharing(contacts);
                break;
            case 4:
                emergencyContacts(contacts);
                break;
            case 0:
                cout << "\nLogging out.Stay safe!\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}