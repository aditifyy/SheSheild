#include "Graph.h"
#include <iostream>

using namespace std;

// Create a graph with the given number of locations.
Graph::Graph(int vertices)
{
    this->vertices = vertices;

    adjacencyList.resize(vertices);
    locationNames.resize(vertices);
}

// Save the name of a location at its given ID.
void Graph::addLocation(int id, string name)
{
    if (id >= 0 && id < vertices)
    {
        locationNames[id] = name;
    }
}

// Connect two locations with a road.
// Each road stores its distance and risk level.
void Graph::addRoad(int source, int destination, int distance, int risk)
{
    Edge road;

    road.destination = destination;
    road.distance = distance;
    road.risk = risk;

    adjacencyList[source].push_back(road);

    // Roads can be travelled in both directions.
    Edge reverseRoad;

    reverseRoad.destination = source;
    reverseRoad.distance = distance;
    reverseRoad.risk = risk;

    adjacencyList[destination].push_back(reverseRoad);
}

// Display all available locations and their IDs.
void Graph::displayLocations()
{
    cout << "\n--- Locations ---\n";

    for (int i = 0; i < vertices; i++)
    {
        cout << i << " -> " << locationNames[i] << endl;
    }
}

// Show the roads connected to each location.
void Graph::displayGraph()
{
    cout << "\n--- Road Network ---\n";

    for (int i = 0; i < vertices; i++)
    {
        cout << "\n" << locationNames[i] << ":\n";

        for (const Edge& road : adjacencyList[i])
        {
            cout << "   -> " << locationNames[road.destination]
                 << " | Distance: " << road.distance
                 << " km | Risk: " << road.risk << "/10\n";
        }
    }
}

// Return the graph's adjacency list.
vector<vector<Edge>>& Graph::getGraph()
{
    return adjacencyList;
}

// Return the names of all locations.
vector<string>& Graph::getLocationNames()
{
    return locationNames;
}
