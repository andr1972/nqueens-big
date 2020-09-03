#pragma once
#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Log {
    static  string logName;
    static string solutionName;
    static string boardName;
public:
    explicit Log(int N);
    static void dot_console(int i);
    static void to_console(int i, int cnt);
    static void logRot(int rotCnt);
    static void logWorse(int worseCnt);
    static void tempAppend(const vector<int> &pos, int64_t progress, int cnt, void *owner);
    static string NowStr();
    static void saveToFile(const vector<int> &pos);
    static void drawBoard(vector<int> &v);
};
