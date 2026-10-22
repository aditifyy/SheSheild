sos.cpp

#include "SOS.h"

#include <iostream>

using namespace std;

void SOS::triggerSOS(
    string location)
{
    emergencyQueue.push(
        location
    );

    cout << "\n************************************\n";
    cout << "          SOS ACTIVATED\n";
    cout << "************************************\n";

    cout << "Emergency location: "
         << location
         << endl;

    cout << "SOS request added to emergency queue.\n";

    cout << "************************************\n";
}

void SOS::processSOS()
{
    if (emergencyQueue.empty())
    {
        cout << "\nNo pending SOS alerts.\n";

        return;
    }

    cout << "\nProcessing SOS alert...\n";

    cout << "Emergency at: "
         << emergencyQueue.front()
         << endl;

    emergencyQueue.pop();

    cout << "Alert sent to trusted contacts.\n";
}

void SOS::displayPendingAlerts()
{
    if (emergencyQueue.empty())
    {
        cout << "\nNo pending SOS alerts.\n";

        return;
    }

    cout << "\nPending SOS alerts: "
         << emergencyQueue.size()
         << endl;
}
