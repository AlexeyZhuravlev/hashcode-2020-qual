#pragma once

#include <algorithm>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <unordered_map>

using namespace std;

const int MAX_B = 100005;

struct TSolLib {
    int Id;
    vector<int> Books;
};

struct TSolution {
    vector<TSolLib> SignedLib;
};

struct TLibrary {
    int N;
    int T;
    int M;
    vector<int> Books;
};

struct Context {
    TSolution Solution;

    int B;
    int L;
    int D;
    int Scores[MAX_B];
    TLibrary Libs[MAX_B];

    void Input() {
        cin >> B >> L >> D;
        for (int i = 0; i < B; ++i) {
            cin >> Scores[i];
        }
        for (int i = 0; i < L; ++i) {
            cin >> Libs[i].N >> Libs[i].T >> Libs[i].M;
            Libs[i].Books.resize(Libs[i].N);
            for (int j = 0; j < Libs[i].N; j++) {
                cin >> Libs[i].Books[j];
            }
        }
    }

    void Output() {
        cout << Solution.SignedLib.size() << endl;
        for (auto& signedLib : Solution.SignedLib) {
            cout << signedLib.Id << " " << signedLib.Books.size() << endl;
            for (auto book : signedLib.Books) {
                cout << book << " ";
            }
            cout << endl;
        }
    }

    uint64_t GetScore() {
        return 0;
    }
    
};
