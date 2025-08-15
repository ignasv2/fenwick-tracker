#include <iostream>
#include <string>
#include "Fenwick.h"
#include <fstream>
#include <sstream>

using namespace std;

// prints the menu of options when called
static void print_menu() {
    cout << "\nFenwick Tree Demo (Binary Indexed Tree)\n";
    cout << "#################################\n";
    cout << "1) add <index> <delta>                      (point update + show update path)\n";
    cout << "2) prefix <index>                           (prefix sum + show summation path)\n";
    cout << "3) range <leftIndex> <rightIndex>           (range sum)\n";
    cout << "4) reset                                    (zeroes out all data)\n";
    cout << "5) size                                     (print maximum index)\n";
    cout << "6) load CSV                                 (load csv file from data folder)\n";    
    cout << "0) exit                                     (exit the program)\n";
    cout << "#################################\n";
}

// For this next function I had to try to piece the code together from different sournces here
// These are the ones I relied on
// [FT-1] CP-Algorithms — Fenwick Tree (Binary Indexed Tree), incl. range-sum identity
//        https://cp-algorithms.com/data_structures/fenwick.html
// [FT-2] GeeksforGeeks — Binary Indexed Tree or Fenwick Tree
//        https://www.geeksforgeeks.org/binary-indexed-tree-or-fenwick-tree-2/
// [FT-3] P. M. Fenwick, “A New Data Structure for Cumulative Frequency Tables,”
//        https://onlinelibrary.wiley.com/doi/pdf/10.1002/spe.4380240306
void show_local_window(const FenwickTree& fenw, int center, int halfwidth) {
    int n = fenw.size();
    if (n<=0) return;

    int L = center - halfwidth;
    if (L<1) L = 1;
    int R = center + halfwidth;
    if (R > n) R = n;

    cout << "Local buckets [" << L << ".." << R << "]:" << endl;
    for (int i=L; i<=R; i++) {
        long long prev = fenw.prefix_sum(i-1);
        long long curr = fenw.prefix_sum(i);
        long long bucket_val = curr - prev;
        cout << " " << i << ": " << bucket_val << endl;
    }
}

// For CsvRow and parse_csv_row, I used information that I found from the following:
// [CSV-1] std::getline (overload with custom delimiter) —
//         https://en.cppreference.com/w/cpp/string/basic_string/getline
// [CSV-2] std::istringstream —
//         https://en.cppreference.com/w/cpp/io/basic_istringstream
// [CSV-3a] std::stod (string to double) —
//          https://en.cppreference.com/w/cpp/string/basic_string/stof
// [CSV-3b] std::stoll (string to long long) —
//          https://en.cppreference.com/w/cpp/string/basic_string/stol
struct CsvRow {
    string ts;
    double close;
    long long volume;
    int bucket;
};

bool parse_csv_row(const string& line, CsvRow& out) {
    istringstream iss(line);
    string ts, open, high, low, close, volume;
    if (getline(iss, ts, ',') &&
        getline(iss, open, ',') &&
        getline(iss, high , ',') &&
        getline(iss, low, ',') &&
        getline(iss, close, ',') &&
        getline(iss, volume, ',')) {
        try {
            out.ts = ts;
            out.close = stod(close);
            out.volume = stoll(volume);
            out.bucket = (int)out.close;
        }
        catch (...) {
            return false;
        }
        return true;
    }
    return false;
}

// File I/O + Fenwick updates:
// [IO-1] std::ifstream (file input stream) —
//        https://en.cppreference.com/w/cpp/io/basic_ifstream
// [IO-2] std::string::c_str (C-string view for APIs) —
//        https://en.cppreference.com/w/cpp/string/basic_string/c_str
// [IO-3] std::getline for line-by-line reading —
//        https://en.cppreference.com/w/cpp/string/basic_string/getline
// Fenwick background and range/prefix identity (for applied updates/queries):
// [FT-1] CP-Algorithms — https://cp-algorithms.com/data_structures/fenwick.html
// [FT-2] GeeksforGeeks — https://www.geeksforgeeks.org/binary-indexed-tree-or-fenwick-tree-2/
void load_csv_batch(const string& path, FenwickTree& fenw) {
    ifstream fin(path.c_str());
    if (!fin) {
        cerr << "Unable to open " << path << endl;
        return;
    }
    string line;
    // header
    getline(fin, line);

    long long rows = 0;
    long long applied = 0;
    long long bad = 0;
    long long oob = 0;
    long long total_vol = 0;

    while (getline(fin, line)) {
        rows++;
        CsvRow r;
        if (!parse_csv_row(line, r)) {
            bad++;
            continue;
        }
        if (r.bucket >= 1 && r.bucket <= fenw.size()) {
            fenw.add(r.bucket, r.volume);
            applied++;
            total_vol += r.volume;
        }
        else {
            oob++;
        }
    }
    cout << "Loaded '" << path << "': rows=" << rows
        << ", applied=" << applied
        << ", out_of_bounds=" << oob
        << ", bad=" << bad
        << ", total_volume=" << total_vol << endl;
}

int main() {
    // ask user to choose the capacity of the Fenwick Tree
    cout << "Enter maximum index (n): ";
    int n;
    // checks if input is invalid and exits if that's the case
    if (!(cin >> n)) {
        cout << "Invalid input. Ending the program.\n";
        return 0;
    }

    // builds FenwickTree with n elements in it
    FenwickTree fenwick(n);
    print_menu();

    // main loop
    while (true) {
        cout << "\nEnter command (0=exit, 1=add, 2=prefix, 3=range, 4=reset, 5=size, 6=load csv): ";
        int cmd;
        // checks if the command chosen is one of the integer choices, and exits if that's not the case
        if (!(cin >> cmd)) {
            cout << "Input is invalid. Ending the program.\n";
            break;
        }
        
        // exits the program if called
        if (cmd == 0) {
            cout << "Till the next time!\n";
            break;
        }
        // adds delta to tree at the index
        else if (cmd == 1) {
            int idx;
            long long delta;
            if (cin >> idx >> delta) {
                // checks to see if index is in the valid range
                if (idx < 1 || idx > fenwick.size()) {
                    cout << "The index is out of range. Valid choices are 1 through " << fenwick.size() << "\n";
                }
                else {
                    // performs a point update
                    fenwick.add(idx, delta);
                    cout << "Added " << delta << " at index " << idx << ".\n";
                    cout << "prefix(" << idx << ") = " << fenwick.prefix_sum(idx) << "\n";
                    show_local_window(fenwick, idx, 2);
                }
            }
            else {
                cout << "Choice: 1 <index> <delta>\n";
                cin.clear();
                string junk;
                getline(cin, junk);
            }
        }
        // prefix sum up to index
        else if (cmd == 2) {
            int idx;
            if (cin >> idx) {
                long long ans = fenwick.prefix_sum(idx);
                cout << "prefix(" << idx << ") = " << ans << "\n";

                int center = idx;
                if (center < 1) center = 1;
                int nmax = fenwick.size();
                if (center > nmax) center = nmax;
                show_local_window(fenwick, center, 2);
            }
            else {
                cout << "Choice: 2 <index>\n";
                cin.clear();
                string junk;
                getline(cin, junk);
            }
        }

        // calculated the sum over a range from leftIndex to rightIndex
        else if (cmd == 3) {
            // range
            int leftIndex, rightIndex;
            if (cin >> leftIndex >> rightIndex) {
                long long total = fenwick.range_sum(leftIndex, rightIndex);
                cout << "range_sum(" << leftIndex << ".." << rightIndex << ") = " << total << "\n";
            }
            else {
                cout << "Choice: 3 <leftIndex> <rightIndex>\n";
                cin.clear();
                string junk;
                getline(cin, junk);
            }
        }
        // resets the data in Fenwick Tree to zero
        else if (cmd == 4) {
            fenwick.reset();
            cout << "The tree has been reset.\n";
        }
        // prints out how many elements the tree actually supports
        else if (cmd == 5) {
            cout << "Max index (size) = " << fenwick.size() << "\n";
        }
        // command to load csv file from data folder
        else if (cmd == 6) {
            load_csv_batch("../data/intraday.csv", fenwick);
            cout << "Loaded intraday.csv into Fenwick tree\n";
        }
        // handles unaccounted for commands
        else {
            cout << "You have entered an unknown command, please try again.\n";
            print_menu();
        }
    }
    return 0;
}