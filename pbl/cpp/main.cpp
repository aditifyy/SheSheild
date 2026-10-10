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


// ============================================
// LOGIN / SIGNUP MENU
// ============================================

void authenticationMenu(Auth& authentication)
{
    int choice;

    while (true)
    {
        cout << "\n\n";
        cout << "============================================\n";
        cout << "                SHESHIELD\n";
        cout << "         WOMEN SAFETY SYSTEM\n";
        cout << "============================================\n";

        cout << "1. Login\n";
        cout << "2. Sign Up\n";
        cout << "0. Exit\n";

        cout << "============================================\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                if (authentication.login())
                {
                    return;
                }

                break;
            }

            case 2:
            {
                authentication.signUp();

                break;
            }

            case 0:
            {
                cout << "\nThank you for using SheShield.\n";

                exit(0);
            }

            default:
            {
                cout << "\nInvalid choice.\n";
            }
        }
    }
}


// ============================================
// EMERGENCY CONTACT MENU
// ============================================

void emergencyContacts(ContactManager& contacts)
{
    int choice;

    do
    {
        cout << "\n============================================\n";
        cout << "       EMERGENCY / TRUSTED CONTACTS\n";
        cout << "============================================\n";

        cout << "1. Add Contact\n";
        cout << "2. View Contacts\n";
        cout << "3. Remove Contact\n";
        cout << "0. Back\n";

        cout << "\nEnter choice: ";
        cin >> choice;

        cin.ignore();

        switch (choice)
        {
            case 1:
            {
                string name;
                string phone;

                cout << "\nEnter contact name: ";
                getline(cin, name);

                cout << "Enter phone number: ";
                getline(cin, phone);

                contacts.addContact(name, phone);

                break;
            }

            case 2:
            {
                contacts.displayContacts();

                break;
            }

            case 3:
            {
                string name;

                cout << "\nEnter contact name to remove: ";
                getline(cin, name);

                contacts.removeContact(name);

                break;
            }

            case 0:
            {
                break;
            }

            default:
            {
                cout << "\nInvalid choice.\n";
            }
        }

    } while (choice != 0);
}


// ============================================
// LIVE LOCATION SHARING
// ============================================

void liveLocationSharing(ContactManager& contacts)
{
    string location;

    cout << "\n============================================\n";
    cout << "           LIVE LOCATION SHARING\n";
    cout << "============================================\n";

    cout << "Enter your current location: ";
    getline(cin, location);

    cout << "\nCurrent Location: "
         << location << endl;

    vector<Contact>& contactList =
        contacts.getContacts();

    if (contactList.empty())
    {
        cout << "\nNo trusted contacts available.\n";
        return;
    }

    cout << "\nSharing location with trusted contacts...\n";

    for (int i = 0; i < contactList.size(); i++)
    {
        cout << "\nLocation shared with: "
             << contactList[i].getName()
             << " - "
             << contactList[i].getPhone();
    }

    cout << "\n\nLocation sharing completed.\n";
}


// ============================================
// MAIN
// ============================================

int main()
{
    // ========================================
    // AUTHENTICATION
    // ========================================

    Auth authentication("../data/users.txt");

    authenticationMenu(authentication);


    // ========================================
    // CREATE GRAPH
    // ========================================

    Graph graph(7);

    graph.addLocation(0, "College Gate");
    graph.addLocation(1, "Main Market");
    graph.addLocation(2, "City Mall");
    graph.addLocation(3, "Bus Stand");
    graph.addLocation(4, "Railway Station");
    graph.addLocation(5, "Hospital");
    graph.addLocation(6, "Safe Shelter");


    // ========================================
    // ADD ROADS
    // ========================================

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


    // ========================================
    // CREATE OBJECTS
    // ========================================

    SafetyRoute safetyRoute(&graph);

    ContactManager contacts;

    SOS sosSystem;


    // ========================================
    // DEFAULT EMERGENCY CONTACTS
    // ========================================

    contacts.addContact(
        "Mother",
        "9876543210"
    );

    contacts.addContact(
        "Father",
        "9876501234"
    );


    int choice;


    // ========================================
    // MAIN SHESHIELD MENU
    // ========================================

    do
    {
        cout << "\n\n";
        cout << "============================================\n";
        cout << "                SHESHIELD\n";
        cout << "         WOMEN SAFETY SYSTEM\n";
        cout << "============================================\n";

        cout << "1. Safe Navigation\n";
        cout << "2. SOS Alert\n";
        cout << "3. Live Location Sharing\n";
        cout << "4. Emergency / Trusted Contacts\n";
        cout << "0. Logout / Exit\n";

        cout << "============================================\n";

        cout << "Enter your choice: ";
        cin >> choice;

        cin.ignore();


        switch (choice)
        {
            // =================================
            // SAFE NAVIGATION
            // =================================

            case 1:
            {
                int source;
                int destination;

                cout << "\n============================================\n";
                cout << "              SAFE NAVIGATION\n";
                cout << "============================================\n";

                graph.displayLocations();

                cout << "\nEnter your current location ID: ";
                cin >> source;

                cout << "Where do you want to go? ";
                cin >> destination;

                if (source < 0 || source >= 7 ||
                    destination < 0 || destination >= 7)
                {
                    cout << "\nInvalid location ID.\n";
                }
                else
                {
                    safetyRoute.findSafestRoute(
                        source,
                        destination
                    );
                }

                break;
            }


            // =================================
            // SOS
            // =================================

            case 2:
            {
                string location;

                cout << "\n============================================\n";
                cout << "                 SOS ALERT\n";
                cout << "============================================\n";

                cout << "Enter your current location: ";
                getline(cin, location);

                sosSystem.triggerSOS(location);

                cout << "\nEmergency contacts:\n";

                contacts.displayContacts();

                break;
            }


            // =================================
            // LIVE LOCATION
            // =================================

            case 3:
            {
                liveLocationSharing(contacts);

                break;
            }


            // =================================
            // CONTACTS
            // =================================

            case 4:
            {
                emergencyContacts(contacts);

                break;
            }


            // =================================
            // LOGOUT / EXIT
            // =================================

            case 0:
            {
                cout << "\nLogging out...\n";
                cout << "Thank you for using SheShield.\n";

                break;
            }


            default:
            {
                cout << "\nInvalid choice.\n";
            }
        }

    } while (choice != 0);


    return 0;
}