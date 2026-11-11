#include <iostream>
#include <algorithm>
#include <vector>
#include <ostream>
#include <unordered_set>
#include <unordered_map>
#include <stack>

using Edge = std::pair<size_t, size_t>;
using Edges = std::vector<Edge>;

std::ostream & operator<<(std::ostream & out, const Edges edges) {
    out << "Edges:" << std::endl;
    for (size_t i{0}; i < edges.size(); i++) 
        out << i << ") " << edges[i].first << ", "
                << edges[i].second << std::endl;

    return out;
}


void iterativeDFS(size_t start, const Edges& edges) {
    std::unordered_map<size_t, std::vector<size_t>> adj;
    for (const auto& [u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (auto& [node, neighbors] : adj) {
        std::sort(neighbors.rbegin(), neighbors.rend());
    }

    std::unordered_set<size_t> visited;
    std::stack<size_t> s;
    s.push(start);

    std::cout << "Forward iteration DFS: ";

    while (!s.empty()) {
        size_t current = s.top();
        s.pop();

        if (visited.insert(current).second) {
            std::cout << current << " ";

            for (size_t neighbor : adj[current]) {
                if (visited.find(neighbor) == visited.end()) {
                    s.push(neighbor);
                }
            }
        }
    }
    std::cout << std::endl;
}

int main(void) {
    Edges edges{
        {
            {1, 2}, {1, 4}, {1, 6},
            {2, 3}, {2, 5}, {2, 6},
            {3, 4}, {4, 5}, {5, 6}
        }
    };

    std::sort(edges.begin(), edges.end());

    std::cout << edges << std::endl;

    iterativeDFS(1, edges);

    return 0;
}
