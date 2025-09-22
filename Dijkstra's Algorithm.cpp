#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <stack>
#include <string>

using namespace std;
const int INF = numeric_limits<int>::max();

unordered_map<string, vector<pair<string, int>>> directedGraph = {
    {"Dorm", {make_pair("Cafeteria", 5), make_pair("Library", 7)}},
    {"Cafeteria", {make_pair("LectureHall", 3)}},
    {"Library", {make_pair("Lab", 4)}},
    {"LectureHall", {make_pair("Gym", 6)}},
    {"Lab", {make_pair("Gym", 2)}},
    {"Gym", {}}
};

unordered_map<string, vector<pair<string, int>>> makeUndirectedGraph(
    const unordered_map<string, vector<pair<string, int>>>& input) {
    unordered_map<string, vector<pair<string, int>>> graph = input;
    for (unordered_map<string, vector<pair<string, int>>>::const_iterator it = input.begin(); it != input.end(); ++it) {
        const string& from = it->first;
        const vector<pair<string, int>>& edges = it->second;
        for (size_t i = 0; i < edges.size(); ++i) {
            const string& to = edges[i].first;
            int weight = edges[i].second;
            graph[to].push_back(make_pair(from, weight));
        }
    }
    return graph;
}

void findShortestPath(const string& start, const string& end,
                      const unordered_map<string, vector<pair<string, int>>>& graph) {
    unordered_map<string, int> cost;
    unordered_map<string, string> prev;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

    for (unordered_map<string, vector<pair<string, int>>>::const_iterator it = graph.begin(); it != graph.end(); ++it)
        cost[it->first] = INF;

    cost[start] = 0;
    pq.push(make_pair(0, start));

    while (!pq.empty()) {
        pair<int, string> top = pq.top(); pq.pop();
        int currCost = top.first;
        string curr = top.second;
        if (currCost > cost[curr]) continue;

        const vector<pair<string, int>>& neighbors = graph.at(curr);
        for (size_t i = 0; i < neighbors.size(); ++i) {
            string next = neighbors[i].first;
            int time = neighbors[i].second;
            int newCost = currCost + time;
            if (newCost < cost[next]) {
                cost[next] = newCost;
                prev[next] = curr;
                pq.push(make_pair(newCost, next));
            }
        }
    }

    if (cost[end] == INF) {
        cout << "No path found from " << start << " to " << end << endl;
        return;
    }

    stack<string> path;
    string current = end;
    while (current != start) {
        path.push(current);
        current = prev[current];
    }
    path.push(start);

    cout << "Shortest travel time: " << cost[end] << " minutes\nRoute: ";
    while (!path.empty()) {
        cout << path.top(); path.pop();
        if (!path.empty()) cout << " -> ";
    }
    cout << endl;
}

int main() {
    string start, end;
    cout << "Enter starting location: "; cin >> start;
    cout << "Enter destination: "; cin >> end;

    unordered_map<string, vector<pair<string, int>>> graph = makeUndirectedGraph(directedGraph);
    if (graph.find(start) == graph.end() || graph.find(end) == graph.end()) {
        cout << "Invalid location. Available places: ";
        for (unordered_map<string, vector<pair<string, int>>>::iterator it = graph.begin(); it != graph.end(); ++it)
            cout << it->first << " ";
        cout << endl;
        return 1;
    }

    findShortestPath(start, end, graph);
    return 0;
}
