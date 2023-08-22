#ifndef COVER_H
#define COVER_H

#include <set>
#include <vector>
#include <iostream>
#include <queue>

typedef struct ListNode2 {
    public:
    bool val;
    struct ListNode2* left;
    struct ListNode2* right;
    struct ListNode2* up;
    struct ListNode2* down;
    struct ListNode2* colHead;
    
    int row;
    int col;
    
    int colNodes;
    
    ListNode2() : val(0), colNodes(0), row(-1), col(-1), left(nullptr), right(nullptr), up(nullptr), down(nullptr), colHead(nullptr) {
        
    }
} Node;

class DLX {
    public:
    std::vector<std::vector<Node>> list;
    Node* head;
    int n, m;
    
    std::vector<Node*> sol;
    std::set<int> hiddenCols;

    int inc(int x, int mod);

    int dec(int x, int mod);
    
    void translate(std::vector<std::vector<bool>>& mat2); // mat2 does not have a row representing head nodes.
    
    void dispInfo(Node* n);
    
    void dispCols();

    void dispMat(bool showData);
    
    void hide(Node* src);
    
    void show(Node* src);
    
    Node* minCol();
    
    void dispSol();
    
    // Always start with search(0)
    bool solve(int src);
};

#endif
