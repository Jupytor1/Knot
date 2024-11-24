#include "../header.hpp"

int main(void){
    int i, j;
    int n;
    
    cin >> n;
    KNOT::Knot kn;
    kn.init(n, 0);
    rep0(i, n){
        cin >> kn.itsc[i].type;
        rep0(j, 4){
            int partner, ch;
            cin >> partner >> ch;
            kn.itsc[i].ch[j].partner_itsc_id = partner - 1;
            kn.itsc[i].ch[j].partner_ch = ch;
        }
    }

    kn.count_loop();
    kn.simplify_knot();
    kn.print();

    kn.calc_jones_poly();

    cout << "Jones Polynomial:" << endl;
    cout << "digmin: " << kn.jones_poly.digmin << endl;
    cout << "digmax: " << kn.jones_poly.digmax << endl;
    cout << "{ ";
    for (int coef: kn.jones_poly.coef){
        cout << coef << " ";
    }
    cout << "}" << endl;

    return 0;
}