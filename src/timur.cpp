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

    bool usedLibraries[100000];

    void SingUpNewLibrary(int& nextLib, int& nextSingUpDay, const int day) {
        if (cur_libs.size() == L) {
            nextSingUpDay = -1;
            return;
        }

        int result_idx = -1;
        double maxScore = 0;

        for (int i = 0; i < L; ++i) {
            if (usedLibraries[i]) {
                continue;
            }

            const auto& l = Libs[i];

            int remained = D - day - l.T;
            int remainedBooks = remained * l.M;

            double score = 0;

            for (int b : l.Books) {
                if (!remainedBooks) {
                    break;
                }
                if (!usedBooks[b]) {
                    score += Scores[b];
                    --remainedBooks;
                }
            }

            score /= log(Libs[i].T);

            if (score > maxScore) {
                maxScore = score;
                result_idx = i;
            }
        }

        nextLib = result_idx;
        usedLibraries[nextLib] = true;
        nextSingUpDay += Libs[result_idx].T;
    }


    void Solve() {

        int nextLib = -1;
        int nextSingUpDay = 0;
        for(int d = 0; d < D; ++d) {
            if (d == nextSingUpDay) {
                if (nextLib != -1) {
                    Add(nextLib);
                }
            }
            SelectBooks();
            if (d == nextSingUpDay) {
                SingUpNewLibrary(nextLib, nextSingUpDay, d);
            }
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
