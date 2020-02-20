#include <common.h>

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>
#include <random>
#include <cmath>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <cstdlib>
#include <chrono>
#include <unordered_set>

using namespace std;

struct TQueueEntry {
    int LibNum;
    double Score;
    vector<int> LibraryPrefixSum;
    vector<int> Books;
};

struct MySolver : public Context {
    void Solve() {
        auto daysLeft = D;
        auto libsLeft = L;
        unordered_set<int> usedBookIds;
        vector<int> bookCounts(B, 0);
        for (const auto& lib : Libs) {
            for (const auto& book : lib.Books) {
                ++bookCounts[book];
            }
        }

        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

        auto getBookRemovingProba = [&](int book) {
            std::bernoulli_distribution distr(
                static_cast<double>(bookCounts[book] - 1) / static_cast<double>(libsLeft)
            );
            return distr(gen);
        };


        auto updatePrefixSum = [&](TQueueEntry& entry) {
            entry.LibraryPrefixSum = {0};
            vector<int> filteredBooks;
            for (auto i = 0; i < entry.Books.size(); ++i) {
                auto book = entry.Books[i];
                if (usedBookIds.find(book) != usedBookIds.end()) {
                    continue;
                }
                filteredBooks.push_back(book);
                entry.LibraryPrefixSum.push_back(entry.LibraryPrefixSum.back() + Scores[book]);
            }
            entry.Books = std::move(filteredBooks);
        };

        auto calcScore = [&](const TQueueEntry& entry) -> double {
            const auto& lib = Libs[entry.LibNum];
            if (daysLeft <= lib.T) {
                return 0;
            }
            long long lastBook = min(1llu * entry.Books.size(), 1llu * lib.M * (daysLeft - lib.T));
            long long numBatches = (lastBook + lib.M - 1) / lib.M;
            auto batchesMultiplier = log(max(- numBatches + daysLeft, 2ll)) * 1000;
            // auto batchesMultiplier = static_cast<double>(daysLeft) / static_cast<double>(numBatches);
            // std::cerr << batchesMultiplier << "\n";
            return static_cast<double>(entry.LibraryPrefixSum[lastBook]) / static_cast<double>(lib.T) - batchesMultiplier;
        };

        auto selectBooks = [&](const vector<int>& books, int batchSize) {
            if (1llu * batchSize * daysLeft >= books.size()) {
                return books;
            }
            std::vector<int> result;
            for (int i = books.size() - 1; i >= 0; --i) {
                if (1 + i + result.size() <= 1llu * batchSize * daysLeft) {
                    result.push_back(books[i]);
                    continue;
                }
                if (getBookRemovingProba(books[i])) {
                    continue;
                }
            }
            return result;
        };

        auto taskComparator = [&] (const auto& lhs, const auto& rhs) {
            // should be false if lhs better than rhs
            return lhs.Score < rhs.Score;
        };

        priority_queue<TQueueEntry, vector<TQueueEntry>, decltype(taskComparator)>
            taskQueue(taskComparator);

        for (auto i = 0; i < L; ++i) {
            TQueueEntry newEntry{
                .LibNum = i,
                .Score = 0.,
                .LibraryPrefixSum = {},
                .Books = Libs[i].Books
            };
            // calc prefix sum for whole library
            updatePrefixSum(newEntry);
            newEntry.Score = calcScore(newEntry);
            taskQueue.push(std::move(newEntry));
        }

        while (daysLeft > 0 && !taskQueue.empty()) {
            auto curEntry = taskQueue.top();
            taskQueue.pop();

            if (curEntry.Score == 0) {
                continue;
            }

            updatePrefixSum(curEntry);
            auto newScore = calcScore(curEntry);
            if (newScore != curEntry.Score) {
                curEntry.Score = newScore;
                taskQueue.push(curEntry);
                continue;
            }

            daysLeft -= Libs[curEntry.LibNum].T;
            Solution.SignedLibs.push_back(TSolLib{
                .Id = curEntry.LibNum,
                .Books = selectBooks(curEntry.Books, Libs[curEntry.LibNum].M)
                // .Books = curEntry.Books
            });
            usedBookIds.insert(
                Solution.SignedLibs.back().Books.begin(),
                Solution.SignedLibs.back().Books.end()
            );
            for (const auto& book : curEntry.Books) {
                --bookCounts[book];
            }
            --libsLeft;
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
