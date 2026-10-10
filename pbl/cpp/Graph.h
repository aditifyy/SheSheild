#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

using namespace std;

struct Edge
{
    int destination;
    int distance;
    int risk;
};

class Graph
{
private:
    int vertices;

    vector<vector<Edge>> adjacencyList;

    vector<string> locationNames;

public:
    Graph(int vertices);

    void addLocation(int id, string name);

    void addRoad(
        int source,
        int destination,
        int distance,
        int risk
    );

    void displayLocations();

    void displayGraph();

    vector<vector<Edge>>& getGraph();

    vector<string>& getLocationNames();
};

#endif