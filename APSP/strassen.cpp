#include<iostream>
#include<assert.h>
#include<vector>
#include<numeric>
#include<algorithm>
#include<random>
#define CONST 3
#define THRES 64
using namespace std;
const int N = 515;
int n, m;
int d[N][N];
struct Matrix{
    int _n, _m;
    // int a[N][N];
    vector<vector<int> > a;
    Matrix(int n = 0, int m = 0): _n(n), _m(m) {
        // for(int i = 1; i <= n; i++) {
        //     for(int j = 1; j <= m; j++) {
        //         a[i][j] = 0;
        //     }
        // }
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
    Matrix operator * (const Matrix &b) const { // matrix multiplication, to be improved
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

Matrix strassen(const Matrix &A, const Matrix &B) { // Strassen Algorithm 
    assert(A._m == B._n && B._n == B._m && A._n == A._m);
    int p = B._m;
    Matrix C(p, p);
    if(p <= THRES) {
        for(int i = 1; i <= p; i++) {
            for(int j = 1; j <= p; j++) {
                for(int k = 1; k <= p; k++) {
                    C.a[i][j] += A.a[i][k] * B.a[k][j];
                }
            }
        }
        return C;
    }
    int p2 = p / 2;
    Matrix A11(p2, p2), A12(p2, p2), A21(p2, p2), A22(p2, p2);
    Matrix B11(p2, p2), B12(p2, p2), B21(p2, p2), B22(p2, p2);
    Matrix C11(p2, p2), C12(p2, p2), C21(p2, p2), C22(p2, p2);
    for(int i = 1; i <= p2; i++) {
        for(int j = 1; j <= p2; j++) {
            A11.a[i][j] = A.a[i][j];
            A12.a[i][j] = A.a[i][j + p2];
            A21.a[i][j] = A.a[i + p2][j];
            A22.a[i][j] = A.a[i + p2][j + p2];
        }
    }
    for(int i = 1; i <= p2; i++) {
        for(int j = 1; j <= p2; j++) {
            B11.a[i][j] = B.a[i][j];
            B12.a[i][j] = B.a[i][j + p2];
            B21.a[i][j] = B.a[i + p2][j];
            B22.a[i][j] = B.a[i + p2][j + p2];
        }
    }
    Matrix P1 = strassen(A11, B12 - B22);
    Matrix P2 = strassen(A11 + A12, B22);
    Matrix P3 = strassen(A21 + A22, B11);
    Matrix P4 = strassen(A22, B21 - B11);
    Matrix P5 = strassen(A11 + A22, B11 + B22);
    Matrix P6 = strassen(A12 - A22, B21 + B22);
    Matrix P7 = strassen(A11 - A21, B11 + B12);
    C11 = P5 + P4 - P2 + P6;
    C12 = P1 + P2;
    C21 = P3 + P4;
    C22 = P5 + P1 - P3 - P7;
    for(int i = 1; i <= p2; i++) {
        for(int j = 1; j <= p2; j++) {
            C.a[i][j] = C11.a[i][j];
            C.a[i][j + p2] = C12.a[i][j];
            C.a[i + p2][j] = C21.a[i][j];
            C.a[i + p2][j + p2] = C22.a[i][j];
        }
    }
    return C;
}
Matrix mul(const Matrix &A, const Matrix &B) {
    // cout<<"MUL "<<A._n<<" "<<A._m<<" "<<B._n<<" "<<B._m<<endl;
    if(A._n == A._m) {
        return strassen(A, B);
    }
    int q = A._n / A._m;
    if(q >= 8) {
        return A * B;
    }
    Matrix C(A._n, B._m), T(A._m, A._m), S(A._m, A._m), Ans(A._m, A._m);
    for(int i = 0; i < q; i++) {
        for(int j = 1; j <= A._m; j++) {
            for(int k = 1; k <= A._m; k++) {
                T.a[j][k] = A.a[i * A._m + j][k];
            }
        }
        for(int i_ = 0; i_ < q; i_++ ) {
            for(int j = 1; j <= A._m; j++) {
                for(int k = 1; k <= A._m; k++) {
                    S.a[j][k] = B.a[j][i_ * A._m + k];
                }
            }
            Ans = strassen(T, S);
            for(int j = 1; j <= A._m; j++) {
                for(int k = 1; k <= A._m; k++) {
                    C.a[i * A._m + j][i_ * A._m + k] = Ans.a[j][k];
                }
            }
        }
    }
    return C;
}
Matrix D(n, n);
Matrix Solve(Matrix &A){
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= n; j++) {
            A.a[i][i] = 1;
        }
    }
    Matrix Z = strassen(A, A);
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
    Matrix S = strassen(A, _D);
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
    Matrix W = - strassen(A, B), Z(n, n);
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
            Z = mul(AR, BR);
            // cout<<"Ok"<<endl;
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
                // brute_cnt ++;
                for(int l = 1; l <= n; l++) {
                    if(A.a[i][l] && B.a[l][j]) {
                        W.a[i][j] = l;
                        break;
                    }
                }
            }
        }
    }
    // cout<<"Brute cnt: "<<brute_cnt<<endl;
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