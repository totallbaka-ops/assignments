#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Binary search for numeric keys
int binarySearchKey(const vector<unsigned long long> &keys, unsigned long long x) {
    int low = 0, high = (int)keys.size() - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (keys[mid] == x) return mid;
        else if (keys[mid] < x) low = mid + 1;
        else high = mid - 1;
    }
    return -(low + 1);
}

// Binary search for strings (lexicographically)
int binarySearchString(const vector<string> &pool, const string &s) {
    int low = 0, high = (int)pool.size() - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (pool[mid] == s) return mid;
        else if (pool[mid] < s) low = mid + 1;
        else high = mid - 1;
    }
    return -(low + 1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<unsigned long long> keys;  // for query keys
        vector<int> valueIdx;             // index into pool
        vector<string> pool;              // string pool

        for (int q = 0; q < n; q++) {
            int type;
            cin >> type;

            if (type == 0) {
                unsigned long long i;
                string s;
                cin >> i >> s;

                // Find or insert string in pool
                int pIdx = binarySearchString(pool, s);
                if (pIdx < 0) {
                    pIdx = -(pIdx + 1);
                    pool.insert(pool.begin() + pIdx, s);
                }

                // Map integer key → pooled string index
                int idx = binarySearchKey(keys, i);
                if (idx >= 0) {
                    valueIdx[idx] = pIdx;  // update
                } else {
                    idx = -(idx + 1);
                    keys.insert(keys.begin() + idx, i);
                    valueIdx.insert(valueIdx.begin() + idx, pIdx);
                }

            } else if (type == 1) {
                unsigned long long i;
                cin >> i;
                int idx = binarySearchKey(keys, i);
                if (idx >= 0)
                    cout << pool[valueIdx[idx]] << "\n";
                else
                    cout << 0 << "\n";
            }
        }
    }
    return 0;
}
