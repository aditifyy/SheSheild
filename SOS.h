#ifndef SOS_Hyui
#define SOS_H

#include <queue>
#include <string>

using namespace std;

class SOS
{
private:
    queue<string> emergencyQueue;

public:
    void triggerSOS(
        string location
    );

    void processSOS();

    void displayPendingAlerts();
};

#endif
