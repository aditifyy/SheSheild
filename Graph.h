graph.h


#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

using namespace std;

// Stores information about a road connecting two locations.
struct Edge
{
    int destination;
    int distance;
    int risk;
};

// Manages locations and the roads between them.
class Graph
{
private:
    int vertices;

    // Each location has a list of roads connected to it.
    vector<vector<Edge>> adjacencyList;

    // Stores the name of each location.
    vector<string> locationNames;

public:
    Graph(int vertices);

    void addLocation(int id, string name);

    void addRoad(int source, int destination, int distance, int risk);

    void displayLocations();
    void displayGraph();

    // Give access to the graph and location names.
    vector<vector<Edge>>& getGraph();
    vector<string>& getLocationNames();
};

#endif
