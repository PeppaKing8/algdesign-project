// generate random graph with n vertices.
#include<iostream>
#include<cmath>
using namespace std;
const int n = 256;
int d[n + 1][n + 1];
int fa[n + 1];
bool random(int x, int y) { // w.p. x/y
    return rand() % y < x;
}
int find(int x) {
    return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void merge(int x, int y) {
    fa[find(x)] = find(y);
}
int main() {
    srand(time(0));
    int m = 0;
    for(int i = 1; i <= n; i++) fa[i] = i;
    for(int i = 1; i <= n; i++) {
        for(int j = i + 1; j <= n; j++) {
            int k = random(1, 10);
            d[i][j] = d[j][i] = k;
            if (k) m++, merge(i, j);
        }
    }
    // make sure the graph is connected
    for(int i = 2; i <= n; i++) {
        if(find(i) != find(1)) {
            int j = rand() % (i - 1) + 1;
            d[i][j] = d[j][i] = 1;
            m++, merge(i, j);
        }
    }
    cout << n << " " << m << endl;
    for(int i = 1; i <= n; i++) {
        for(int j = i + 1; j <= n; j++) {
            if(d[i][j]) cout << i << " " << j << endl;
        }
    }
    return 0;
}