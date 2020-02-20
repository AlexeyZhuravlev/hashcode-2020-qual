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
    vector<TSolLib> SignedLibs;
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
            sort(Libs[i].Books.begin(), Libs[i].Books.end(), [&](int ba, int bb) {
                return Scores[ba] >= Scores[bb];
            });
        }
    }

    void Output() {
        cout << Solution.SignedLibs.size() << endl;
        for (auto& signedLib : Solution.SignedLibs) {
            cout << signedLib.Id << " " << signedLib.Books.size() << endl;
            for (auto book : signedLib.Books) {
                cout << book << " ";
            }
            cout << endl;
        }
    }

    bool IsSigned[MAX_B];
    uint64_t GetScore() {
        uint64_t score = 0;
        memset(IsSigned, false, sizeof(IsSigned));
        uint64_t currentSignUp = 0;
        for (auto& lib : Solution.SignedLibs) {
            int id = lib.Id;
            assert(0 <= id && id < L);
            assert(!IsSigned[id] && "signed library twice.");
            IsSigned[id] = true;
            currentSignUp += Libs[id].T;
            if (currentSignUp < D) {
                int nb = 0;
                int day = 0;
                for (int i = 0; i < Libs[id].Books.size(); ++i) {
                    if (nb == Libs[id].M) {
                        day++;
                        nb = 1;
                    }
                    int book = Scores[Libs[id].Books[i]];
                    assert(0 <= book && book < B);
                    if (currentSignUp + day < D) {
                        score += Scores[Libs[id].Books[i]];
                    }
                }
            }
        }
        return score;
    }
    
};
