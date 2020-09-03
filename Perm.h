#pragma once
#include "ThreadData.h"
#include "ZeroNode.h"
#include <thread>
#include <exception>
#include <cassert>
#include <thread>
#include <random>
#include <algorithm>

class Perm {
    ZeroNode *root;
    vector<thread*> threads;
    int NThr;

    static void execute(ThreadData* self) {
        NewInterchangeData *xchgData = self->xchgData;
        for (self->posSwaps=0; self->posSwaps < self->swaps.size(); self->posSwaps++)
        {
            if (xchgData->endflag) return;
            if (self->posSwaps % 1000 == 0)
            {
                lock_guard<mutex> guard(xchgData->chg_mutex);
                if (!xchgData->endflag)
                    xchgData->progressDot(self->posSwaps);
            }
            if (self->posSwaps % 10000 == 0)
            {
                lock_guard<mutex> guard(xchgData->chg_mutex);
                if (!xchgData->endflag)
                    xchgData->progress(self->copy_v, self->posSwaps);
            }
            pair<int,int> p1;
            p1 = self->swaps[self->posSwaps];
            int row1 = p1.first;
            int row2 = p1.second;
            int dcnt = diff2Thr(self->copy_v, row1, row2);
            if (dcnt<0) {
                uint64_t futureSum = computeFutureSum(self->copy_v, p1);
                auto it = NewInterchangeData::solutionsSums.find(futureSum);
                if (it == NewInterchangeData::solutionsSums.end()) {
                    lock_guard<mutex> guard(xchgData->chg_mutex);
                    if (!xchgData->endflag) {
                        xchgData->ret_p = p1;
                        xchgData->ret_dcnt = dcnt;
                        xchgData->endflag = true;
                    }
                    return;
                }
            } else if (dcnt==0) {
                uint64_t futureSum = computeFutureSum(self->copy_v, p1);
                auto it = xchgData->checkSums.find(futureSum);
                if (it == xchgData->checkSums.end()) {
                    lock_guard<mutex> guard(xchgData->chg_mutex);
                    if (!xchgData->endflag)
                    {
                        xchgData->zeroTransitions.push_back(p1);
                        xchgData->checkSums.insert(futureSum);
                    }
                }
            }
        }
        lock_guard<mutex> guard(xchgData->chg_mutex);
        if (xchgData->endflag) return;
        xchgData->ret_p = make_pair(-1, -1);
        xchgData->ret_dcnt = 0;
        xchgData->endflag = true;
    }

    static void executeA(ThreadData* self) {
        NewInterchangeData *xchgData = self->xchgData;
        for (self->posSwaps=0; self->posSwaps < self->swaps.size(); self->posSwaps++)
        {
            if (xchgData->endflag) return;
            if (self->posSwaps % 1000 == 0)
            {
                lock_guard<mutex> guard(xchgData->chg_mutex);
                if (!xchgData->endflag)
                    xchgData->progressDot(self->posSwaps);
            }
            if (self->posSwaps % 10000 == 0)
            {
                lock_guard<mutex> guard(xchgData->chg_mutex);
                if (!xchgData->endflag)
                    xchgData->progress(self->copy_v, self->posSwaps);
            }
            pair<int,int> p1;
            p1 = self->swaps[self->posSwaps];
            int row1 = p1.first;
            int row2 = p1.second;
            int dcnt = diff2Thr(self->copy_v, row1, row2);
            if (dcnt>0 && dcnt<xchgData->worstDiff) {
                uint64_t futureSum = computeFutureSum(self->copy_v, p1);
                auto it = NewInterchangeData::solutionsSums.find(futureSum);
                if (it == NewInterchangeData::solutionsSums.end()) {
                    lock_guard<mutex> guard(xchgData->chg_mutex);
                    if (!xchgData->endflag) {
                        xchgData->ret_p = p1;
                        xchgData->ret_dcnt = dcnt;
                        xchgData->endflag = true;
                    }
                    return;
                }
            }
        }
        lock_guard<mutex> guard(xchgData->chg_mutex);
        if (xchgData->endflag) return;
        xchgData->ret_p = make_pair(-1, -1);
        xchgData->ret_dcnt = INT32_MAX;
        xchgData->endflag = true;
    }

    void startThreads(ZeroNode *zn, bool tempWorse) {
        assert(threadData.empty());
        if (NThr==0)
            threadData.emplace_back(ThreadData(&xchgData, vmain));
        else
            for (int i = 0; i < NThr; i++)
                threadData.emplace_back(ThreadData(&xchgData, vmain));
        propagate(zn);
        xchgData.endflag = false;
        assert(threads.empty());
        if (NThr>0)
        for (int i=0; i<NThr; i++) {
            auto data = &threadData[i];
            thread *t ;
            if (tempWorse)
                t = new thread(&executeA, data);
            else
                t = new thread(&execute, data);
            threads.push_back(t);
        }
    }

    void stopThreads(bool tempWorse) {
        if (NThr==0) {
            if (tempWorse)
                executeA(&threadData[0]);
            else
                execute(&threadData[0]);
        }
        else {
            for (auto t : threads) {
                t->join();
                delete t;
            }
            threads.clear();
        }
        threadData.clear();
    }

    int runChunk(ZeroNode *zn) {
        startThreads(zn, false);
        stopThreads(false);
        return xchgData.ret_dcnt;
    }
    int getCnt=0;
public:
    explicit Perm(vector<int> *vmain, int NThr): vmain(vmain),NThr(NThr) {
        generator = new mt19937(300);
        root = new ZeroNode(make_pair(-1,-1),this, nullptr, 0);
        root->sum = computeSum(*vmain);
        xchgData.checkSums.insert(root->sum);
        xchgData.solutionsSums.insert(root->sum);
        xchgData.owner = this;
    }

    ~Perm(){
        delete root;
        delete generator;
    }

    chrono::time_point<chrono::high_resolution_clock> playTime = chrono::high_resolution_clock::now();

    void dumpPair(ofstream &ofs,pair<int,int> &p);
    void dumpStruct(ofstream &ofs);
    NewInterchangeData xchgData;
    vector<ThreadData> threadData;
    vector<int> *vmain;
    //return diff<0 or ==0 if can't
    int getBetter() {
        getCnt++;
        if (getCnt!=1) throw exception();
        ZeroNode *current=root,*tmp;
        int childPos=0;
        //vector<int> childPosVec;
        while (true)
        {
            xchgData.current = current;
            int dcnt = runChunk(current);
            //playWav("../short.wav");
            if (dcnt<0) {
                swapTab(*vmain, xchgData.ret_p.first, xchgData.ret_p.second);
                return dcnt;
            }
            current->addChilds(&xchgData);
            //childPosVec.push_back(childPos);
            tmp = current->goFirstChild();
            if (!tmp)
                tmp = current->goNextSiblingOrUncle();
            if (!tmp)
                return 0;
            else
                current = tmp;
        }
    }

    mt19937 *generator;

    void propagate(ZeroNode *zn) {
        pair<int,int> except;
        if (zn)
            except = zn->p;
        else
            except = make_pair(-1,-1);
        int N = vmain->size();
        vector<pair<int, int>> temp;
        for (int i = 0; i < N - 1; i++)
            for (int j = i + 1; j < N; j++) {
                pair<int, int> p(i, j);
                if (p != except) {
                    temp.push_back(p);
                }
            }
        shuffle(temp.begin(), temp.end(), *generator);
        int len = NThr==0?temp.size():temp.size() / NThr;
        int start = 0;
        int NThr1=NThr==0?1:NThr;
        for (int i = 0; i < NThr1; i++) {
            int end = i < NThr1 - 1 ? start + len : temp.size();
            threadData[i].copyPart(temp, start, end);
            start = end;
        }
    }

    bool tryWorse(int worstDiff);
};

