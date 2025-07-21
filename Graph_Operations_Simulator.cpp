#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

class Graph {
    unordered_map<int, list<pair<int, int>>> adj;
    bool directed;
    bool weighted;

public:
    Graph(bool dir, bool wt) : directed(dir), weighted(wt) {}

    void addEdge(int u, int v, int w = 1) {
        adj[u].push_back({v, w});
        if (!directed) {
            adj[v].push_back({u, w});
        }
    }

    void deleteEdge(int u, int v) {
        adj[u].remove_if([v](pair<int, int> p) { return p.first == v; });
        if (!directed) {
            adj[v].remove_if([u](pair<int, int> p) { return p.first == u; });
        }
    }

    void viewGraph() {
        for (auto& node : adj) {
            cout << node.first << ": ";
            for (auto& edge : node.second) {
                cout << "(" << edge.first << ", " << edge.second << ") ";
            }
            cout << endl;
        }
    }

    void convertWeightedToUnweighted() {
        if (weighted) {
            for (auto& pair : adj) {
                for (auto& edge : pair.second) {
                    edge.second = 1;
                }
            }
            weighted = false;
        }
    }

    void convertDirectedToUndirected() {
        if (directed) {
            unordered_map<int, list<pair<int, int>>> newAdj = adj;
            for (auto& pair : adj) {
                for (auto& edge : pair.second) {
                    newAdj[edge.first].push_back({pair.first, edge.second});
                }
            }
            adj = newAdj;
            directed = false;
        }
    }

    void BFS(int start) {
        unordered_map<int, bool> visited;
        queue<int> q;
        q.push(start);
        visited[start] = true;
        cout << "BFS: ";
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            cout << node << " ";
            for (auto& neighbour : adj[node]) {
                if (!visited[neighbour.first]) {
                    q.push(neighbour.first);
                    visited[neighbour.first] = true;
                }
            }
        }
        cout << endl;
    }

    void DFSUtil(int node, unordered_map<int, bool>& visited) {
        visited[node] = true;
        cout << node << " ";
        for (auto& neighbour : adj[node]) {
            if (!visited[neighbour.first]) {
                DFSUtil(neighbour.first, visited);
            }
        }
    }

    void DFS(int start) {
        unordered_map<int, bool> visited;
        cout << "DFS: ";
        DFSUtil(start, visited);
        cout << endl;
    }

    bool hasPath(int u, int v) {
        unordered_map<int, bool> visited;
        return dfsPath(u, v, visited);
    }

    bool dfsPath(int u, int v, unordered_map<int, bool>& visited) {
        if (u == v) return true;
        visited[u] = true;
        for (auto& neighbour : adj[u]) {
            if (!visited[neighbour.first]) {
                if (dfsPath(neighbour.first, v, visited)) return true;
            }
        }
        return false;
    }

    void allPathsUtil(int u, int v, vector<bool>& visited, vector<int>& path, vector<vector<int>>& paths) {
        visited[u] = true;
        path.push_back(u);
        if (u == v) {
            paths.push_back(path);
        } else {
            for (auto& neighbour : adj[u]) {
                if (!visited[neighbour.first]) {
                    allPathsUtil(neighbour.first, v, visited, path, paths);
                }
            }
        }
        path.pop_back();
        visited[u] = false;
    }

    vector<vector<int>> findAllPaths(int u, int v, int n) {
        vector<bool> visited(n, false);
        vector<int> path;
        vector<vector<int>> paths;
        allPathsUtil(u, v, visited, path, paths);
        return paths;
    }

    vector<int> shortestPath(int u, int v) {
        unordered_map<int, int> dist;
        unordered_map<int, int> parent;
        for (auto& pair : adj) {
            dist[pair.first] = INT_MAX;
        }
        dist[u] = 0;
        parent[u] = -1;

        queue<int> q;
        q.push(u);

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            for (auto& neighbour : adj[node]) {
                if (dist[neighbour.first] == INT_MAX) {
                    dist[neighbour.first] = dist[node] + 1;
                    parent[neighbour.first] = node;
                    q.push(neighbour.first);
                }
            }
        }

        vector<int> path;
        if (dist[v] == INT_MAX) return path;

        for (int at = v; at != -1; at = parent[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
        return path;
    }

    void degreeOfNode(int node) {
        if (directed) {
            int inDegree = 0, outDegree = adj[node].size();
            for (auto& pair : adj) {
                for (auto& edge : pair.second) {
                    if (edge.first == node) ++inDegree;
                }
            }
            cout << "In-degree of node " << node << ": " << inDegree << endl;
            cout << "Out-degree of node " << node << ": " << outDegree << endl;
        } else {
            cout << "Degree of node " << node << ": " << adj[node].size() << endl;
        }
    }
};

int main() {
    
    bool directed, weighted;
    cout << "Is the graph directed (1) or undirected (0)? ";
    cin >> directed;
    cout << "Is the graph weighted (1) or unweighted (0)? ";
    cin >> weighted;

    Graph g(directed, weighted);

    int n, m;
    cout << "Enter number of nodes and edges: ";
    cin >> n >> m;

    cout << "Enter edges (u v [w]):\n";
    for (int i = 0; i < m; ++i) {
        int u, v, w = 1;
        cin >> u >> v;
        if (weighted) cin >> w;
        g.addEdge(u, v, w);
    }

    while (true) {
        cout << "\nChoose operation:\n";
        cout << "1. View Graph\n";
        cout << "2. Add Edge\n";
        cout << "3. Delete Edge\n";
        cout << "4. Convert to Unweighted\n";
        cout << "5. Convert to Undirected\n";
        cout << "6. BFS Traversal\n";
        cout << "7. DFS Traversal\n";
        cout << "8. Check Path Exists\n";
        cout << "9. Find All Paths\n";
        cout << "10. Find Shortest Path\n";
        cout << "11. Degree of a Node\n";
        cout << "0. Exit\n";

        int op;
        cin >> op;

        if (op == 0) break;
        if (op == 1) g.viewGraph();
        else if (op == 2) {
            int u, v, w = 1;
            cout << "Enter edge (u v [w]): ";
            cin >> u >> v;
            if (weighted) cin >> w;
            g.addEdge(u, v, w);
        } else if (op == 3) {
            int u, v;
            cout << "Enter edge (u v): ";
            cin >> u >> v;
            g.deleteEdge(u, v);
        } else if (op == 4) g.convertWeightedToUnweighted();
        else if (op == 5) g.convertDirectedToUndirected();
        else if (op == 6) {
            int start;
            cout << "Enter start node for BFS: ";
            cin >> start;
            g.BFS(start);
        } else if (op == 7) {
            int start;
            cout << "Enter start node for DFS: ";
            cin >> start;
            g.DFS(start);
        } else if (op == 8) {
            int u, v;
            cout << "Enter nodes u and v: ";
            cin >> u >> v;
            cout << (g.hasPath(u, v) ? "Path exists." : "No path.") << endl;
        } else if (op == 9) {
            int u, v;
            cout << "Enter nodes u and v: ";
            cin >> u >> v;
            auto paths = g.findAllPaths(u, v, n);
            for (auto& path : paths) {
                for (int node : path) cout << node << " ";
                cout << endl;
            }
        } else if (op == 10) {
            int u, v;
            cout << "Enter nodes u and v: ";
            cin >> u >> v;
            auto path = g.shortestPath(u, v);
            if (path.empty()) cout << "No path exists.\n";
            else {
                for (int node : path) cout << node << " ";
                cout << endl;
            }
        } else if (op == 11) {
            int node;
            cout << "Enter node: ";
            cin >> node;
            g.degreeOfNode(node);
        }
    }

    return 0;
}






