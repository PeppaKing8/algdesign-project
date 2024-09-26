#include <iostream>

#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <queue>
#include <list>
#include <map>
#include <set>

int testVertexCount = 5;
int testEdgeCount = 7;

struct Edge {
    int src, dest, weight;
    int id;

    void printEdge() {
        std::cout << "Edge: " << src << " -> " << dest << " with weight " << weight << std::endl;
    }

    void removeEdge() {
        src = -1;
        dest = -1;
        weight = -1;
        id = -1;
    }
};

class Graph {
public:
    int V, E;
    int totalEdges = 0;
    Edge *edges;
    Edge ***edgeMap;

    Graph(int V, int E) : V(V), E(E) {
        edges = new Edge[E];
        for (int i = 0; i < E; ++i) {
            edges[i] = {-1, -1, -1, -1};
        }
        edgeMap = new Edge**[V];
        for (int i = 0; i < V; ++i) {
            edgeMap[i] = new Edge*[V];
            for (int j = 0; j < V; ++j) {
                edgeMap[i][j] = nullptr;
            }
        }
    }

    void addEdge(int src, int dest, int weight) {
        if (edgeMap[src][dest] != nullptr) {
            edgeMap[src][dest]->weight += weight;
            // edgeMap[dest][src]->weight += weight;
            totalEdges++;
        }
        else {
            Edge edge = {src, dest, weight, totalEdges};
            edges[totalEdges] = edge;
            edgeMap[src][dest] = &edges[totalEdges];
            edgeMap[dest][src] = &edges[totalEdges];
            totalEdges++;
        }
    }

    void printGraph() {
        std::cout << "Graph with " << V << " vertices and " << E << " edges" << std::endl;
        for (int i = 0; i < E; ++i) {
            if (edges[i].src == -1) {
                continue;
            }
            Edge edge = edges[i];
            std::cout << "Edge: " << edge.src << " -> " << edge.dest << " with weight " << edge.weight << std::endl;
        }
    }


};

Graph generateRandomGraph(int V, int E, int seed) {
    Graph graph(V, E);
    std::srand(seed);
    // std::srand(std::time(0));

    for (int i = 0; i < E; ++i) {
        int src = std::rand() % V;
        int dest = std::rand() % V;
        while (dest == src) {
            dest = std::rand() % V;
        }
        int weight = std::rand() % 100 + 1; // Random weight between 1 and 100
        graph.addEdge(src, dest, weight);
    }
    std::srand(std::time(0));

    return graph;
}