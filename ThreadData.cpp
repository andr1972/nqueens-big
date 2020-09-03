//
// Created by andrzej on 8/28/20.
//

#include "ThreadData.h"

ThreadData::ThreadData(NewInterchangeData *xchgData, vector<int> *v): xchgData(xchgData) {
    copy_v = *v;
}

int NewInterchangeData::cnt;
set<uint64_t> NewInterchangeData::solutionsSums;