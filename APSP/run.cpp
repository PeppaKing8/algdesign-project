#include<iostream>
#include<chrono>
using namespace std;
int main(){
    string s;
    cin >> s;
    // compile all
    system("g++ gen.cpp -o gen");
    system("g++ bfs.cpp -o bfs");
    system("g++ floyd.cpp -o floyd");
    system("g++ matmul.cpp -o matmul");
    system("g++ strassen.cpp -o strassen");
    // run gen.cpp
    auto start = chrono::high_resolution_clock::now();
    cout << "Generating data/" + s + ".in" << endl;
    string cmd = "./gen > data/" + s + ".in";
    int ret = system(cmd.c_str());
    if (ret != 0) {
        cerr << "Error running gen" << endl;
        return 1;
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "Time used: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
    // run bfs.cpp
    start = chrono::high_resolution_clock::now();
    cout << "Running bfs.cpp" << endl;
    cmd = "./bfs < data/" + s + ".in > data/" + s + "_bfs.out";
    ret = system(cmd.c_str());
    if (ret != 0) {
        cerr << "Error running gen" << endl;
        return 1;
    }
    end = chrono::high_resolution_clock::now();
    cout << "Time used: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
    // run floyd.cpp
    start = chrono::high_resolution_clock::now();
    cout << "Running floyd.cpp" << endl;
    cmd = "./floyd < data/" + s + ".in > data/" + s + "_floyd.out";
    ret = system(cmd.c_str());
    if (ret != 0) {
        cerr << "Error running gen" << endl;
        return 1;
    }
    end = chrono::high_resolution_clock::now();
    cout << "Time used: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
    // run matmul.cpp
    start = chrono::high_resolution_clock::now();
    cout << "Running matmul.cpp" << endl;
    cmd = "./matmul < data/" + s + ".in > data/" + s + "_matmul.out";
    ret = system(cmd.c_str());
    if (ret != 0) {
        cerr << "Error running gen" << endl;
        return 1;
    }
    end = chrono::high_resolution_clock::now();
    cout << "Time used: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
    // run strassen.cpp
    start = chrono::high_resolution_clock::now();
    cout << "Running strassen.cpp" << endl;
    cmd = "./strassen < data/" + s + ".in > data/" + s + "_strassen.out";
    ret = system(cmd.c_str());
    if (ret != 0) {
        cerr << "Error running gen" << endl;
        return 1;
    }
    end = chrono::high_resolution_clock::now();
    cout << "Time used: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
    // compare bfs.out, floyd.out, matmul.out, strassen.out
    cout << "Comparing output" << endl;
    cmd = "diff data/" + s + "_bfs.out data/" + s + "_floyd.out";
    system(cmd.c_str());
    cmd = "diff data/" + s + "_bfs.out data/" + s + "_matmul.out";
    system(cmd.c_str());
    cmd = "diff data/" + s + "_bfs.out data/" + s + "_strassen.out";
    system(cmd.c_str());
    cout << "All passed" << endl;
    return 0;
}