#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <stack>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <random>
#include <climits>
#include <bitset>
//#include <bit>
//#include <tuple>
//#include <atcoder/modint>

#define flush fflush(stdout)
#define endl '\n'
#define all(v) v.begin(), v.end()
using namespace std;
//using namespace atcoder;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> P;
typedef pair<ll, ll> Pl;
const int mod1 = (int)1e9 + 7, mod2 = (int)998244353;
const int INF = 1e9 + 1e6;
const ll LINF = 1e18 + 1e9;
//const int INF = INT32_MAX - 100;
//const ll LINF = LLONG_MAX - 100;
const int di[9] = {1, 0, -1, 0, 1, 1, -1, -1, 0}, dj[9] = {0, 1, 0, -1, -1, 1, -1, 1, 0};

#define rep0(i, n) for (i = 0; i < n; i++)
#define rep1(i, a, b) for (i = a; i < b; i++)
#define reprev(i, n) for (i = n - 1; i >= 0; i--)

#define cumsum(v, l, r) (l <= 0)? v[r - 1] : (v[r - 1] - v[l - 1])

#define lnprint(b) if (b){cout << endl;}else{cout << ' ';}


namespace KNOT{

class Channel{
    public:
    int itsc_id;
    int channel_type; // 0, 1, 2, or 3
    int partner_itsc_id;
    int partner_ch;   // 0, 1, 2, or 3
};

class Intersection{
    public:
    int type; // 0: L0,  1: L+,  -1: L-,  2: deactivated
    int id;
    vector<Channel> ch = vector<Channel>(4);
};

class Polynomial{
    public:
    int digmin, digmax;
    vector<int> coef;

    void trim_zero(void);

    Polynomial operator+(const Polynomial &a);

    Polynomial operator-(const Polynomial &a);

    Polynomial operator*(const Polynomial &a);

    Polynomial operator/(const Polynomial &a);
};

class Knot{
    public:
    vector<Intersection> itsc;
    int itsc_n;
    int simple_loop_num = 0; // only simple knots (just a loop)
    int loop_num = -1; // excluding the simple knots
    Polynomial jones_poly;

    void init(int itsc_n_init, int simple_loop_num_init);

    void count_loop(void);

    void connect(Channel in_ch, Channel out_ch);

    void simplify_knot(void);

    void calc_jones_poly(void);

    void print(void);

};

}

