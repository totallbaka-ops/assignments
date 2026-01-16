#include <iostream>
#include <cstdlib>   // for rand()
#include <cmath>     // for pow()
using namespace std;


#define MAX_LEVEL 10   
#define P 0.5 

struct Node {
    int val;
    Node** forward; // array of forward pointers
    int level;      // number of levels this node spans

    Node(int val, int level) {
        this->val = val;
        this->level = level;
        forward = new Node*[level + 1];
        for (int i = 0; i <= level; i++)
            forward[i] = nullptr;
    }
};

class SkipList {
    int level;       // current highest level
    Node* head;      // header node

public:
    SkipList() {
        level = 0;
        head = new Node(-1, MAX_LEVEL);
    }

    int randomLevel() {
        int lvl = 0;
        while (((double)rand() / RAND_MAX) < P && lvl < MAX_LEVEL)
            lvl++;
        return lvl;
    }

    // 1️⃣ INSERT
    void insert(int val) {
        Node* update[MAX_LEVEL + 1];
        Node* curr = head;

        for (int i = level; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->val < val)
                curr = curr->forward[i];
            update[i] = curr;    //check each lane and when val>= given the stopp and insert
        }

        curr = curr->forward[0];
        int lvl = randomLevel();

        if (lvl > level) {
            for (int i = level + 1; i <= lvl; i++)
                update[i] = head;
            level = lvl;
        }

        Node* newNode = new Node(val, lvl);
        for (int i = 0; i <= lvl; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }

        print(); // print bottom level after every operation
    }

    // 2️⃣ DELETE all occurrences
    void deleteVal(int val) {
        Node* update[MAX_LEVEL + 1];
        Node* curr = head;

        for (int i = level; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->val < val)
                curr = curr->forward[i];
            update[i] = curr;
        }

        curr = curr->forward[0];

        while (curr && curr->val == val) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != curr) break;
                update[i]->forward[i] = curr->forward[i];
            }
            curr = curr->forward[0];
        }

        while (level > 0 && head->forward[level] == nullptr)
            level--;

        print();
    }

    // 3️⃣ SEARCH
    bool search(int val) {
        Node* curr = head;
        for (int i = level; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->val < val)
                curr = curr->forward[i];
        }
        curr = curr->forward[0];
        return (curr && curr->val == val);
    }

    // 4️⃣ COUNT OCCURRENCES
    int countOccurrences(int val) {
        int count = 0;
        Node* curr = head->forward[0];
        while (curr) {
            if (curr->val == val) count++;
            curr = curr->forward[0];
        }
        return count;
    }

    // 5️⃣ LOWER BOUND (>= e)
    int lowerBound(int val) {
        Node* curr = head;
        for (int i = level; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->val < val)
                curr = curr->forward[i];
        }
        curr = curr->forward[0];
        if (curr) return curr->val;
        return 0;
    }

    // 6️⃣ UPPER BOUND (> e)
    int upperBound(int val) {
        Node* curr = head;
        for (int i = level; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->val <= val)
                curr = curr->forward[i];
        }
        curr = curr->forward[0];
        if (curr) return curr->val;
        return 0;
    }

    // 7️⃣ CLOSEST ELEMENT
    int closestElement(int val) {
        Node* curr = head;
        for (int i = level; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->val < val)
                curr = curr->forward[i];
        }

        int left = curr->val;
        Node* rightNode = curr->forward[0];
        int right = (rightNode ? rightNode->val : left);

        if (left == -1) return right;
        if (abs(val - left) <= abs(right - val)) return left;
        return right;
    }

    // 📜 PRINT bottom level
    void print() {
        Node* curr = head->forward[0];
        while (curr) {
            cout << curr->val << " ";
            curr = curr->forward[0];
        }
        cout << "\n";
    }
};



int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    SkipList sl;

    int q;
    cin >> q; // number of queries

    while (q--) {
        string cmd;
        cin >> cmd;

        if (cmd == "I") {          // Insert
            int x;
            cin >> x;
            sl.insert(x);
        }
        else if (cmd == "D") {     // Delete
            int x;
            cin >> x;
            sl.deleteVal(x);
        }
        else if (cmd == "S") {     // Search
            int x;
            cin >> x;
            cout << (sl.search(x) ? "True" : "False") << "\n";
        }
        else if (cmd == "C") {     // Count occurrences
            int x;
            cin >> x;
            cout << sl.countOccurrences(x) << "\n";
        }
        else if (cmd == "L") {     // Lower bound
            int x;
            cin >> x;
            cout << sl.lowerBound(x) << "\n";
        }
        else if (cmd == "U") {     // Upper bound
            int x;
            cin >> x;
            cout << sl.upperBound(x) << "\n";
        }
        else if (cmd == "E") {     // Closest element
            int x;
            cin >> x;
            cout << sl.closestElement(x) << "\n";
        }
    }

    return 0;
}
