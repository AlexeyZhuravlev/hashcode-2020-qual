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
bool willBeScanned[MAX_B];

struct MySolver : public Context {

    vector<int> cur_libs;
    vector<int> pointers;
    vector<vector<int>> chosen_books;

    void Add(int idx, int day) {
        cur_libs.push_back(idx);
        pointers.push_back(0);
        chosen_books.emplace_back();

        int maxBooksToSend = (D - day) * Libs[idx].M;
        for (int p = 0; p < Libs[idx].N && maxBooksToSend; p++) {
            auto b_idx = Libs[idx].Books[p];
            if (!usedBooks[b_idx]) {
                willBeScanned[b_idx] = true;
                maxBooksToSend--;
            }
        }
    }

    double GetLibScore(int i, int day) { // i in cur_libs
        int l_idx = cur_libs[i];

        int totalRemainedBooks = 0;
        int totalRemainedScore = 0;
        int estimatedRemainedBooks = 0;
        int estimatedRemainedScore = 0;
        int maxBooksToSend = (D - day) * Libs[l_idx].M;

        for (int p = pointers[i]; p < Libs[l_idx].N; p++) {
            auto b_idx = Libs[l_idx].Books[p];
            if (!usedBooks[b_idx]) {
                if (totalRemainedBooks < maxBooksToSend) {
                    estimatedRemainedBooks += 1;
                    estimatedRemainedScore += Scores[b_idx];
                }
                totalRemainedBooks += 1;
                totalRemainedScore += Scores[b_idx];
            }
        }

        return estimatedRemainedBooks * 1.0 / Libs[l_idx].M;
    }

    void SelectBooks(int day) {
        vector<int> order;
        for (int i = 0; i < cur_libs.size(); ++i) {
            order.push_back(i);
        }
//        sort(order.begin(), order.end(), [&](int a, int b) {
//            return GetLibScore(a, day) > GetLibScore(b, day);
//        });

        for (int i : order) { // try another order

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

    int numLibraries[MAX_B];

    double IntersectionScore(int b) {
        return log(1 + numLibraries[b]);
    }

    double GetBookScore(int b) {
        // check for current libraries;
        return Scores[b]; // / IntersectionScore(b);
    }

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
                    if (!willBeScanned[b]) {
                        score += GetBookScore(b);
                    }
                    --remainedBooks;
                }
            }

            score /= Libs[i].T;

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

        for (const auto& l : Libs) {
            for (int b : l.Books) {
                numLibraries[b]++;
            }
        }

        int nextLib = -1;
        int nextSingUpDay = 0;
        for(int d = 0; d < D; ++d) {
            if (d == nextSingUpDay) {
                if (nextLib != -1) {
                    Add(nextLib, d);
                }
            }
            SelectBooks(d);
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
