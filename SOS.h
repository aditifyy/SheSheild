sos.h


#ifndef SAFETYROUTE_H
#define SAFETYROUTE_H

#include "Graph.h"

using namespace std;

// Finds the safest route between two locations.
class SafetyRoute
{
private:
    Graph* graph;

public:
    // Use the graph to calculate a safe route.
    SafetyRoute(Graph* graph);

    void findSafestRoute(int source, int destination);
};

#endif
