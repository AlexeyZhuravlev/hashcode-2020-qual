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

struct Context {
    using TSolution = int;
    TSolution Solution;

    void Input() {
        
    }

    void Output() {
        
    }

    uint64_t Verify() {
        assert(Solution.size() <= C * S && "Too many steps.");

        map<pair<int, int>, int> compiledOnServer;
        memset(earliestAvailable, -1, sizeof(earliestAvailable));
        memset(earliestAvailableEverywhere, -1, sizeof(earliestAvailableEverywhere));
        memset(timeOnServer, 0, sizeof(timeOnServer));

        auto getEarliestAvailable = [&](int file, int server) {
            if (earliestAvailableEverywhere[file] < 0) {
                return -1;
            }
            auto it = compiledOnServer.find(make_pair(file, server));
            if (it != compiledOnServer.end()) {
                return min(earliestAvailableEverywhere[file], it->second);
            }
            return earliestAvailableEverywhere[file];
        };

        for (auto& [file, server] : Solution) {
            auto earliestStart = timeOnServer[server];
            for (auto dep : Deps[file]) {
                auto tDep = getEarliestAvailable(dep, server);
                assert(tDep >= 0 && "Dependency not available");
                earliestStart = max(earliestStart, tDep);
            }
            auto compileFinish = earliestStart + CT[file];
            if (earliestAvailable[file] < 0) {
                earliestAvailable[file] = compileFinish;
            } else {
                earliestAvailable[file] = min(earliestAvailable[file], compileFinish);
            }
            auto replicateFinish = compileFinish + RT[file];
            if (earliestAvailableEverywhere[file] < 0) {
                earliestAvailableEverywhere[file] = replicateFinish;
            } else {
                earliestAvailableEverywhere[file] = min(earliestAvailableEverywhere[file], replicateFinish);
            }
            assert(compiledOnServer.find(make_pair(file, server)) == compiledOnServer.end() && "Compiling same file on same server");
            compiledOnServer[make_pair(file, server)] = compileFinish;
            timeOnServer[server] = compileFinish;
        }

        uint64_t score = 0;
        for (size_t i = 0; i < T; ++i) {
            int f = Target[i];
            if (earliestAvailable[f] < 0) {
                continue;
            }
            if (earliestAvailable[f] <= Deadline[i]) {
                score += Points[i] + (Deadline[i] - earliestAvailable[f]);
            }
        }

        return score;
    }

    uint64_t GetScore() {
        return Verify();
    }

    void Better(const int iterations = 1000, const int generation = 100) {

    }

};
