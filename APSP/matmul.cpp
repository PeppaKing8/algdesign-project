#include<iostream>
#include<assert.h>
#include<vector>
#include<numeric>
#include<algorithm>
#include<random>
#define CONST 3
using namespace std;
const int N = 515;
const int INF = 0x3f3f3f3f;
int n, m;
int d[N][N];
struct Matrix{
    int _n, _m;
    vector<vector<int> > a;
    Matrix(int n = 0, int m = 0): _n(n), _m(m) {
        a.resize(n + 1);
        for(int i = 1; i <= n; i++) {
            a[i].resize(m + 1);
            for(int j = 1; j <= m; j++) {
                a[i][j] = 0;
            }
        }
    }
    Matrix operator + (const Matrix &b) const { 
        assert(_n == b._n && _m == b._m);
        Matrix c(_n, _m);
        for(int i = 1; i <= _n; i++) {
            for(int j = 1; j <= _m; j++) {
                c.a[i][j] = a[i][j] + b.a[i][j];
            }
        }
        return c;
    }
    Matrix operator - (const Matrix &b) const {
        assert(_n == b._n && _m == b._m);
        Matrix c(_n, _m);
        for(int i = 1; i <= _n; i++) {
            for(int j = 1; j <= _m; j++) {
                c.a[i][j] = a[i][j] - b.a[i][j];
            }
        }
        return c;
    }
   Matrix operator - () const {
        Matrix c(_n, _m);
        for(int i = 1; i <= _n; i++) {
            for(int j = 1; j <= _m; j++) {
                c.a[i][j] = -a[i][j];
            }
        }
        return c;
    }
    Matrix operator * (const Matrix &b) const { // matrix multiplication, O(n^3)
        assert(_m == b._n);
        Matrix c(_n, b._m);
        for(int i = 1; i <= _n; i++) {
            for(int j = 1; j <= b._m; j++) {
                for(int k = 1; k <= _m; k++) {
                    c.a[i][j] += a[i][k] * b.a[k][j];
                }
            }
        }
        return c;
    }
    bool operator == (const Matrix &b) const {
        if(_n != b._n || _m != b._m) return false;
        for(int i = 1; i <= _n; i++) {
            for(int j = 1; j <= _m; j++) {
                if(a[i][j] != b.a[i][j]) return false;
            }
        }
        return true;
    }
};
Matrix D(n, n);
Matrix Solve(Matrix &A){
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            A.a[i][i] = 1;
        }
    }
    Matrix Z = A * A;
    Matrix _A(n, n);
    bool flag = true;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if(Z.a[i][j] && i != j) _A.a[i][j] = 1;
            else _A.a[i][j] = 0;
            if(!Z.a[i][j] && i != j) flag = false;
        }
    }
    if(flag) return _A + _A - A;
    Matrix _D = Solve(_A);
    Matrix S = A * _D;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if(S.a[i][j] >= _D.a[i][j] * Z.a[i][i]) _D.a[i][j] <<= 1;
            else _D.a[i][j] = (_D.a[i][j] << 1) - 1;
        }
    }
    return _D;
}
std::vector<int> selectRandomSubset(int n, int r) {
    std::vector<int> elements(n);
    std::iota(elements.begin(), elements.end(), 1);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(elements.begin(), elements.end(), g);
    std::vector<int> subset(elements.begin(), elements.begin() + r);
    return subset;
}
Matrix BPWM(const Matrix &A, const Matrix &B){
    Matrix W = - A * B, Z(n, n);
    int k = floor(log(n)), r = 1, lim = floor(CONST * k);
    for(int i = 0; i <= k; i++) {
        Matrix AR(n, r), BR(r, n);
        for(int _ = 1; _ <= lim; _++) {
            std::vector<int> R = selectRandomSubset(n, r);
            for(int i = 1; i <= r; i++) {
                for(int j = 1; j <= n; j++) {
                    AR.a[j][i] = R[i - 1] * A.a[j][R[i - 1]];
                    BR.a[i][j] = B.a[R[i - 1]][j];
                }
            }
            Z = AR * BR;
            for(int _i = 1; _i <= n; _i++) {
                for(int _j = 1; _j <= n; _j++) {
                    int now = Z.a[_i][_j];
                    if(W.a[_i][_j] >= 0 || now <= 0 || now > n) continue;
                    if(A.a[_i][now] && B.a[now][_j]) {
                        W.a[_i][_j] = now;
                    }
                }
            }
        }
        r <<= 1;
    }
    // int brute_cnt = 0;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if(W.a[i][j] < 0) { // brute-force
                // brute_cnt++;
                for(int l = 1; l <= n; l++) {
                    if(A.a[i][l] && B.a[l][j]) {
                        W.a[i][j] = l;
                        break;
                    }
                }
            }
        }
    }
    // cout << "Brute cnt: " << brute_cnt << endl;
    return W;
}
int main(){
    cin >> n >> m;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            d[i][j] = 0;
        }
    }
    for(int i = 1; i <= m; i++) {
        int a, b;
        cin >> a >> b;
        d[a][b] = d[b][a] = 1;
    }
    Matrix A(n, n);
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            A.a[i][j] = d[i][j];
        }
    }
    D = Solve(A);
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if(i == j) D.a[i][j] = 0;
            std::cout << D.a[i][j] << " ";
        }
        std::cout << endl;
    }
    // compute successor matrix
    Matrix Suc(n, n);
    Matrix _D(n, n);
    for(int s = 0; s < 3; s++) {
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                if((D.a[i][j] + 1) % 3 == s) _D.a[i][j] = 1;
                else _D.a[i][j] = 0;
            }
        }
        _D = BPWM(A, _D);
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                if(D.a[i][j] % 3 == s) Suc.a[i][j] = _D.a[i][j];
            }
        }
    }
    // Check if the successor matrix is correct
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            if(i == j || d[i][j] == 1) continue;
            // cout << i << " " << j << " " << Suc.a[i][j] << endl;
            assert(D.a[i][Suc.a[i][j]] + D.a[Suc.a[i][j]][j] == D.a[i][j]);
            assert(Suc.a[i][j] != i);
            assert(Suc.a[i][j] != j);
        }
    }
}