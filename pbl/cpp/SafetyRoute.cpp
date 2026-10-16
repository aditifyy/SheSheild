#include "SafetyRoute.h"

#include <iostream>
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

SafetyRoute::SafetyRoute(Graph* graph)
{
    this->graph = graph;
}

void SafetyRoute::findSafestRoute(
    int source,
    int destination)
{
    vector<vector<Edge>>& adj =
        graph->getGraph();

    vector<string>& names =
        graph->getLocationNames();

    int n = adj.size();

    const int INF =
        numeric_limits<int>::max();

    /*
        cost.first  = total risk
        cost.second = total distance

        Safety has higher priority than distance.
    */

    vector<pair<int, int>> cost(
        n,
        {INF, INF}
    );

    vector<int> parent(
        n,
        -1
    );

    /*
        Priority Queue / Min Heap

        It first compares risk.
        If risk is equal, it compares distance.
    */

    priority_queue<
        pair<pair<int, int>, int>,
        vector<pair<pair<int, int>, int>>,
        greater<pair<pair<int, int>, int>>
    > pq;

    cost[source] = {0, 0};

    pq.push({
        {0, 0},
        source
    });

    while (!pq.empty())
    {
        auto current = pq.top();

        pq.pop();

        int currentRisk =
            current.first.first;

        int currentDistance =
            current.first.second;

        int currentNode =
            current.second;

        if (currentNode == destination)
        {
            break;
        }

        for (Edge edge : adj[currentNode])
        {
            int nextNode =
                edge.destination;

            int newRisk =
                currentRisk + edge.risk;

            int newDistance =
                currentDistance + edge.distance;

            pair<int, int> newCost = {
                newRisk,
                newDistance
            };

            if (newCost < cost[nextNode])
            {
                cost[nextNode] =
                    newCost;

                parent[nextNode] =
                    currentNode;

                pq.push({
                    newCost,
                    nextNode
                });
            }
        }
    }

    if (cost[destination].first == INF)
    {
        cout << "\nNo route found.\n";

        return;
    }

    vector<int> route;

    int current = destination;

    while (current != -1)
    {
        route.push_back(current);

        current = parent[current];
    }

    reverse(
        route.begin(),
        route.end()
    );

    cout << "\n====================================\n";
    cout << "         SHESHIELD SAFE ROUTE\n";
    cout << "====================================\n";

    cout << "\nRoute:\n";

    for (int i = 0; i < route.size(); i++)
    {
        cout << names[route[i]];

        if (i != route.size() - 1)
        {
            cout << " -> ";
        }
    }

    cout << "\n\nTotal Safety Risk : "
         << cost[destination].first;

    cout << "\nTotal Distance    : "
         << cost[destination].second
         << " km";

    cout << "\n\nPriority: SAFETY > DISTANCE\n";

    cout << "\n====================================\n";
}