#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <stack>
#include <string>

using namespace std;

const int INF = numeric_limits<int>::max();

// directed graph
unordered_map<string, vector<pair<string, int>>> directedGraph = {
    {"Dorm", {make_pair("Cafeteria", 5), make_pair("Library", 7)}},
    {"Cafeteria", {make_pair("LectureHall", 3)}},
    {"Library", {make_pair("Lab", 4)}},
    {"LectureHall", {make_pair("Gym", 6)}},
    {"Lab", {make_pair("Gym", 2)}},
    {"Gym", {}}
};

// Converts directed graph to undirected by adding reverse edges
unordered_map<string, vector<pair<string, int>>> makeUndirectedGraph(
    const unordered_map<string, vector<pair<string, int>>>& inputGraph) {
    
    unordered_map<string, vector<pair<string, int>>> undirectedGraph = inputGraph;

    for (const auto& entry : inputGraph) {
        const string& from = entry.first;
        for (const auto& edge : entry.second) {
            const string& to = edge.first;
            int weight = edge.second;
            undirectedGraph[to].push_back(make_pair(from, weight));
        }
    }

    return undirectedGraph;
}

void findShortestPath(const string& start, const string& end,
                      const unordered_map<string, vector<pair<string, int>>>& graph) {
    unordered_map<string, int> shortestCost;
    unordered_map<string, string> cameFrom;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> nextStops;

    for (unordered_map<string, vector<pair<string, int>>>::const_iterator it = graph.begin(); it != graph.end(); ++it) {
        shortestCost[it->first] = INF;
    }

    shortestCost[start] = 0;
    nextStops.push(make_pair(0, start));

    while (!nextStops.empty()) {
        pair<int, string> top = nextStops.top();
        nextStops.pop();
        int currentCost = top.first;
        string currentLocation = top.second;

        if (currentCost > shortestCost[currentLocation]) continue;

        vector<pair<string, int>> neighbors = graph.at(currentLocation);
        for (size_t i = 0; i < neighbors.size(); ++i) {
            string neighbor = neighbors[i].first;
            int travelTime = neighbors[i].second;
            int newCost = currentCost + travelTime;

            if (newCost < shortestCost[neighbor]) {
                shortestCost[neighbor] = newCost;
                cameFrom[neighbor] = currentLocation;
                nextStops.push(make_pair(newCost, neighbor));
            }
        }
    }

    if (shortestCost[end] == INF) {
        cout << "No path found from " << start << " to " << end << endl;
        return;
    }

    // Reconstruct path
    stack<string> path;
    string current = end;
    while (current != start) {
        path.push(current);
        current = cameFrom[current];
    }
    path.push(start);

    cout << "Shortest travel time: " << shortestCost[end] << " minutes" << endl;
    cout << "Route: ";
    while (!path.empty()) {
        cout << path.top();
        path.pop();
        if (!path.empty()) cout << " -> ";
    }
    cout << endl;
}

int main() {
    string start, end;
    cout << "Enter starting location (e.g., Dorm): ";
    cin >> start;
    cout << "Enter destination (e.g., Gym): ";
    cin >> end;

    // Convert to undirected graph
    unordered_map<string, vector<pair<string, int>>> graph = makeUndirectedGraph(directedGraph);

    if (graph.find(start) == graph.end() || graph.find(end) == graph.end()) {
        cout << "Invalid location. Available places: ";
        for (unordered_map<string, vector<pair<string, int>>>::iterator it = graph.begin(); it != graph.end(); ++it) {
            cout << it->first << " ";
        }
        cout << endl;
        return 1;
    }

    findShortestPath(start, end, graph);
    return 0;
}
