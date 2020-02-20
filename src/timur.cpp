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

bool usedBooks[MAX_B];

struct MySolver : public Context {

    vector<int> cur_libs;
    vector<int> pointers;
    vector<vector<int>> chosen_books;

    void Add(int idx) {
        cur_libs.push_back(idx);
        pointers.push_back(0);
        chosen_books.emplace_back();
    }

    void SelectBooks() {
        for (int i = 0; i < cur_libs.size(); ++i) { // try another order
            int l_idx = cur_libs[i];
            int& p = pointers[i];

            int chosen = 0;

            while (p < Libs[l_idx].N && chosen < Libs[l_idx].M) {
                auto b_idx = Libs[l_idx].Books[p++];
                if (!usedBooks[b_idx]) {
                    chosen_books[i].emplace_back(b_idx);
                    usedBooks[b_idx] = true;
                }
            }
        }
    }

    vector<int> sortedLibraries;
    int sortedLibrariesIdx;

    void SingUpNewLibrary(int& nextLib, int& nextSingUpDay) {
        if (cur_libs.size() == L) {
            nextSingUpDay = -1;
            return;
        }
        int result_idx = sortedLibraries[sortedLibrariesIdx++];

        nextLib = result_idx;
        nextSingUpDay += Libs[result_idx].T;
    }


    void Solve() {
        for (int i = 0; i < L; ++i) {
            sortedLibraries.push_back(i);
        }
        sort(sortedLibraries.begin(), sortedLibraries.end(), [&](int a, int b) {
            return Libs[a].M > Libs[b].M;
        });

        int nextLib = -1;
        int nextSingUpDay = 0;
        for(int d = 0; d < D; ++d) {
            if (d == nextSingUpDay) {
                if (nextLib != -1) {
                    Add(nextLib);
                }
                SingUpNewLibrary(nextLib, nextSingUpDay);
            }
            SelectBooks();
        }
        for (int i = 0; i < cur_libs.size(); ++i) {
            if (chosen_books[i].size()) {
                Solution.SignedLibs.push_back({cur_libs[i], chosen_books[i]});
            }
        }
    }
};

int main() {
    MySolver solver;

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
