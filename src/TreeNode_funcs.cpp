#include "../header.hpp"

KNOT::TreeNode::TreeNode(void){
    calc_done = false;
    L0 = nullptr;
    Lplus = nullptr;
    Lminus = nullptr;
    par = nullptr;
    type = 2;
}

KNOT::TreeNode::~TreeNode(void){
    if (L0) delete L0;
    if (Lplus) delete Lplus;
    if (Lminus) delete Lminus;
}
