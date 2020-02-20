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

struct TQueueEntry {
    int LibNum;
    long Score;
};

struct MySolver : public Context {
    void Solve() {
        vector<vector<int>> LibraryPrefixSum(L);
        auto daysLeft = D;

        auto recalcScore = [&](const TQueueEntry& entry) {
            const auto& lib = Libs[entry.LibNum];
            if (daysLeft <= lib.T) {
                return 0;
            }
            long long lastBook = min(1ll * lib.N, 1ll * lib.M * (daysLeft - lib.T));
            return LibraryPrefixSum[entry.LibNum][lastBook];
        };

        auto taskComparator = [&] (const auto& lhs, const auto& rhs) {
            // should be false if lhs better than rhs
            return lhs.Score < rhs.Score;
        };

        priority_queue<TQueueEntry, vector<TQueueEntry>, decltype(taskComparator)>
            taskQueue(taskComparator);

        for (auto i = 0; i < L; ++i) {
            const auto& lib = Libs[i];
            vector<int> curLibPrefixSum(lib.N + 1);
            curLibPrefixSum[0] = 0;
            auto numBatches = (lib.N + lib.M - 1) / lib.M;
            for (auto j = 0; j < numBatches; ++j) {
                for (auto k = j * lib.M; k < min((j + 1) * lib.M, lib.N); ++k) {
                    curLibPrefixSum[k + 1] = curLibPrefixSum[k] + Scores[lib.Books[k]];
                }
            }
            LibraryPrefixSum[i] = std::move(curLibPrefixSum);
            TQueueEntry newEntry{
                .LibNum = i,
                .Score = 0
            };
            newEntry.Score = recalcScore(newEntry);
            taskQueue.push(std::move(newEntry));
        }

        while (daysLeft > 0 && !taskQueue.empty()) {
            auto curEntry = taskQueue.top();
            taskQueue.pop();
            auto newScore = recalcScore(curEntry);
            if (newScore != curEntry.Score) {
                curEntry.Score = newScore;
                taskQueue.push(curEntry);
                continue;
            }
            if (curEntry.Score == 0) {
                continue;
            }
            daysLeft -= Libs[curEntry.LibNum].T;
            Solution.SignedLibs.push_back(TSolLib{
                .Id = curEntry.LibNum,
                .Books = {}
            });
            const auto& lib = Libs[curEntry.LibNum];
            for (auto i = 0; i < daysLeft; ++i) {
                long long firstBook = i * lib.M;
                long long lastBook = min(lib.N, (i + 1) * lib.M);
                for (auto j = firstBook; j < lastBook; ++j) {
                    Solution.SignedLibs.back().Books.push_back(lib.Books[j]);
                }
                if (lastBook == lib.N) {
                    break;
                }
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
