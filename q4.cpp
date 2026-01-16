#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include <climits>
using namespace std;

class Graph {
private:
    int V;
    bool isDirected;
    vector<vector<pair<int, int>>> adj; // {neighbor, weight}
    
    void dfsUtil(int v, vector<bool>& visited, vector<int>& result) {
        visited[v] = true;
        result.push_back(v);
        
        // Get neighbors in sorted order
        vector<int> neighbors;
        for (auto& p : adj[v]) {
            if (!visited[p.first]) {
                neighbors.push_back(p.first);
            }
        }
        sort(neighbors.begin(), neighbors.end());
        
        for (int u : neighbors) {
            if (!visited[u]) {
                dfsUtil(u, visited, result);
            }
        }
    }
    
    bool hasCycleDirectedUtil(int v, vector<int>& color) {
        color[v] = 1; // Gray (processing)
        
        for (auto& p : adj[v]) {
            int u = p.first;
            if (color[u] == 1) return true; // Back edge
            if (color[u] == 0 && hasCycleDirectedUtil(u, color)) return true;
        }
        
        color[v] = 2; // Black (processed)
        return false;
    }
    
    bool hasCycleUndirectedUtil(int v, int parent, vector<bool>& visited) {
        visited[v] = true;
        
        for (auto& p : adj[v]) {
            int u = p.first;
            if (!visited[u]) {
                if (hasCycleUndirectedUtil(u, v, visited)) return true;
            } else if (u != parent) {
                return true;
            }
        }
        return false;
    }
    
    void fillOrder(int v, vector<bool>& visited, stack<int>& st) {
        visited[v] = true;
        for (auto& p : adj[v]) {
            if (!visited[p.first]) {
                fillOrder(p.first, visited, st);
            }
        }
        st.push(v);
    }
    
    void dfsForSCC(int v, vector<bool>& visited, vector<int>& component) {
        visited[v] = true;
        component.push_back(v);
        
        for (auto& p : adj[v]) {
            if (!visited[p.first]) {
                dfsForSCC(p.first, visited, component);
            }
        }
    }
    
    Graph getTranspose() {
        Graph g(V, isDirected);
        for (int v = 0; v < V; v++) {
            for (auto& p : adj[v]) {
                g.adj[p.first].push_back({v, p.second});
            }
        }
        return g;
    }
    
    void topologicalSortUtil(int v, vector<bool>& visited, stack<int>& st) {
        visited[v] = true;
        
        vector<int> neighbors;
        for (auto& p : adj[v]) {
            neighbors.push_back(p.first);
        }
        sort(neighbors.begin(), neighbors.end());
        
        for (int u : neighbors) {
            if (!visited[u]) {
                topologicalSortUtil(u, visited, st);
            }
        }
        st.push(v);
    }
    
public:
    Graph(int vertices, bool directed) : V(vertices), isDirected(directed) {
        adj.resize(V);
    }
    
    void addEdge(int u, int v, int w = 1) {
        adj[u].push_back({v, w});
        if (!isDirected) {
            adj[v].push_back({u, w});
        }
    }
    
    int addVertex() {
        adj.push_back(vector<pair<int, int>>());
        return V++;
    }
    
    // Type 0: BFS
    void bfs(int s) {
        if (s >= V) return;
        
        vector<bool> visited(V, false);
        priority_queue<int, vector<int>, greater<int>> pq; // Min heap for lex order
        vector<int> result;
        
        pq.push(s);
        visited[s] = true;
        
        while (!pq.empty()) {
            int v = pq.top();
            pq.pop();
            result.push_back(v);
            
            for (auto& p : adj[v]) {
                int u = p.first;
                if (!visited[u]) {
                    visited[u] = true;
                    pq.push(u);
                }
            }
        }
        
        for (int i = 0; i < result.size(); i++) {
            cout << result[i];
            if (i < result.size() - 1) cout << " ";
        }
        cout << "\n";
    }
    
    // Type 1: DFS
    void dfs(int s) {
        if (s >= V) return;
        
        vector<bool> visited(V, false);
        vector<int> result;
        dfsUtil(s, visited, result);
        
        for (int i = 0; i < result.size(); i++) {
            cout << result[i];
            if (i < result.size() - 1) cout << " ";
        }
        cout << "\n";
    }
    
    // Type 2: Cycle Detection
    bool hasCycle() {
        if (isDirected) {
            vector<int> color(V, 0); // 0: white, 1: gray, 2: black
            for (int i = 0; i < V; i++) {
                if (color[i] == 0) {
                    if (hasCycleDirectedUtil(i, color)) return true;
                }
            }
            return false;
        } else {
            vector<bool> visited(V, false);
            for (int i = 0; i < V; i++) {
                if (!visited[i]) {
                    if (hasCycleUndirectedUtil(i, -1, visited)) return true;
                }
            }
            return false;
        }
    }
    
    // Type 3: Bipartiteness Check
    bool isBipartite() {
        if (isDirected) return false;
        
        vector<int> color(V, -1);
        
        for (int start = 0; start < V; start++) {
            if (color[start] == -1) {
                queue<int> q;
                q.push(start);
                color[start] = 0;
                
                while (!q.empty()) {
                    int v = q.front();
                    q.pop();
                    
                    for (auto& p : adj[v]) {
                        int u = p.first;
                        if (color[u] == -1) {
                            color[u] = 1 - color[v];
                            q.push(u);
                        } else if (color[u] == color[v]) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
    
    // Type 4: Connected Components
    int countComponents() {
        if (isDirected) {
            // Count SCCs using Kosaraju
            vector<bool> visited(V, false);
            stack<int> st;
            
            for (int i = 0; i < V; i++) {
                if (!visited[i]) {
                    fillOrder(i, visited, st);
                }
            }
            
            Graph gr = getTranspose();
            fill(visited.begin(), visited.end(), false);
            
            int count = 0;
            while (!st.empty()) {
                int v = st.top();
                st.pop();
                
                if (!visited[v]) {
                    vector<int> component;
                    gr.dfsForSCC(v, visited, component);
                    count++;
                }
            }
            return count;
        } else {
            vector<bool> visited(V, false);
            int count = 0;
            
            for (int i = 0; i < V; i++) {
                if (!visited[i]) {
                    vector<int> component;
                    dfsUtil(i, visited, component);
                    count++;
                }
            }
            return count;
        }
    }
    
    // Type 5: Dijkstra's Algorithm
    void dijkstra(int s, int t) {
        if (s >= V || t >= V) {
            cout << "-1\n";
            return;
        }
        
        vector<int> dist(V, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        
        dist[s] = 0;
        pq.push({0, s});
        
        while (!pq.empty()) {
            int d = pq.top().first;
            int v = pq.top().second;
            pq.pop();
            
            if (d > dist[v]) continue;
            
            for (auto& p : adj[v]) {
                int u = p.first;
                int w = p.second;
                
                if (dist[v] + w < dist[u]) {
                    dist[u] = dist[v] + w;
                    pq.push({dist[u], u});
                }
            }
        }
        
        if (dist[t] == INT_MAX) {
            cout << "-1\n";
        } else {
            cout << dist[t] << "\n";
        }
    }
    
    // Type 6: Kosaraju's Algorithm
    void kosaraju() {
        if (!isDirected) return;
        
        vector<bool> visited(V, false);
        stack<int> st;
        
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                fillOrder(i, visited, st);
            }
        }
        
        Graph gr = getTranspose();
        fill(visited.begin(), visited.end(), false);
        
        vector<vector<int>> sccs;
        
        while (!st.empty()) {
            int v = st.top();
            st.pop();
            
            if (!visited[v]) {
                vector<int> component;
                gr.dfsForSCC(v, visited, component);
                sort(component.begin(), component.end());
                sccs.push_back(component);
            }
        }
        
        // Sort SCCs lexicographically
        sort(sccs.begin(), sccs.end());
        
        for (auto& scc : sccs) {
            for (int i = 0; i < scc.size(); i++) {
                cout << scc[i];
                if (i < scc.size() - 1) cout << " ";
            }
            cout << "\n";
        }
    }
    
    // Type 7: Topological Sort
    void topologicalSort() {
        if (!isDirected) return;
        
        vector<bool> visited(V, false);
        stack<int> st;
        
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                topologicalSortUtil(i, visited, st);
            }
        }
        
        vector<int> result;
        while (!st.empty()) {
            result.push_back(st.top());
            st.pop();
        }
        
        for (int i = 0; i < result.size(); i++) {
            cout << result[i];
            if (i < result.size() - 1) cout << " ";
        }
        cout << "\n";
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int isDirected, V, E;
    cin >> isDirected >> V >> E;
    
    Graph g(V, isDirected);
    
    for (int i = 0; i < E; i++) {
        int u, v, w = 1;
        cin >> u >> v;
        if (cin.peek() != '\n') {
            cin >> w;
        }
        g.addEdge(u, v, w);
    }
    
    int Q;
    cin >> Q;
    
    while (Q--) {
        int type;
        cin >> type;
        
        switch (type) {
            case 0: { // BFS
                int s;
                cin >> s;
                g.bfs(s);
                break;
            }
            case 1: { // DFS
                int s;
                cin >> s;
                g.dfs(s);
                break;
            }
            case 2: { // Cycle Detection
                cout << (g.hasCycle() ? 1 : 0) << "\n";
                break;
            }
            case 3: { // Bipartiteness
                cout << (g.isBipartite() ? 1 : 0) << "\n";
                break;
            }
            case 4: { // Connected Components
                cout << g.countComponents() << "\n";
                break;
            }
            case 5: { // Dijkstra
                int s, t;
                cin >> s >> t;
                g.dijkstra(s, t);
                break;
            }
            case 6: { // Kosaraju
                g.kosaraju();
                break;
            }
            case 7: { // Topological Sort
                g.topologicalSort();
                break;
            }
            case 8: { // Add Edge
                int u, v, w = 1;
                cin >> u >> v;
                if (cin.peek() != '\n') {
                    cin >> w;
                }
                g.addEdge(u, v, w);
                break;
            }
            case 9: { // Add Vertex
                int newVertex = g.addVertex();
                cout << newVertex << "\n";
                break;
            }
        }
    }
    
    return 0;
}