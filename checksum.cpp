#include "checksum.h"
#include "crc64.h"

uint64_t computeSum(const vector<int> &v) {
    uint64_t crc = 0;
    for (int i=0; i<v.size();i++){
        int n = v[i];
        crc = crc64((char*)&n, 4, crc);
        //sum += v[i] * (i + 1);
    }
    return crc;
}

uint64_t computeFutureSum(vector<int> &v, pair<int,int> p) {
    uint64_t crc = 0;
    for (int i=0; i<v.size();i++){
        int n;
        if (i==p.first)
            n = v[p.second];
        else if (i==p.second)
            n = v[p.first];
        else n = v[i];
        //sum += n * (i + 1);
        crc = crc64((char*)&n, 4, crc);
    }
    return crc;
}
