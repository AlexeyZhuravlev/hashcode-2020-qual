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

struct MySolver : public Context {
    void Solve() {
        memset(DP, -1, sizeof(DP));
        DP[0] = -2;
        for (int l = 0; l < L; l++) {
            auto& lib = Libs[l];
            for (int i = D - 1 - lib.T; i >= 0; i--) {
                if (DP[i] == -1 || DP[i + lib.T] != -1) {
                    continue;
                }
                DP[i + lib.T] = l;
            }
        }
        vector<int> libs;
        for (int l = D - 1; l >= 0; l--) {
            if (DP[l] == -1) {
                continue;
            }
            int x = l;
            while (DP[x] >= 0) {
                libs.push_back(DP[x]);
                x = x - Libs[DP[x]].T;
            }
            break;
        }
        for (auto l : libs) {
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
