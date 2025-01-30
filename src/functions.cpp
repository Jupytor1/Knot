#include "../header.hpp"

void KNOT::calcJP_1step_forward(TreeNode **node){
    if ((*node)->knot.itsc_n == 0){
        int i;
        Polynomial r = {-1, 1, vector<int>{-1, 0, -1}};
        (*node)->knot.jones_poly = {0, 0, vector<int>{1}};
        rep0(i, (*node)->knot.simple_loop_num - 1){
            (*node)->knot.jones_poly = (*node)->knot.jones_poly * r;
        }
        (*node)->calc_done = true;
        *node = (*node)->par;
        return;
    }
    if ((*node)->L0){
        // there are already 2 children
        TreeNode **the_other_child;
        bool origin_is_Lplus;
        if ((*node)->Lplus){
            the_other_child = &((*node)->Lplus);
            origin_is_Lplus = false;
        }else{
            the_other_child = &((*node)->Lminus);
            origin_is_Lplus = true;
        }
        if ((*node)->L0->calc_done == false){
            *node = (*node)->L0;
        }else if ((*the_other_child)->calc_done == false){
            *node = *the_other_child;
        }else{
            Polynomial c, d;
            if (origin_is_Lplus){
                c = {1, 3, vector<int>{-1, 0, 1}};
                d = {4, 4, vector<int>{1}};
            }else{
                c = {-3, -1, vector<int>{1, 0, -1}};
                d = {-4, -4, vector<int>{1}};
            }
            (*node)->knot.jones_poly = (*node)->L0->knot.jones_poly * c + (*the_other_child)->knot.jones_poly * d;
            delete (*node)->L0;
            (*node)->L0 = nullptr;
            delete (*the_other_child);
            (*the_other_child) = nullptr;
            (*node)->calc_done = true;
            *node = (*node)->par;
        }
    }else{
        // there are no children yet, so create them
        int inv_itsc = 0;
        while (inv_itsc < (*node)->knot.itsc_n){
            KNOT::Knot a, b;
            a = (*node)->knot;
            b = (*node)->knot;
            // a: L0
            a.itsc[inv_itsc].type = 2;
            if (a.itsc[inv_itsc].ch[0].partner_itsc_id == inv_itsc && a.itsc[inv_itsc].ch[3].partner_itsc_id == inv_itsc
                && a.itsc[inv_itsc].ch[0].partner_ch == 3 && a.itsc[inv_itsc].ch[3].partner_ch == 0){
                a.simple_loop_num++;
            }else{
                a.connect(a.itsc[inv_itsc].ch[0], a.itsc[inv_itsc].ch[3]);
            }
            if (a.itsc[inv_itsc].ch[1].partner_itsc_id == inv_itsc && a.itsc[inv_itsc].ch[2].partner_itsc_id == inv_itsc
                && a.itsc[inv_itsc].ch[1].partner_ch == 2 && a.itsc[inv_itsc].ch[2].partner_ch == 1){
                a.simple_loop_num++;
            }else{
                a.connect(a.itsc[inv_itsc].ch[1], a.itsc[inv_itsc].ch[2]);
            }
            // b: L+/L-
            b.itsc[inv_itsc].type *= -1;
            // simplify a and b
            a.simplify_knot();
            b.simplify_knot();
            if (a.itsc_n < (*node)->knot.itsc_n && b.itsc_n < (*node)->knot.itsc_n){
                (*node)->L0 = new TreeNode;
                (*node)->L0->knot = a;
                (*node)->L0->par = (*node);
                (*node)->L0->type = 0;
                if ((*node)->knot.itsc[inv_itsc].type == 1){
                    (*node)->Lminus = new TreeNode;
                    (*node)->Lminus->knot = b;
                    (*node)->Lminus->par = (*node);
                    (*node)->Lminus->type = -1;
                }else{
                    (*node)->Lplus = new TreeNode;
                    (*node)->Lplus->knot = b;
                    (*node)->Lplus->par = (*node);
                    (*node)->Lplus->type = 1;
                }
                return;
            }
            inv_itsc++;
        }
    }
}

void KNOT::reset_temp_id(TreeNode *node){
    node->temp_id = -1;
    if (node->L0) reset_temp_id(node->L0);
    if (node->Lplus) reset_temp_id(node->Lplus);
    if (node->Lminus) reset_temp_id(node->Lminus);
}

void KNOT::assign_temp_id(TreeNode *root, vector<TreeNode*> *nodes){
    TreeNode *node;
    int id;
    reset_temp_id(root);
    node = root;
    id = 0;
    while (node){
        if (node->temp_id == -1){
            node->temp_id = id;
            nodes->push_back(node);
            id++;
        }
        if (node->L0 != nullptr && node->L0->temp_id == -1){
            node = node->L0;
        }else if (node->Lplus != nullptr && node->Lplus->temp_id == -1){
            node = node->Lplus;
        }else if (node->Lminus != nullptr && node->Lminus->temp_id == -1){
            node = node->Lminus;
        }else{
            node = node->par;
        }
    }
}

bool KNOT::save_state(KNOT::TreeNode *root, KNOT::TreeNode *cur_node, string filename){
    std::ofstream fout(filename);
    if (!fout){
        cout << "File not opened." << endl;
        return false;
    }
    int i;
    vector<TreeNode*> nodes;
    KNOT::assign_temp_id(root, &nodes);
    int node_num = nodes.size();
    // define the tree structure
    fout << node_num << endl;
    rep1(i, 1, node_num){
        fout << nodes[i]->par->temp_id << ' ' << i << ' ' << nodes[i]->type << endl;
    }
    fout << endl;
    // id of the current node
    fout << cur_node->temp_id << endl;
    fout << endl;
    // detailed information for each node
    rep0(i, node_num){
        fout << i << endl;
        fout << ((nodes[i]->calc_done)? 1 : 0) << endl;
        nodes[i]->knot.print_connection(fout);
        if (nodes[i]->calc_done){
            fout << nodes[i]->knot.jones_poly.digmin << ' ' << nodes[i]->knot.jones_poly.digmax << endl;
            for (int coef: nodes[i]->knot.jones_poly.coef){
                fout << coef << ' ';
            }
            fout << endl;
        }
        fout << endl;
    }
    fout.close();
    return true;
}

bool KNOT::load_state(KNOT::TreeNode **root, KNOT::TreeNode **cur_node, string filename){
    std::ifstream fin(filename);
    if (!fin){
        cout << "File not opened." << endl;
        return false;
    }
    int i;
    int node_num;
    fin >> node_num;
    vector<TreeNode*> nodes;
    rep0(i, node_num){
        nodes.push_back(new TreeNode);
        nodes[i]->temp_id = i;
    }
    *root = nodes[0];
    rep0(i, node_num - 1){
        int par, ch, type;
        fin >> par >> ch >> type;
        nodes[ch]->par = nodes[par];
        nodes[ch]->type = type;
        if (type == 0){
            nodes[par]->L0 = nodes[ch];
        }else if (type == 1){
            nodes[par]->Lplus = nodes[ch];
        }else if (type == -1){
            nodes[par]->Lminus = nodes[ch];
        }
    }
    int cur_node_id;
    fin >> cur_node_id;
    *cur_node = nodes[cur_node_id];
    rep0(i, node_num){
        int node_id;
        int calc_done;
        fin >> node_id;
        fin >> calc_done;
        nodes[node_id]->calc_done = (calc_done == 1);
        nodes[node_id]->knot.read_connection(fin);
        if (calc_done == 1){
            int digmin, digmax;
            fin >> digmin >> digmax;
            nodes[node_id]->knot.jones_poly.digmin = digmin;
            nodes[node_id]->knot.jones_poly.digmax = digmax;
            int j;
            rep1(j, digmin, digmax + 1){
                int coef;
                fin >> coef;
                nodes[node_id]->knot.jones_poly.coef.push_back(coef);
            }
        }
    }
    fin.close();
    return true;
}

string KNOT::getLogFilename(void){
    time_t now = time(0); // Get current time
    tm* timeinfo = localtime(&now); // Convert to local time

    char buffer[80];
    // Format the date as YYYY-MM-DD (you can change the format string as needed)
    strftime(buffer, sizeof(buffer), "%Y-%m-%d-%H-%M-%S", timeinfo);

    string filename = "log_" + string(buffer) + ".txt";
    return filename;
}

