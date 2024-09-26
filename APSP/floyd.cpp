#include<iostream>
#include<cmath>
using namespace std;
const int N = 515;
const int INF = 0x3f3f3f3f;
int n, m;
int d[N][N];
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
        d[a][b] = d[b][a] = 1;
    }
    for(int k = 1; k <= n; k++) {
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
            }
        }
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