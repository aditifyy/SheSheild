
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>

#include "Auth.h"
#include "Graph.h"
#include "SafetyRoute.h"
#include "Contact.h"
#include "SOS.h"

using namespace std;

// Show the login and signup options.
void authenticationMenu(Auth& authentication)
{
    int choice;

    while (true)
    {
        cout << "\n===== SheShield =====\n";
        cout << "1. Login\n";
        cout << "2. Sign Up\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
                if (authentication.login())
                {
                    return;
                }
                break;

            case 2:
                authentication.signUp();
                break;

            case 0:
                cout << "Thank you for using SheShield.\n";
                exit(0);

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Let users add, view or remove trusted contacts.
void emergencyContacts(ContactManager& contacts)
{
    int choice;

    do
    {
        cout << "\n--- Trusted Contacts ---\n";
        cout << "1. Add Contact\n";
        cout << "2. View Contacts\n";
        cout << "3. Remove Contact\n";
        cout << "0. Back\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
            case 1:
            {
                string name, phone;

                cout << "Enter contact name: ";
                getline(cin, name);

                cout << "Enter phone number: ";
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

                cout << "Enter the name to remove: ";
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

// Demonstrate location sharing with saved contacts.
void liveLocationSharing(ContactManager& contacts)
{
    string location;

    cout << "\n--- Location Sharing ---\n";
    cout << "Enter your current location: ";
    getline(cin, location);

    cout << "\nCurrent location: " << location << '\n';

    vector<Contact>& contactList = contacts.getContacts();

    if (contactList.empty())
    {
        cout << "No trusted contacts have been added yet.\n";
        return;
    }

    cout << "\nLocation sharing demonstration:\n";

    for ( Contact& contact : contactList)
    {
        cout << "Contact: " << contact.getName()
             << " | Phone: " << contact.getPhone() << '\n';
    }

    cout << "\nThis is a demonstration; no actual location was sent.\n";
}

int main()
{
    // Ask the user to log in or create an account.
    Auth authentication("../data/users.txt");
    authenticationMenu(authentication);

    // Create the locations used in the route demonstration.
    Graph graph(7);

    graph.addLocation(0, "College Gate");
    graph.addLocation(1, "Main Market");
    graph.addLocation(2, "City Mall");
    graph.addLocation(3, "Bus Stand");
    graph.addLocation(4, "Railway Station");
    graph.addLocation(5, "Hospital");
    graph.addLocation(6, "Safe Shelter");

    // Add roads with their distance and risk values.
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

    // Set up the project's main features.
    SafetyRoute safetyRoute(&graph);
    ContactManager contacts;
    SOS sosSystem;

    // Add sample contacts for testing.
    contacts.addContact("Mother", "9876543210");
    contacts.addContact("Father", "9876501234");

    int choice;

    do
    {
        cout << "\n===== SheShield: Women Safety System =====\n";
        cout << "1. Safe Navigation\n";
        cout << "2. SOS Alert\n";
        cout << "3. Live Location Sharing\n";
        cout << "4. Trusted Contacts\n";
        cout << "0. Logout / Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
            case 1:
            {
                int source, destination;

                graph.displayLocations();

                cout << "\nEnter your current location ID: ";
                if (!(cin >> source))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid location ID.\n";
                    break;
                }

                cout << "Enter your destination ID: ";
                if (!(cin >> destination))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid location ID.\n";
                    break;
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (source < 0 || source >= 7 ||
                    destination < 0 || destination >= 7)
                {
                    cout << "Please enter a valid location ID.\n";
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

                cout << "\n--- SOS Alert ---\n";
                cout << "Enter your current location: ";
                getline(cin, location);

                sosSystem.triggerSOS(location);

                cout << "\nSaved emergency contacts:\n";
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
                cout << "Thank you for using SheShield.\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
