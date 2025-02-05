#include "../header.hpp"

void KNOT::Knot::init(int itsc_n_init, int simple_loop_num_init){
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

void KNOT::Knot::count_loop(void){
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

void KNOT::Knot::connect(Channel in_ch, Channel out_ch){
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

void KNOT::Knot::simplify_knot(void){
    int i, j;
    // deactivate unnecessary intersections
    // i.e. change the type of an intersection into 2
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

void KNOT::Knot::read_connection(std::istream &fin){
    int i, j;
    fin >> itsc_n;
    fin >> simple_loop_num;
    init(itsc_n, simple_loop_num);
    rep0(i, itsc_n){
        fin >> itsc[i].type;
        rep0(j, 4){
            int partner, ch;
            fin >> partner >> ch;
            itsc[i].ch[j].partner_itsc_id = partner;
            itsc[i].ch[j].partner_ch = ch;
        }
    }
}

void KNOT::Knot::print_attr(void){
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

void KNOT::Knot::print_connection(std::ostream &fout){
    int i, j;
    fout << itsc_n << endl;
    fout << simple_loop_num << endl;
    rep0(i, itsc_n){
        fout << itsc[i].type;
        rep0(j, 4){
            fout << ' ' << itsc[i].ch[j].partner_itsc_id << ' ' << itsc[i].ch[j].partner_ch;
        }
        fout << endl;
    }
}
