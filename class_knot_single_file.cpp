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

    void trim_zero(void){
        int i;
        vector<int> newcoef;
        int l, r;
        l = 0;
        while (l < digmax - digmin + 1 && coef[l] == 0){
            l++;
        }
        r = digmax - digmin;
        while (r > 0 && coef[r] == 0){
            r--;
        }
        i = l;
        while (i <= r){
            newcoef.push_back(coef[i]);
            i++;
        }
        digmin = l + digmin;
        digmax = r + digmin;
        coef = newcoef;
    }

    Polynomial operator+(const Polynomial &a){
        Polynomial res;
        int i;
        res.digmin = min(this->digmin, a.digmin);
        res.digmax = max(this->digmax, a.digmax);
        res.coef.resize(res.digmax - res.digmin + 1);
        rep0(i, this->digmax - this->digmin + 1){
            res.coef[i - res.digmin + this->digmin] += this->coef[i];
        }
        rep0(i, a.digmax - a.digmin + 1){
            res.coef[i - res.digmin + a.digmin] += a.coef[i];
        }
        res.trim_zero();
        return res;
    }

    Polynomial operator-(const Polynomial &a){
        Polynomial res;
        int i;
        res.digmin = min(this->digmin, a.digmin);
        res.digmax = max(this->digmax, a.digmax);
        res.coef.resize(res.digmax - res.digmin + 1);
        rep0(i, this->digmax - this->digmin + 1){
            res.coef[i - res.digmin + this->digmin] += this->coef[i];
        }
        rep0(i, a.digmax - a.digmin + 1){
            res.coef[i - res.digmin + a.digmin] -= a.coef[i];
        }
        res.trim_zero();
        return res;
    }

    Polynomial operator*(const Polynomial &a){
        Polynomial res;
        int i, j;
        res.digmin = this->digmin + a.digmin;
        res.digmax = this->digmax + a.digmax;
        res.coef.resize(res.digmax - res.digmin + 1);
        rep0(i, this->digmax - this->digmin + 1){
            rep0(j, a.digmax - a.digmin + 1){
                res.coef[i + j] += this->coef[i] * a.coef[j];
            }
        }
        res.trim_zero();
        return res;
    }

    Polynomial operator/(const Polynomial &a){
        Polynomial res;
        vector<int> this_vec = this->coef;
        int i, j;
        res.digmin = this->digmin - a.digmin;
        res.digmax = this->digmax - a.digmax;
        i = 0;
        while (i < res.digmax - res.digmin + 1){
            res.coef[i] = this_vec[this->digmax - this->digmin - i] / a.coef[a.digmax - a.digmin];
            rep0(j, a.digmax - a.digmin + 1){
                this_vec[this->digmax - this->digmin - i - j] -= a.coef[a.digmax - a.digmin - j] * res.coef[i];
            }
            i++;
        }
        res.trim_zero();
        return res;
    }
};

class Knot{
    public:
    vector<Intersection> itsc;
    int itsc_n;
    int simple_loop_num = 0; // only simple knots (just a loop)
    int loop_num = -1; // excluding the simple knots
    Polynomial jones_poly;

    void init(int itsc_n_init, int simple_loop_num_init){
        int i, j;
        itsc_n = itsc_n_init;
        simple_loop_num = simple_loop_num_init;
        itsc.resize(itsc_n);
        rep0(i, itsc_n){
            itsc[i].id = i;
            rep0(j, 4){
                itsc[i].ch[j].itsc_id = i;
                itsc[i].ch[j].channel_type = j;
            }
        }
    }

    void count_loop(void){
        int i;
        vector<bool> checked(itsc_n * 2, false);
        // checked[i*2]:   ch0/ch2 stream of ith intersection
        // checked[i*2+1]: ch1/ch3 stream of ith intersection
        Channel cur;
        loop_num = 0;
        while (true){
            rep0(i, itsc_n){
                if (checked[i * 2] == false){
                    cur = itsc[i].ch[0];
                    break;
                }
                if (checked[i * 2 + 1] == false){
                    cur = itsc[i].ch[1];
                    break;
                }
            }
            if (i == itsc_n){
                break;
            }
            loop_num++;
            while (true){
                if (checked[cur.itsc_id * 2 + cur.channel_type]){
                    break;
                }
                checked[cur.itsc_id * 2 + cur.channel_type] = true;
                int next_itsc_id;
                int next_ch;
                next_itsc_id = itsc[cur.itsc_id].ch[cur.channel_type + 2].partner_itsc_id;
                next_ch = itsc[cur.itsc_id].ch[cur.channel_type + 2].partner_ch;
                cur = itsc[next_itsc_id].ch[next_ch];
            }
        }
    }

    void connect(Channel in_ch, Channel out_ch){
        int from_itsc, from_ch;
        int to_itsc, to_ch;
        from_itsc = in_ch.partner_itsc_id;
        from_ch = in_ch.partner_ch;
        to_itsc = out_ch.partner_itsc_id;
        to_ch = out_ch.partner_ch;
        Channel *from, *to;
        from = &(itsc[from_itsc].ch[from_ch]);
        to = &(itsc[to_itsc].ch[to_ch]);
        from->partner_itsc_id = to->itsc_id;
        from->partner_ch = to->channel_type;
        to->partner_itsc_id = from->itsc_id;
        to->partner_ch = from->channel_type;
    }

    void simplify_knot(void){
        int i, j;
        // deactivate unnecessary intersections
        // that is, changing the type of an intersection into 2
        bool finished;
        finished = false;
        while (finished == false){
            finished = true;
            rep0(i, itsc_n){
                if (itsc[i].type == 2) continue;
                int in_ch, out_ch;
                in_ch = out_ch = -1;
                if (itsc[i].ch[0].partner_itsc_id == i && itsc[i].ch[3].partner_itsc_id == i
                    && itsc[i].ch[0].partner_ch == 3 && itsc[i].ch[3].partner_ch == 0){
                    in_ch = 1;
                    out_ch = 2;
                }else
                if (itsc[i].ch[1].partner_itsc_id == i && itsc[i].ch[2].partner_itsc_id == i
                    && itsc[i].ch[2].partner_ch == 1 && itsc[i].ch[1].partner_ch == 2){
                    in_ch = 0;
                    out_ch = 3;
                }
                if (in_ch != -1 && out_ch != -1){
                    finished = false;
                    if (itsc[i].ch[in_ch].partner_itsc_id == i && itsc[i].ch[out_ch].partner_itsc_id == i){
                        simple_loop_num++;
                    }else{
                        connect(itsc[i].ch[in_ch], itsc[i].ch[out_ch]);
                    }
                    itsc[i].type = 2;
                }
            }
            rep0(i, itsc_n){
                if (itsc[i].type == 2) continue;
                if (itsc[i].ch[2].partner_itsc_id == itsc[i].ch[3].partner_itsc_id
                    && itsc[i].ch[2].partner_ch == 1 && itsc[i].ch[3].partner_ch == 0
                    && itsc[i].type + itsc[itsc[i].ch[2].partner_itsc_id].type == 0){
                    finished = false;
                    Intersection *itsc1, *itsc2;
                    itsc1 = &itsc[i];
                    itsc2 = &(itsc[itsc[i].ch[2].partner_itsc_id]);
                    // itsc1/ch0 -> itsc2/ch3
                    if (itsc1->ch[0].partner_itsc_id == itsc2->id && itsc2->ch[3].partner_itsc_id == itsc1->id){
                        simple_loop_num++;
                    }else{
                        connect(itsc1->ch[0], itsc2->ch[3]);
                    }
                    // itsc1/ch1 -> itsc2/ch2
                    if (itsc1->ch[1].partner_itsc_id == itsc2->id && itsc2->ch[2].partner_itsc_id == itsc1->id){
                        simple_loop_num++;
                    }else{
                        connect(itsc1->ch[1], itsc2->ch[2]);
                    }
                    itsc1->type = 2;
                    itsc2->type = 2;
                }else
                if (itsc[i].ch[1].partner_itsc_id == itsc[i].ch[2].partner_itsc_id
                    && itsc[i].ch[1].partner_ch == 2 && itsc[i].ch[2].partner_ch == 1
                    && itsc[i].type + itsc[itsc[i].ch[1].partner_itsc_id].type == 0){
                    finished = false;
                    Intersection *itsc1, *itsc2;
                    itsc1 = &itsc[i];
                    itsc2 = &(itsc[itsc[i].ch[1].partner_itsc_id]);
                    // itsc1/ch0 -> itsc2/ch3
                    if (itsc1->ch[0].partner_itsc_id == itsc2->id && itsc2->ch[3].partner_itsc_id == itsc1->id){
                        simple_loop_num++;
                    }else{
                        connect(itsc1->ch[0], itsc2->ch[3]);
                    }
                    // itsc2/ch0 -> itsc1/ch3
                    if (itsc2->ch[0].partner_itsc_id == itsc1->id && itsc1->ch[3].partner_itsc_id == itsc2->id){
                        simple_loop_num++;
                    }else{
                        connect(itsc2->ch[0], itsc1->ch[3]);
                    }
                    itsc1->type = 2;
                    itsc2->type = 2;
                }else
                if (itsc[i].ch[0].partner_itsc_id == itsc[i].ch[3].partner_itsc_id
                    && itsc[i].ch[0].partner_ch == 3 && itsc[i].ch[3].partner_ch == 0
                    && itsc[i].type + itsc[itsc[i].ch[0].partner_itsc_id].type == 0){
                    finished = false;
                    Intersection *itsc1, *itsc2;
                    itsc1 = &itsc[i];
                    itsc2 = &(itsc[itsc[i].ch[0].partner_itsc_id]);
                    // itsc1/ch1 -> itsc2/ch2
                    if (itsc1->ch[1].partner_itsc_id == itsc2->id && itsc2->ch[2].partner_itsc_id == itsc1->id){
                        simple_loop_num++;
                    }else{
                        connect(itsc1->ch[1], itsc2->ch[2]);
                    }
                    // itsc2/ch1 -> itsc1/ch2
                    if (itsc2->ch[1].partner_itsc_id == itsc1->id && itsc1->ch[2].partner_itsc_id == itsc2->id){
                        simple_loop_num++;
                    }else{
                        connect(itsc2->ch[1], itsc1->ch[2]);
                    }
                    itsc1->type = 2;
                    itsc2->type = 2;
                }
            }
        }
        // create a new intersection vector without unnecessary ones
        // and replace the old vector with it
        vector<int> idx_projection(itsc_n, -1);
        vector<Intersection> new_itsc;
        int idx;
        idx = 0;
        rep0(i, itsc_n){
            if (itsc[i].type != 2){
                idx_projection[i] = idx;
                new_itsc.push_back(itsc[i]);
                idx++;
            }
        }
        int new_itsc_n = idx;
        rep0(i, new_itsc_n){
            new_itsc[i].id = idx_projection[new_itsc[i].id]; // == i
            rep0(j, 4){
                new_itsc[i].ch[j].itsc_id = idx_projection[new_itsc[i].ch[j].itsc_id]; // == i
                new_itsc[i].ch[j].partner_itsc_id = idx_projection[new_itsc[i].ch[j].partner_itsc_id];
            }
        }
        itsc_n = new_itsc_n;
        itsc = new_itsc;
        count_loop();
    }

    void calc_jones_poly(void){
        int i;
        //simplify_knot();
        if (itsc_n == 0){
            Polynomial r = {-1, 1, vector<int>{-1, 0, -1}};
            jones_poly = {0, 0, vector<int>{1}};
            rep0(i, simple_loop_num - 1){
                jones_poly = jones_poly * r;
            }
            return;
        }
        Knot lzero = (*this);
        if (lzero.itsc[0].ch[0].partner_itsc_id == 0 && lzero.itsc[0].ch[3].partner_itsc_id == 0){
            simple_loop_num++;
        }else{
            lzero.connect(lzero.itsc[0].ch[0], lzero.itsc[0].ch[3]);
        }
        if (lzero.itsc[0].ch[1].partner_itsc_id == 0 && lzero.itsc[0].ch[2].partner_itsc_id == 0){
            simple_loop_num++;
        }else{
            lzero.connect(lzero.itsc[0].ch[1], lzero.itsc[0].ch[2]);
        }
        lzero.itsc[0].type = 2;
        lzero.simplify_knot();
        lzero.print();
        lzero.calc_jones_poly();
        /*
        cout << "Jones Polynomial:" << endl;
        cout << "digmin: " << lzero.jones_poly.digmin << endl;
        cout << "digmax: " << lzero.jones_poly.digmax << endl;
        cout << "{ ";
        for (int coef: lzero.jones_poly.coef){
            cout << coef << " ";
        }
        cout << "}" << endl;
        cout << endl;
        */
        if (itsc[0].type == 1){
            Knot lminus;
            lminus = (*this);
            lminus.itsc[0].type = -1;
            lminus.simplify_knot();
            lminus.print();
            lminus.calc_jones_poly();
            /*
            cout << "Jones Polynomial:" << endl;
            cout << "digmin: " << lminus.jones_poly.digmin << endl;
            cout << "digmax: " << lminus.jones_poly.digmax << endl;
            cout << "{ ";
            for (int coef: lminus.jones_poly.coef){
                cout << coef << " ";
            }
            cout << "}" << endl;
            cout << endl;
            */
            Polynomial a = {1, 3, vector<int>{-1, 0, 1}};
            Polynomial b = {4, 4, vector<int>{1}};
            jones_poly = a * lzero.jones_poly + b * lminus.jones_poly;
        }else
        if (itsc[0].type == -1){
            Knot lplus;
            lplus = (*this);
            lplus.itsc[0].type = 1;
            lplus.simplify_knot();
            lplus.print();
            lplus.calc_jones_poly();
            /*
            cout << "Jones Polynomial:" << endl;
            cout << "digmin: " << lplus.jones_poly.digmin << endl;
            cout << "digmax: " << lplus.jones_poly.digmax << endl;
            cout << "{ ";
            for (int coef: lplus.jones_poly.coef){
                cout << coef << " ";
            }
            cout << "}" << endl;
            cout << endl;
            */
            Polynomial a = {-3, -1, vector<int>{1, 0, -1}};
            Polynomial b = {-4, -4, vector<int>{1}};
            jones_poly = a * lzero.jones_poly + b * lplus.jones_poly;
        }
    }

    void print(void){
        int i, j;
        cout << "simple loop num: " << simple_loop_num << endl;
        cout << "loop num: " << loop_num << endl;
        cout << "intersections:" << endl;
        rep0(i, itsc_n){
            cout << "itsc" << i << ": " << itsc[i].type << ",";
            rep0(j, 4){
                cout << " (" << itsc[i].ch[j].partner_itsc_id << ", " << itsc[i].ch[j].partner_ch << ")";
            }
            cout << endl;
        }
        cout << endl;
    }

};

}