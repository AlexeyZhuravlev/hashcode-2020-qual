#include <common.h>

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <chrono>

using namespace std;

int DP[MAX_B];
vector<int> P[MAX_B];

struct MySolver : public Context {
    void Solve() {
        memset(DP, -1, sizeof(DP));
        DP[0] = 0;
        for (int l = 0; l < L; l++) {
            auto& lib = Libs[l];
            for (int i = D - 1; i >= lib.T; i--) {
                if (DP[i - lib.T] == -1) {
                    continue;
                }
                if (DP[i] == -1 || DP[i - lib.T] + 1 > DP[i]) {
                    DP[i] = DP[i - lib.T] + 1;
                    P[i] = P[i - lib.T];
                    P[i].push_back(l);
                }
            }
        }
        int mx = -1;
        int score = 0;
        vector<int> slib;
        for (int i = 0; i < D; i++) {
            if (DP[i] == -1) {
                continue;
            }
            vector<int> libs;
            libs = P[i];
            sort(libs.begin(), libs.end(), [&](int a, int b) {
                return Libs[a].T < Libs[b].T;
            });
            if (libs.size() < 2) {
                continue;
            }
            while (!libs.empty()) {
                Solution.SignedLibs.clear();
                for (auto l : libs) {
                    TSolLib cur;
                    cur.Id = l;
                    cur.Books = Libs[l].Books;
                    Solution.SignedLibs.push_back(std::move(cur));
                }
                if (GetScore() > score) {
                    score = GetScore();
                    mx = i;
                    slib = libs;
                }
                libs.erase(libs.begin());
            }
        }
            Solution.SignedLibs.clear();
            for (auto l : slib) {
                TSolLib cur;
                cur.Id = l;
                cur.Books = Libs[l].Books;
                Solution.SignedLibs.push_back(std::move(cur));
            }
    }
};

MySolver solver;

int main() {
    solver.Input();

    auto start = std::chrono::system_clock::now();
    cerr << "Started solving..." << endl;
    solver.Solve();
    cerr << "Done!" << endl;
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cerr << "Test time: " << elapsed_seconds.count() << endl;

    cerr << "Outputting" << endl;
    solver.Output();

    cerr << solver.GetScore() << endl;
    return 0;
}
