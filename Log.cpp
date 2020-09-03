#include "Log.h"
#include <fstream>
#include "Perm.h"
#include "scores.h"

Log::Log(int N) {
    solutionName = "../"+to_string(N)+".dat";
    boardName = "../" + to_string(N) + ".board.dat";
    logName = "../"+to_string(N)+".log";
}

void Log::dot_console(int i) {
    cout << "." << flush;
}

void Log::to_console(int i, int cnt) {
    cout << cnt << " " << i << " " << NowStr() << endl <<flush;
}

uint64_t prevSum = 0;

void Log::logRot(int rotCnt) {
    ofstream ofs;
    ofs.open (logName, ofstream::out | ofstream::app);
    ofs << "Rotation "<< rotCnt << endl << endl;
}

void Log::logWorse(int worseCnt) {
    ofstream ofs;
    ofs.open (logName, ofstream::out | ofstream::app);
    ofs << "TryWorse " << worseCnt << endl << endl;
}

void Log::tempAppend(const vector<int> &pos, int64_t progress, int cnt, void *owner) {
    uint64_t sum = computeSum(pos);
    if (sum==prevSum) return;
    prevSum = sum;
    auto permNode = (Perm*)owner;
    ofstream ofs;
    ofs.open (logName, ofstream::out | ofstream::app);

    ofs << "time at " << NowStr() << endl;
    ofs << "progress=" << progress << " cnt=" << cnt << endl;
    ofs << "checksum=" << sum << endl;
    ofs << "print(" <<pos.size() << ")" <<endl;
    ofs << "print(\"";
    for (int i=0; i<pos.size(); i++)
    {
        if (i>0)ofs << " ";
        ofs << pos[i]+1;
    }
    ofs << "\")" <<endl;
    permNode->dumpStruct(ofs);
    ofs <<endl <<flush;
}

string Log::NowStr() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    struct tm * timeinfo;
    timeinfo = localtime (&now_time);
    char buf[100];
    sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
            timeinfo->tm_year+1900,timeinfo->tm_mon+1,timeinfo->tm_mday,
            timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
    return buf;
}

void Log::saveToFile(const vector<int> &pos) {
    ofstream outfile(solutionName);
    outfile << pos.size() << endl;
    for (int i=0; i<pos.size(); i++)
    {
        if (i>0)outfile << " ";
        outfile << pos[i]+1;
    }
    outfile <<endl;
}

void Log::drawBoard(vector<int> &v) {
    ofstream outfile(boardName);
    int N = v.size();
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++)
        {
            if (v[i]==j) outfile << "*";
            else outfile << ".";
        }
        outfile << endl;
    }
}

string Log::logName;
string Log::boardName;
string Log::solutionName;
