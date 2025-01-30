#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <cmath>
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

#include <ctime>
#include <iomanip> // For std::put_time (C++20 and later, or use <sstream> and manual formatting)

#define flush fflush(stdout)
#define endl '\n'
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> P;
typedef pair<ll, ll> Pl;
const int INF = 1e9 + 1e6;
const ll LINF = 1e18 + 1e9;

#define rep0(i, n) for (i = 0; i < n; i++)
#define rep1(i, a, b) for (i = a; i < b; i++)
#define reprev(i, n) for (i = n - 1; i >= 0; i--)

#define lnprint(b) if (b){cout << endl;}else{cout << ' ';}


namespace KNOT{

// ========== CLASSES ==========

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

    //bool calc_jones_poly(int depth);

    void read_connection(std::istream &fin);

    void print_attr(void);

    void print_connection(std::ostream &fout);

};

class TreeNode{
    public:
    Knot knot;
    bool calc_done;
    TreeNode *par;
    TreeNode *L0, *Lplus, *Lminus;
    int type; // 0: L0,  1: L+,  -1: L-,  2: None(root)
    int temp_id;

    TreeNode(void);

    //bool calc_jones_poly(int depth);

    ~TreeNode(void);

};


// ========== FUNCTIONS ==========

void calcJP_1step_forward(TreeNode **node);

void reset_temp_id(TreeNode *node);

void assign_temp_id(TreeNode *root, vector<TreeNode*> *nodes);

bool save_state(TreeNode *root, TreeNode *cur_node, string filename);

bool load_state(TreeNode **root, TreeNode **cur_node, string filename);

string getLogFilename(void);

}

