#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
#include <sstream>
#include <tuple>
#include "scores.h"
#include "Log.h"
#include "Perm.h"
#include "ArgParser.h"

using namespace std;

class StopWatch
{
public:
    chrono::time_point<chrono::high_resolution_clock> a, b;
    void start() { a = chrono::high_resolution_clock::now(); }
    void stop() { b = chrono::high_resolution_clock::now(); }
    double duration() const
    {
        chrono::duration<double> elapsed_seconds = b - a;
        return elapsed_seconds.count();
    }
};

vector<int> rotate(vector<int> &pos, int delta) {
    vector<int> res(pos.size());
    if (delta>0) { //left
        for (int i=0; i<pos.size()-delta; i++)
            res[i] = pos[i+delta];
        for (int i=pos.size()-delta; i<pos.size(); i++)
            res[i] = pos[i-(pos.size()-delta)];
    } else {
        delta = -delta;
        for (int i=0; i<delta; i++)
            res[i] = pos[i+(pos.size()-delta)];
        for (int i=delta; i<pos.size(); i++)
            res[i] = pos[i-delta];
    }
    return res;
}

int tryrot(vector<int> &pos) {
    pos = rotate(pos, 1);
    int tryCnt = thrCnt(pos);
    pos = rotate(pos, -1);
    return tryCnt;
}

void rot(vector<int> &pos) {
    rotate(pos.begin(), pos.begin()+1, pos.end());
}

void initPos(vector<int> &v, int n, const string boardS) {
    stringstream ss(boardS);
    for (int i=0; i<n; i++)
    {
        int k;
        ss>>k;
        v.push_back(k-1);
    }
}

int main(int argc, char *argv[]) {
    elap::ArgParser parser;
    parser.addParam('n', "n", 1, 1);
    if (!parser.parse(argc, argv)) {
        cout << parser.getError() << endl << endl;
        return 1;
    }
    elap::Param* param = parser.getParam('n');
    if (!param) {
        cout << "need param -n" << endl;
        exit(1);
    }
    int N = stoi(param->options[0]);

    Log log(N);
    int NThr=thread::hardware_concurrency();;
    int totrotcnt = 0;
    random_device r;
    mt19937 generator(300);
    int64_t maxlen = 0;
    StopWatch sw;
    printf("N=%d\n", N);
    fflush(stdout);
    vector<int> pos;

    fflush(stdout);
    sw.start();
    pos.clear();
    //initPos(pos, N, "222 281 338 224 194 59 76 261 286 292 62 103 207 328 245 66 356 225 87 120 176 353 113 339 132 64 209 181 130 166 210 145 104 45 268 312 256 221 362 58 75 254 287 160 302 191 337 102 392 184 203 111 296 94 47 264 398 29 144 165 283 395 248 152 288 280 304 241 9 195 68 340 119 232 306 179 317 276 54 107 333 331 125 290 204 170 142 373 399 393 275 324 357 244 80 35 8 28 344 386 269 69 48 215 295 311 38 182 25 60 173 410 36 108 193 267 409 134 307 23 346 243 326 334 169 186 52 138 216 91 226 101 180 55 18 316 408 325 278 110 358 273 172 336 27 272 246 390 61 63 257 380 7 53 405 310 178 20 5 86 236 124 77 121 391 22 34 105 265 135 155 332 320 247 129 319 163 112 70 187 117 341 359 16 277 82 253 260 206 394 167 168 360 51 201 279 375 230 126 78 382 17 368 285 200 372 4 89 26 412 404 369 258 233 309 116 213 379 185 71 323 114 212 148 314 84 3 374 33 401 343 174 274 354 383 141 11 371 388 149 83 150 266 400 2 355 234 81 30 21 31 65 46 228 348 321 220 293 217 10 384 106 136 322 329 363 131 250 12 282 299 219 159 153 255 100 24 396 73 42 50 411 413 188 43 183 259 67 44 370 97 147 140 376 403 318 72 289 19 377 350 171 40 211 303 79 300 223 98 109 335 313 74 189 352 143 6 251 270 41 407 198 218 252 139 13 406 123 93 387 202 305 385 162 15 151 197 381 365 39 239 291 249 347 57 154 235 164 156 1 297 351 205 99 115 315 122 199 327 96 49 128 192 90 175 402 367 238 271 378 157 14 308 242 85 237 214 263 32 88 298 56 366 133 262 364 177 37 349 229 158 240 231 196 118 208 345 127 95 92 294 146 190 284 161 137 397 361 227 389 330 301 342");
    for (int i = 0; i < N; i++) pos.push_back(i);
    shuffle(pos.begin(), pos.end(), generator);
    NewInterchangeData::cnt = thrCnt(pos);
    //permNode->initPerm();
    NewInterchangeData::solutionsSums.clear();
    int rotcnt = 0;
    int worsecnt = 0;
    int64_t i=0;
    while (true)
    {
        i++;
        auto perm = new Perm(&pos, NThr);
        int dcnt = perm->getBetter();
        if (dcnt<0)
        {
            NewInterchangeData::cnt+=dcnt;
            if (NewInterchangeData::cnt==0) break;
        } else {
            printf("%d-", NewInterchangeData::cnt);
            int tryCnt = tryrot(pos);
            bool b;
            if (tryCnt-NewInterchangeData::cnt>2) {
                worsecnt++;
                Log::logWorse(worsecnt);
                b = perm->tryWorse(tryCnt - NewInterchangeData::cnt);
            }
            else
                b = false;
            if (!b) {
                rot(pos);
                rotcnt++;
                totrotcnt++;
                Log::logRot(rotcnt);
                NewInterchangeData::cnt=thrCnt(pos);
                printf("%d rot(%d)\n",NewInterchangeData::cnt, totrotcnt);
            } else {
                NewInterchangeData::cnt=thrCnt(pos);
                printf("%d swap3 bo rot %d\n",NewInterchangeData::cnt, tryCnt);
                if (NewInterchangeData::cnt<tryCnt)
                    printf("zaoszczedzone bo %d<%d\n",NewInterchangeData::cnt,tryCnt);
            }
        }
        delete perm;
    }
    sw.stop();
    double dur = sw.duration();
    cout << "time = " << dur << endl;
    Log::saveToFile(pos);
    Log::drawBoard(pos);
    printf("\n");
    return 0;
}

