#include <set>
#include <cmath>
#include "scores.h"
#include <tuple>
#include <cassert>
#include <algorithm>

void swapTab(vector<int> &pos, int i, int j) {
    int aux = pos[i];
    pos[i]=pos[j];
    pos[j]=aux;
}

static bool threatens(int row_a, int col_a, int row_b, int col_b)
{
    return row_a == row_b // same row
           or col_a == col_b // same column
           or std::abs(row_a - row_b) == std::abs(col_a - col_b); // diagonal
}

int thrCnt(const std::vector<int> &pos)
{
    int end_idx = pos.size();
    int cnt = 0;
    for (int row_a = 0; row_a < end_idx; row_a++) {
        for (int row_b = row_a + 1; row_b < end_idx; row_b++) {
            int col_a = pos[row_a];
            int col_b = pos[row_b];
            if (threatens(row_a, col_a, row_b, col_b)) {
                cnt++;
            }
        }
    }
    return cnt;
}

int diff2Thr(std::vector<int> &pos, int a, int b) {
    int N = pos.size();
    int cnt1 = 0;
    int row_a = a;
    for (int row_b = 0; row_b < N; row_b++) {
        if (row_b==row_a) continue;
        int col_a = pos[row_a];
        int col_b = pos[row_b];
        if (threatens(row_a, col_a, row_b, col_b)) {
            cnt1++;
        }
    }
    row_a = b;
    for (int row_b = 0; row_b < N; row_b++) {
        if (row_b==row_a) continue;
        int col_a = pos[row_a];
        int col_b = pos[row_b];
        if (threatens(row_a, col_a, row_b, col_b)) {
            cnt1++;
        }
    }
    swapTab(pos, a,b);
    int cnt2 = 0;
    row_a = a;
    for (int row_b = 0; row_b < N; row_b++) {
        if (row_b==row_a) continue;
        int col_a = pos[row_a];
        int col_b = pos[row_b];
        if (threatens(row_a, col_a, row_b, col_b)) {
            cnt2++;
        }
    }
    row_a = b;
    for (int row_b = 0; row_b < N; row_b++) {
        if (row_b==row_a) continue;
        int col_a = pos[row_a];
        int col_b = pos[row_b];
        if (threatens(row_a, col_a, row_b, col_b)) {
            cnt2++;
        }
    }
    swapTab(pos, a,b);
    return cnt2-cnt1;
}
