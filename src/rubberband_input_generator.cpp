#include "../header.hpp"

// Function for generating input for calculating Jones polynomial of 
// roundly connected N rubberband

int main(int argc, char *argv[]){
    int i, j;
    int n;

    if (argc != 2){
        cout << "Usage: ./rb_in_gen.exe N" << endl;
        return 0;
    }

    n = atoi(argv[1]);

    cout << n * 6 << endl;
    rep0(i, n){
        int prev, next;
        prev = (i + n - 1) % n;
        next = (i + 1) % n;
        // 1
        cout << "-1 " << prev * 6 + 3 << " " << 3 << " "
                      <<    i * 6 + 4 << " " << 2 << " "
                      <<    i * 6 + 2 << " " << 1 << " "
                      <<    i * 6 + 2 << " " << 0 << endl;
        // 2
        cout << "-1 " <<    i * 6 + 1 << " " << 3 << " "
                      <<    i * 6 + 1 << " " << 2 << " "
                      <<    i * 6 + 3 << " " << 1 << " "
                      <<    i * 6 + 3 << " " << 0 << endl;
        // 3
        cout << "-1 " <<    i * 6 + 2 << " " << 3 << " "
                      <<    i * 6 + 2 << " " << 2 << " "
                      <<    i * 6 + 6 << " " << 1 << " "
                      << next * 6 + 1 << " " << 0 << endl;
        // 4
        cout << " 1 " <<    i * 6 + 5 << " " << 3 << " "
                      <<    i * 6 + 5 << " " << 2 << " "
                      <<    i * 6 + 1 << " " << 1 << " "
                      << prev * 6 + 6 << " " << 0 << endl;
        // 5
        cout << " 1 " <<    i * 6 + 6 << " " << 3 << " "
                      <<    i * 6 + 6 << " " << 2 << " "
                      <<    i * 6 + 4 << " " << 1 << " "
                      <<    i * 6 + 4 << " " << 0 << endl;
        // 6
        cout << " 1 " << next * 6 + 4 << " " << 3 << " "
                      <<    i * 6 + 3 << " " << 2 << " "
                      <<    i * 6 + 5 << " " << 1 << " "
                      <<    i * 6 + 5 << " " << 0 << endl;
    }

    return 0;
}
