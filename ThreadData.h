#pragma once
#include <set>
#include <vector>
#include <mutex>
#include "Log.h"
#include "scores.h"
#include "Log.h"

using namespace std;

struct NewInterchangeData {
    void *owner= nullptr;//to log
    int worstDiff=0;
    static int cnt;
    set<uint64_t> checkSums;
    static set<uint64_t> solutionsSums;
    vector<pair<int,int>> zeroTransitions;
    bool endflag = false;
    int ret_dcnt;
    pair<int,int> ret_p;
    void *current;
    //int cnt = 0;
    mutex chg_mutex;
    void progress(const vector<int> &pos, int64_t i){
        Log::to_console(i, cnt);
        Log::tempAppend(pos, i, cnt, owner);
    };
    void progressDot(int i){
        Log::dot_console(i);
    };
};

struct ThreadData {
    ThreadData(NewInterchangeData *xchgData, vector<int> *v);
    vector<int> copy_v;
    NewInterchangeData *xchgData;
    vector<pair<int,int>> swaps;
    int posSwaps=0;
    void copyPart(vector<pair<int,int>> &vec, int from,int to) {
        auto first = vec.begin() + from;
        auto last = vec.begin() + to;
        vector<pair<int,int>> newVec(first, last);
        swaps = newVec;
    }
};