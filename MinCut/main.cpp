#include "graph.h"
using namespace std;

const int V = 700;
const int N = V+5;
const int INF = 0x3f3f3f3f;
int n, m;
struct wxbEdge{
    int v, c;
    int nxt;
}edge[N * N];
struct wxbEdges{
    int u, v, c;
}edges[N * N];
int edge_cnt;
int head[N], cnt;
void add(int u, int v, int c){
    edge[cnt].v = v;
    edge[cnt].c = c;
    edge[cnt].nxt = head[u];
    head[u] = cnt++;
}
int level[N], iter[N];

bool bfs(int s, int t) {
    memset(level, -1, sizeof(level));
    queue<int> q;
    level[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = head[u]; i != -1; i = edge[i].nxt) {
            int v = edge[i].v;
            if (edge[i].c > 0 && level[v] < 0) {
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }
    return level[t] >= 0;
}

int dfs(int u, int t, int f) {
    if (u == t) return f;
    for (int &i = iter[u]; i != -1; i = edge[i].nxt) {
        int v = edge[i].v;
        if (edge[i].c > 0 && level[u] < level[v]) {
            int d = dfs(v, t, min(f, edge[i].c));
            if (d > 0) {
                edge[i].c -= d;
                edge[i ^ 1].c += d;
                return d;
            }
        }
    }
    return 0;
}

int maxFlow(int s, int t) {
    int flow = 0;
    while (bfs(s, t)) {
      // printf("%d\n", flow);
        memcpy(iter, head, sizeof(head));
        int f;
        while ((f = dfs(s, t, INF)) > 0) {
            flow += f;
        }
    }
    return flow;
}

int minCut(const Graph & graph) {
   memset(head, -1, sizeof(head));
   int ans = INF;
   for (int i = 2; i <= graph.V; ++i) {
      cnt = 0;
      memset(head, -1, sizeof(head));
      memset(edge, -1, sizeof(edge));
      memset(edges, -1, sizeof(edges));
      edge_cnt = 0;
      // printf("i = %d\n", i);
      for (int j = 0; j < graph.E; ++j) {
         if (graph.edges[j].id == -1) {
            continue;
         }
       add(graph.edges[j].src, graph.edges[j].dest, graph.edges[j].weight);
       add(graph.edges[j].dest, graph.edges[j].src, graph.edges[j].weight);
      }
      // printf("Here\n");
      int now = maxFlow(0, i - 1);
      ans = min(ans, now);
      // printf("now = %d, ans = %d\n", now, ans);
   }
   return ans;
}

std::vector<int> bfsConnectedComponent(const Graph& graph, int start) {
    std::vector<int> connected;
    std::vector<bool> visited(graph.V, false);
    std::queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        connected.push_back(v);

        for (int i = 0; i < graph.E; ++i) {
            Edge edge = graph.edges[i];
            if (edge.src == v && !visited[edge.dest]) {
                visited[edge.dest] = true;
                q.push(edge.dest);
            } else if (edge.dest == v && !visited[edge.src]) {
                visited[edge.src] = true;
                q.push(edge.src);
            }
        }
    }

    return connected;
}

Graph simplify(const Graph & graph) {
    int V = graph.V;
    std::set<int> vertices;
    std::map<int, int> vertexMap;
    int E = 0;

    for (int i = 0; i < graph.E; ++i) {
        if (graph.edges[i].src != -1) {
            vertices.insert(graph.edges[i].src);
            vertices.insert(graph.edges[i].dest);
            E++;
        }
    }

    int index = 0;
    for (int v : vertices) {
        vertexMap[v] = index++;
    }

    Graph simplifiedGraph(index, E);
    for (int i = 0; i < graph.E; ++i) {
        if (graph.edges[i].src != -1) {
            simplifiedGraph.addEdge(vertexMap[graph.edges[i].src], vertexMap[graph.edges[i].dest], graph.edges[i].weight);
        }
    }
   //  printf("finish simplify\n");
    return simplifiedGraph;
}

Graph Contract(const Graph & graph, int finalSize = 2) {
   Graph contractedGraph = graph;
   std::vector<Edge> contractedEdges;
   //  printf("Contracting\n");
   std::vector<int> existEdges;
   for (int i = 0; i < graph.E; ++i) {
      if (graph.edges[i].id != -1) {
         existEdges.push_back(i);
      }
   }
    while (contractedGraph.V > finalSize) {
        int randomIndex = std::rand() % existEdges.size();
        randomIndex = existEdges[randomIndex];

         Edge randomEdge = contractedGraph.edges[randomIndex];
        if (randomEdge.weight == -1) {
            continue;
        }
        contractedEdges.push_back(randomEdge);
        // Merge the two vertices of the random edge
        int src = randomEdge.src;
        int dest = randomEdge.dest;

        // Update the edges to reflect the contraction
        for (int i = 0; i < graph.V; ++i) {
            Edge* ptr = contractedGraph.edgeMap[dest][i];
            if (ptr != nullptr && ptr->id != -1 && ptr->id != randomEdge.id) {
                Edge* edge = ptr;
                // std::cout << "i: " << i << "removing edge: ";
                // edge->printEdge();
                if (edge->src == dest) {
                    if (edge->dest == src) {
                        // existEdges.remove(edge->id);
                        edge->removeEdge();
                        continue;
                    }
                    edge->src = src;
                    if (contractedGraph.edgeMap[src][edge->dest] == nullptr) {
                        contractedGraph.edgeMap[src][edge->dest] = edge;
                    } else {
                        contractedGraph.edgeMap[src][edge->dest]->weight += edge->weight;
                        // existEdges.remove(edge->id);
                        edge->removeEdge();
                    }
                } else if (edge->dest == dest) {
                    if (edge->src == src) {
                        // existEdges.remove(edge->id);
                        edge->removeEdge();
                        continue;
                    }
                    edge->dest = src;
                    if (contractedGraph.edgeMap[src][edge->src] == nullptr) {
                        contractedGraph.edgeMap[src][edge->src] = edge;
                    } else {
                        contractedGraph.edgeMap[src][edge->src]->weight += edge->weight;
                        // existEdges.remove(edge->id);
                        edge->removeEdge();
                    }
                }

            }
        }

        contractedGraph.V--;
        contractedGraph.edges[randomIndex].removeEdge();
        // contractedGraph.printGraph();
      //   std::cout << "Deleting edge: " << randomEdge.src << " -> " << randomEdge.dest << std::endl;
        delete contractedGraph.edgeMap[dest];
        contractedGraph.edgeMap[dest] = nullptr;
    }
   //  printf("finished\n");
    return simplify(contractedGraph);
}


int FastCut(const Graph & graph) {
    if (graph.V <= 6) {
         // graph.printGraph();
         return minCut(graph);
    }
    else {
        const int t = 1 + graph.V / std::sqrt(2);
      //   printf("t = %d\n", t);
      //   graph.printGraph();
      //   printf("Here 1\n");
        Graph graph1 = Contract(graph, t);
      //   graph1.printGraph();
         // printf("Here 2\n");
        Graph graph2 = Contract(graph, t);
      //   printf("Here 3\n");
        int cut1 = FastCut(graph1);
        int cut2 = FastCut(graph2);
        return std::min(cut1, cut2);
    }
}

int main(int argc, char *argv[]) {
      
      std::vector<Graph> graphs;
      int tryTime = 50;
      int seed = std::time(0);
      for (int i = 0; i < tryTime; ++i) {
         graphs.push_back(generateRandomGraph(V, V*V, seed));
      }
      auto start = std::chrono::high_resolution_clock::now();
      int min_cut_result = minCut(graphs[0]);
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> min_cut_duration = end - start;
      std::cout << "Min Cut Result: " << min_cut_result << " Time: " << min_cut_duration.count() << "s" << std::endl;

      start = std::chrono::high_resolution_clock::now();
      int fast_cut_result = INF;
      for (int i = 0; i < tryTime; i++) {
            fast_cut_result = std::min(fast_cut_result, FastCut(graphs[i]));
            if (fast_cut_result == min_cut_result) {
               break;
            }
      }
      end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> fast_cut_duration = end - start;
      std::cout << "Fast Cut Result: " << fast_cut_result << " Time: " << fast_cut_duration.count() << "s" << std::endl;
}