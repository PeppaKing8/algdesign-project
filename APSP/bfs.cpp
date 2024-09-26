#include<iostream>
using namespace std;
const int N = 515;
const int INF = 0x3f3f3f3f;
int n, m;
struct Edge{
    int v;
    int next;
}edge[N * N];
int head[N], cnt = 0;
int d[N][N];
void add(int u, int v) {
    edge[++cnt] = (Edge){v, head[u]};
    head[u] = cnt;
}
void bfs(int s) {
    int q[N], l = 0, r = 0;
    q[r++] = s;
    d[s][s] = 0;
    while(l < r) {
        int u = q[l++];
        for(int i = head[u]; i; i = edge[i].next) {
            int v = edge[i].v;
            if(d[s][v] == INF) {
                d[s][v] = d[s][u] + 1;
                q[r++] = v;
            }
        }
    }
}
int main() {
    cin >> n >> m;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            d[i][j] = i == j ? 0 : INF;
        }
    }
    for(int i = 1; i <= m; i++) {
        int a, b;
        cin >> a >> b;
        add(a, b);
        add(b, a);
    }
    for(int i = 1; i <= n; i++) {
        bfs(i);
    }
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if (d[i][j] == INF) d[i][j] = -1;
            cout << d[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}