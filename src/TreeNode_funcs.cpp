#include "../header.hpp"

KNOT::TreeNode::TreeNode(void){
    calc_done = false;
    L0 = nullptr;
    Lplus = nullptr;
    Lminus = nullptr;
    par = nullptr;
    type = 2;
}

/*
bool KNOT::TreeNode::calc_jones_poly(int depth){
    int i;
    knot.simplify_knot();
    if (knot.itsc_n == 0){
        Polynomial r = {-1, 1, vector<int>{-1, 0, -1}};
        knot.jones_poly = {0, 0, vector<int>{1}};
        rep0(i, knot.simple_loop_num - 1){
            knot.jones_poly = knot.jones_poly * r;
        }
        calc_done = true;
        return true;
    }
    int inv_itsc = 0;
    while (inv_itsc < knot.itsc_n){
        KNOT::Knot a, b;
        a = knot;
        b = knot;
        // a: L0
        a.itsc[inv_itsc].type = 2;
        if (a.itsc[inv_itsc].ch[0].partner_itsc_id == inv_itsc && a.itsc[inv_itsc].ch[3].partner_itsc_id == inv_itsc
            && a.itsc[inv_itsc].ch[0].partner_ch == 3 && a.itsc[inv_itsc].ch[3].partner_ch == 0){
            knot.simple_loop_num++;
        }else{
            a.connect(a.itsc[inv_itsc].ch[0], a.itsc[inv_itsc].ch[3]);
        }
        if (a.itsc[inv_itsc].ch[1].partner_itsc_id == inv_itsc && a.itsc[inv_itsc].ch[2].partner_itsc_id == inv_itsc
            && a.itsc[inv_itsc].ch[1].partner_ch == 2 && a.itsc[inv_itsc].ch[2].partner_ch == 1){
            knot.simple_loop_num++; 
        }else{
            a.connect(a.itsc[inv_itsc].ch[1], a.itsc[inv_itsc].ch[2]);
        }
        // b: L+/L-
        b.itsc[inv_itsc].type *= -1;
        // simplify a and b
        a.simplify_knot();
        b.simplify_knot();
        if (a.itsc_n < knot.itsc_n && b.itsc_n < knot.itsc_n){
            knot_num++;
            cout << "knot_num: " << knot_num << endl;
            cout << "depth: " << depth + 1 << endl;
            a.print_attr();
            if (a.calc_jones_poly(depth + 1) == false){
                return false;
            }
            knot_num++;
            cout << "knot_num: " << knot_num << endl;
            cout << "depth: " << depth + 1 << endl;
            b.print_attr();
            if (b.calc_jones_poly(depth + 1) == false){
                return false;
            }
            // calculate Jones polynomial
            if (knot.itsc[inv_itsc].type == 1){
                Polynomial c = {1, 3, vector<int>{-1, 0, 1}};
                Polynomial d = {4, 4, vector<int>{1}};
                knot.jones_poly = a.jones_poly * c + b.jones_poly * d;
            }else{
                Polynomial c = {-3, -1, vector<int>{1, 0, -1}};
                Polynomial d = {-4, -4, vector<int>{1}};
                knot.jones_poly = a.jones_poly * c + b.jones_poly * d;
            }
            calc_done = true;
            return true;
        }
        inv_itsc++;
    }
    return false;
}
*/

KNOT::TreeNode::~TreeNode(void){
    if (L0) delete L0;
    if (Lplus) delete Lplus;
    if (Lminus) delete Lminus;
}
