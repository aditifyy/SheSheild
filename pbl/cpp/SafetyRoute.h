#ifndef SAFETYROUTE_H
#define SAFETYROUTE_H

#include "Graph.h"

using namespace std;

class SafetyRoute
{
private:
    Graph* graph;

public:
    SafetyRoute(Graph* graph);

    void findSafestRoute(
        int source,
        int destination
    );
};

#endif