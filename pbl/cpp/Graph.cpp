#include "Graph.h"

#include <iostream>

using namespace std;

Graph::Graph(int vertices)
{
    this->vertices = vertices;

    adjacencyList.resize(vertices);

    locationNames.resize(vertices);
}

void Graph::addLocation(int id, string name)
{
    if (id >= 0 && id < vertices)
    {
        locationNames[id] = name;
    }
}

void Graph::addRoad(
    int source,
    int destination,
    int distance,
    int risk)
{
    Edge e1;

    e1.destination = destination;
    e1.distance = distance;
    e1.risk = risk;

    adjacencyList[source].push_back(e1);

    Edge e2;

    e2.destination = source;
    e2.distance = distance;
    e2.risk = risk;

    adjacencyList[destination].push_back(e2);
}

void Graph::displayLocations()
{
    cout << "\n========== LOCATIONS ==========\n";

    for (int i = 0; i < vertices; i++)
    {
        cout << i
             << " -> "
             << locationNames[i]
             << endl;
    }
}

void Graph::displayGraph()
{
    cout << "\n========== ROAD NETWORK ==========\n";

    for (int i = 0; i < vertices; i++)
    {
        cout << "\n"
             << locationNames[i]
             << ":\n";

        for (Edge edge : adjacencyList[i])
        {
            cout << "   -> "
                 << locationNames[edge.destination]
                 << " | Distance: "
                 << edge.distance
                 << " km | Risk: "
                 << edge.risk
                 << "/10\n";
        }
    }
}

vector<vector<Edge>>& Graph::getGraph()
{
    return adjacencyList;
}

vector<string>& Graph::getLocationNames()
{
    return locationNames;
}