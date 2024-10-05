#!/bin/bash

for i in $(seq 300 100 1000)
do
    sed -i '' '4s/.*/const int V = '$i';/' main.cpp
    g++ --std=c++11 main.cpp -o main
    > data.txt
    for j in {1..100}
    do
        ./main >> data.txt
    done
    echo "i = $i" >> result.txt
    python3 draw_pic.py >> result.txt
done