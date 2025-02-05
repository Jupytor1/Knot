#include "../header.hpp"

int main(int argc, char *argv[]){
    if (argc != 2){
        cout << "Usage: ./calc_jones_poly.exe [filename]" << endl;
        return 0;
    }

    ll step;
    KNOT::TreeNode *root;
    KNOT::TreeNode *cur_node;
    KNOT::load_state(&root, &step, &cur_node, argv[1]);
    while (root->calc_done == false){
        KNOT::calcJP_1step_forward(&cur_node);
        cout << "step: " << step << endl;
        if (cur_node != nullptr){
            cur_node->knot.print_attr();
            cout << endl;
        }
        step++;
        if (step % 3000000 == 0){
            string filename = KNOT::getLogFilename();
            KNOT::save_state(root, step, cur_node, "log/" + filename);
        }
    }

    root->knot.print_connection(cout);
    cout << endl;

    cout << "Jones Polynomial:" << endl;
    cout << "digmin: " << root->knot.jones_poly.digmin << endl;
    cout << "digmax: " << root->knot.jones_poly.digmax << endl;
    cout << "{ ";
    for (int coef: root->knot.jones_poly.coef){
        cout << coef << " ";
    }
    cout << "}" << endl;

    delete root;

    return 0;
}