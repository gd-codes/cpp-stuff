/* CS202 Data Structures and Algorithms
Assignment 5 : Solution by group 25 */

#include <stack>
#include <cassert>
#include <sstream>
#include <vector>

#include <iostream>


/* 
- In a BST, insert & delete operations take O(depth) time
- In order to constrain these, a colour [bool] is additionally stored on
each node to make depth = O(lg n) by RBT properties.
- Since nodes containing inserted values may be internal or leaf nodes, 
implementing rank or select in O(lg n) time isn't feasible without knowing
the size of any subtree. So this is appended to each node and used along with
BST properties to be able to perform these operations without traversing
multiple branches. 
- RangeSum could be found ay adding values of the nodes accessed through
in-order traversal of the next (j-i) nodes from the node at rank i
However traversal takes Theta(n) time (> O(lg n)). So the sum of values of 
its subtree is also augmented in each node, and used along with size & BST
properties to get this faster.
*/


class TreeNode {

    protected :
        int val;
        bool red = false;
        int sum;
        int size = 1;

        bool doubleblack = false;

        TreeNode* lc = nullptr;
        TreeNode* rc = nullptr;

    friend class RBST;
    friend bool isNull(TreeNode*);

    public :
        TreeNode() {}
        TreeNode(int x, bool r=false) : val(x), red(r), sum(x) {}

};




class RBST {

    TreeNode* root = nullptr;

    void leftRotate(TreeNode*, TreeNode*);
    void rightRotate(TreeNode*, TreeNode*);
    void printSubtree(std::ostringstream&, const std::string&,
                      const TreeNode*, bool);
    void maintainRBT_ins(std::vector<TreeNode*>&);
    void maintainRBT_del(std::vector<TreeNode*>&);
    int prefixSumSubtree(TreeNode*, int);

    public :
        bool insert(int);
        bool remove(int);
        int rank(int);
        int select(int);
        int rangeSum(int, int);

        RBST() {}
        ~RBST();
        std::string print();
        int size() {return (root != nullptr)? root->size : 0;}
};



typedef std::vector<TreeNode*>::iterator vi;

inline bool isNull(TreeNode* t) {
    return (t==nullptr || (t->doubleblack && t->red));
}


RBST::~RBST() {
    // Destructor
    // Traverse the tree to free all memory allocated with `new`
    if (root != nullptr) {
        std::stack<TreeNode*> iot;  
        TreeNode *curr = root, *temp;
        // In-order traversal so that node's children dont need to be
        // accessed after it is deleted (a node is popped only once)
        while (!iot.empty()) {
            iot.push(curr);
            curr = curr->lc;
            if (curr==nullptr) {
                curr = iot.top(); iot.pop();
                temp = curr->rc; delete curr; curr = temp;
            }
        }
    }
}


void RBST::leftRotate(TreeNode* node, TreeNode* parent) {
    /* 
            node                            rc
           /    \            ==>          /    \
          lc     rc                   node     ...
                /   \               /      \
           rc->lc   ...            lc    rc->lc
     */
    assert(node != nullptr); 
    assert(node->rc != nullptr);       // For debugging
    if (parent == nullptr) assert(node==root);
    else assert(parent->rc == node || parent->lc == node);
    // Adjust the augmented info (sum, size) of nodes being rotated, O(1)
    int tempsum1 = node->rc->val;
    int tempsum2 = 0, tempsize2 = 0;
    if (! isNull(node->rc->rc)) {
        tempsum2 = node->rc->rc->sum; tempsize2 =node->rc->rc->size;
    }
    node->rc->sum += node->val ;
    node->rc->size += 1 ;
    if (! isNull(node->lc)) {
        node->rc->sum += node->lc->sum; 
        node->rc->size += node->lc->size;
    }
    node->sum -= tempsum1 + tempsum2;
    node->size -= 1       + tempsize2;
    // Now perform the rotation, also O(1) time
    if (parent != nullptr) {
        if (parent->lc == node) parent->lc = node->rc;
        else parent->rc = node->rc;
    } else
        root = node->rc;
    TreeNode* temp = node->rc->lc;
    node->rc->lc = node;
    node->rc = temp;
}

void RBST::rightRotate(TreeNode* node, TreeNode* parent) {
    /* 
            node                            lc
           /    \            ==>           /   \
         lc     rc                       ...    node
        /  \                                  /      \
      ...   lc->rc                        lc->rc      rc
     */
    assert(node != nullptr);
    assert(node->lc != nullptr);       // For debugging
    if (parent == nullptr) assert(node==root);
    else assert(parent->rc == node || parent->lc == node);
    // Adjust the augmented info (sum, size) of nodes being rotated, O(1)
    int tempsum1 = node->lc->val;
    int tempsum2 = 0, tempsize2 = 0;
    if (! isNull(node->lc->lc)) {
        tempsum2 = node->lc->lc->sum; tempsize2 = node->lc->lc->size;
    }
    node->lc->sum += node->val ;
    node->lc->size += 1 ;
    if (! isNull(node->rc)) {
        // If node->rc is not null (take care of intermediate deletion stages)
        node->lc->sum += node->rc->sum;
        node->lc->size += node->rc->size;
    }
    node->sum -= tempsum1 + tempsum2;
    node->size -= 1       + tempsize2;
    // Now perform the rotation, also O(1) time
    if (parent != nullptr) {
        if (parent->lc == node) parent->lc = node->lc;
        else parent->rc = node->lc;
    } else
        root = node->lc;
    TreeNode* temp = node->lc->rc;
    node->lc->rc = node;
    node->lc = temp;
}


void RBST::printSubtree(std::ostringstream& out, 
        const std::string& pref, const TreeNode* node, bool l) {

    out << pref << (l ? "\u251c\u2500\u2500" : "\u2514\u2500\u2500" );
    if( node != nullptr ) {
        out << ((node->red)? "\u001b[91m[" : "[") << node->val  << 
            "] " << node->size << ", " << node->sum  << 
            ((node->red)? "\u001b[0m\n" : "\n");

        printSubtree(out, pref+(l? "\u2502   ":"    "), node->lc, true);
        printSubtree(out, pref+(l? "\u2502   ":"    "), node->rc, false);
    } else {
        out << "\u257a\n";
    }
}

std::string RBST::print() {
    // For debugging purposes 
    std::ostringstream output;
    printSubtree(output, "", root, false);
    return output.str();
}


bool RBST::insert(int x) {
    if (root==nullptr) {
        root = new TreeNode(x, false);
        return true;
    }
    TreeNode* t = root;
    // Temporary O(height) auxiliary space during insertion
    // saves us from having to use O(n) space by permanently storing
    // the parent in each node, while keeping O(lg n) time
    std::vector<TreeNode*> ancestry;
    while (t != nullptr) {
        if (t->val == x)
            return false;
        ancestry.push_back(t);
        if (x < t->val)
            t = t->lc;
        else
            t = t->rc;
    }

    t = ancestry.back();
    TreeNode* n = new TreeNode(x, true);
    if (x < t->val) t->lc = n; 
    else            t->rc = n;

    // Adjust augmented sum/size info in nodes above it
    for (vi it=ancestry.begin(); it<ancestry.end(); ++it) {
        (*it)->size ++;
        (*it)->sum += x;
    }
    ancestry.push_back(n);
    
    // Check that the tree remains a valid RBT, and finish
    maintainRBT_ins(ancestry);
    root->red = false;
    return true;
}


void RBST::maintainRBT_ins(std::vector<TreeNode*>& ancestry) {
    // Note : ancestry contains all nodes from root till newly 
    // inserted leaf along its branch in sequence
    if (ancestry.size() < 2)
        return; // New node does not have grandparent, no possible conflicts
    
    TreeNode *c = ancestry.back(), *u,
        *p = ancestry.end()[-2], *g = ancestry.end()[-3];
    if (! (c->red && p->red))
        return; // Child & parent aren't both red, no possible conflicts
    
    bool side; // 1 if c is in right subtree, 0 if left
    if (p == g->rc) {
        u = g->lc; side = true;
    } else {
        u = g->rc; side = false;
    }

    if (u != nullptr && u->red) {
        // child, parent, uncle all red
        g->red = true; u->red = false; p->red = false;
        ancestry.pop_back(); ancestry.pop_back();
        maintainRBT_ins(ancestry);
        // Continue checking upwards from grandparent
    
    } else { // child & parent red, uncle black

        if (side && p->lc == c) {// Convert triangle case to line case
            rightRotate(p, g);
            std::swap(p, c);
        } else if (!side && p->rc == c) {
            leftRotate(p, g);
            std::swap(p, c);
        }
                                // Line case
        if (side) 
            leftRotate(g, (g==root) ? nullptr : ancestry.end()[-4]);
        else
            rightRotate(g, (g==root) ? nullptr : ancestry.end()[-4]);
        p->red = false;
        g->red = true;

        // Continue checking upwards from parent (now at location of grandp)
        ancestry.pop_back(); ancestry.pop_back(); ancestry.pop_back();
        ancestry.push_back(p);
        maintainRBT_ins(ancestry);
    }
}


bool RBST::remove(int x) {
    TreeNode* t = root;
    std::vector<TreeNode*> ancestry;
    // Temporary O(height) auxiliary space, used similarly as in insertion
    while (t != nullptr) {
        ancestry.push_back(t);  // Search for the node
        if (x == t->val)
            break;
        else if (x < t->val)
            t = t->lc;
        else
            t = t->rc;
    }
    if (t==nullptr)
        return false; // Node is not present


    // In case of 2 non-null children, use in-order successor
    // (will have 1 non-null child at most)
    if (t->lc != nullptr && t->rc != nullptr) {
        TreeNode* ios = t->rc;
        ancestry.push_back(ios); 
        while (ios->lc != nullptr) {
            ios = ios->lc;
            ancestry.push_back(ios);
        }
        int diff = ios->val - t->val;
        t->val = ios->val; t->sum += diff; // Replace value; 
        // Now adjust augmented info, propagate upwards
        for (vi it=ancestry.begin(); *it != t; ++it)
            (*it)->sum += diff;
        t = ios; //Change node to delete to the successor
    }


    bool simple = false;
    TreeNode* g = ancestry.end()[-2];
    // Simple cases (non-recursive)
    if (t==root && t->lc == nullptr && t->rc == nullptr) {
        root = nullptr;    // Delete root when it is the only node
        simple = true;
    } else if (t->red) {  // Node to delete is red
        TreeNode* temp = (t->rc != nullptr)? t->rc : t->lc;
        if (g->rc == t) g->rc = temp;
        else g->lc = temp;
        simple = true;
    } else if (t->lc != nullptr && t->rc == nullptr) {
        // Black node, with 1 left child only (will be red)
        assert(t->lc->red); simple = true; 
        t->lc->red = false; // Change red to black
        if (t != root) {
            if (g->rc == t) g->rc = t->lc;
            else  g->lc = t->lc;
        } else
            root = t->lc;
    } else if (t->lc == nullptr && t->rc != nullptr) {
        // Black node, with 1 right child only (will be red)
        assert(t->rc->red); simple = true;
        t->rc->red = false; // Change red to black
        if (t != root) {
            if (g->rc == t) g->rc = t->rc;
            else  g->lc = t->rc;
        } else
            root = t->rc;
    }


    // Propagate up, changing augmented sum & size values till root
    for (vi it=ancestry.begin(); *it != t; ++it) {
        (*it)->size --;
        (*it)->sum -= t->val;
    }

    if (simple) {
        delete t;
    } else {
        // The (only) complex case - black leaf node
        assert(t->lc==nullptr && t->rc==nullptr);
        t->doubleblack = true;
        t->red = true;
        /* NOTE : Since we are using `nullptr` to denote NULL leaves, 
                It isn't possible to store additonal info like "double black"
                on those. => Do not delete immediately, but mark it
                and use the now redundant colour bit (TreeNode.red) to
                store whether it should be null (1) or not (0) */
        maintainRBT_del(ancestry);
    }
    return true;
}


void RBST::maintainRBT_del(std::vector<TreeNode*>& ancestry) {
    // The last node is always double black, when called
    TreeNode *u = ancestry.back();
    assert(u->doubleblack);
    if (u==root) {  // Case 3
        u->doubleblack = false;
        return;
    }
    assert(ancestry.size() > 1);
    TreeNode *g = ancestry.end()[-2], *p, *c, *d;
    bool side = (g->lc == u);
    if (side) {
        p = g->rc; assert(p != nullptr); // will have a sibling
        c = p->lc; d = p->rc;
    } else {
        p = g->lc; assert(p != nullptr); // will have a sibling
        c = p->rc; d = p->lc;
    }

    if (p->red) { // Case 1, sibling red
        TreeNode* a = (ancestry.size() > 2)? ancestry.end()[-3] : nullptr;
        if (side)
            leftRotate(g, a);
        else 
            rightRotate(g, a);
        p->red = false;
        g->red = true;
        ancestry.pop_back(); ancestry.pop_back();
        ancestry.push_back(p); ancestry.push_back(g); ancestry.push_back(u);
        maintainRBT_del(ancestry);
    }
    else if (!p->red && (c==nullptr || !c->red) && (d==nullptr || !d->red)) {
        // sibling & both its children black
        if (g->red) {   // Case 2.1.2
            p->red = true; g->red = false;
        } else {        // Case 2.1.1
            p->red = true;
            g->doubleblack = true;
            ancestry.pop_back();
            maintainRBT_del(ancestry);
        }
        if (! u->red)
            u->doubleblack = false;
        else {
            if (side) g->lc = nullptr;
            else g->rc = nullptr;
            delete u;
        }
    } 
    else if (!p->red && (d==nullptr || !d->red) && (c!=nullptr && c->red)) {
        c->red = false; p->red = true;      // Case 2.2
        if (side)
            rightRotate(p, g);
        else 
            leftRotate(p, g);
        maintainRBT_del(ancestry);
    } 
    else if (!p->red && (d!=nullptr && d->red)) {   // Case 2.3
        p->red = g->red;
        g->red = false; d->red = false;
        TreeNode* a = (ancestry.size() > 2)? ancestry.end()[-3] : nullptr;
        if (side)
            leftRotate(g, a);
        else 
            rightRotate(g, a);
        if (! u->red)
            u->doubleblack = false;
        else {
            if (side) g->lc = nullptr;
            else g->rc = nullptr;
            delete u;
        }
    }
}


int RBST::rank(int x) {
    // Return 0 if not found, else a rank from 1..(tree.size)
    int r = 0;
    TreeNode* n = root;
    while (n != nullptr) {
        if (x < n->val) {
            n = n->lc;
        } else {
            r += (n->lc != nullptr) ? n->lc->size + 1 : 1;
            if (x == n->val)
                break;
            else
                n = n->rc;
        }
    }
    if (n==nullptr)
        return 0;
    else
        return r;
}


int RBST::select(int r) {
    // Given rank must be in range
    assert(r >= 1);
    assert(root != nullptr);
    assert(r <= root->size);
    int cr = (root->lc != nullptr)? root->lc->size + 1 : 1;
    TreeNode* n = root; // Start at top
    while (cr != r) {
        if (cr < r) {   // Move right
            n = n->rc; 
            cr += 1 + ((n->lc != nullptr)? n->lc->size : 0);
        } else if (cr > r) { // Move left
            n = n->lc; 
            cr -= 1 + ((n->rc != nullptr)? n->rc->size : 0);
        }
    }
    return n->val;
}


int RBST::prefixSumSubtree(TreeNode* n, int j) {
    assert(j >= 1 && j <= n->size);
    if (n==nullptr)
        return 0;
    if (n->lc == nullptr && n->rc == nullptr)
        return n->val;
    int ls = (n->lc != nullptr) ? n->lc->size : 0;
    // In each case, subtree height reduces 1 step
    if (ls > j-1)   // Move left
        return prefixSumSubtree(n->lc, j);
    else if (ls + 1 == j)
        return n->val + n->lc->sum;
    else            // Move right
        return n->val + n->lc->sum + 
            prefixSumSubtree(n->rc, j - 1 - n->lc->size);
}

int RBST::rangeSum(int i, int j) {
    assert(1 <= i && i <= root->size);
    assert(1 <= j && j <= root->size);
    // Prefixsum takes O(lg n) time, this is the same
    if (i==1) {
        return prefixSumSubtree(root, j);
    } else {
        return prefixSumSubtree(root, j) - prefixSumSubtree(root, i-1);
    }
}


int main() {
    RBST tree;
    std::cout << "Before\n" << tree.print();
    tree.insert(10);
    std::cout << "Inserted 10\n" << tree.print();
    tree.insert(5);
    std::cout << "Inserted 5\n" << tree.print();
    tree.insert(5);
    std::cout << "Inserted 5\n" << tree.print();
    tree.insert(4);
    std::cout << "Inserted 4\n" << tree.print();
    tree.insert(2);
    std::cout << "Inserted 2\n" << tree.print();
    tree.insert(3);
    std::cout << "Inserted 3\n" << tree.print();
    tree.insert(15);
    std::cout << "Inserted 15\n" << tree.print();
    tree.insert(12);
    std::cout << "Inserted 12\n" << tree.print();
    tree.insert(13);
    std::cout << "Inserted 13\n" << tree.print();
    tree.remove(10);
    std::cout << "\nDeleted 10\n" << tree.print();
    tree.remove(5);
    std::cout << "Deleted 27\n" << tree.print();
    tree.remove(5);
    std::cout << "Deleted 5\n" << tree.print();
    tree.remove(15);
    std::cout << "Deleted 15\n" << tree.print();
    tree.remove(3);
    std::cout << "Deleted 3\n" << tree.print();
    tree.remove(13);
    std::cout << "Deleted 13\n" << tree.print();
    tree.remove(2);
    std::cout << "Deleted 2\n" << tree.print();

    int opt=1, x, y;
    do {
        std::cout << "Operations :\n[0] Exit\t[1] Print\t[2] Insert\t"  << 
            "[3] Delete\t[4] Rank\t[5] Select\t[6] RangeSum\nSelect choice - ";
        std::cin >> opt;
        switch (opt) {
            case 1:
                std::cout << tree.print(); break;
            case 2:
                std::cout << "Element to insert> "; std::cin >> x; 
                std::cout << ((tree.insert(x))?"Ok":"Duplicate") << "\n" << tree.print();
                break;
            case 3:
                std::cout << "Element to remove> "; std::cin >> x; 
                std::cout << (tree.remove(x)?"Ok":"Not found") << "\n" << tree.print();
                break;
            case 4:
                std::cout << "Element to search> "; std::cin >> x;
                std::cout << tree.rank(x) << "\n";
                break;
            case 5:
                std::cout << "Rank to get> "; std::cin >> x;
                if (x >= 1 && x <= tree.size())
                    std::cout << tree.select(x) << "\n";
                else 
                    std::cout << "Invalid\n";
                break;
            case 6:
                std::cout << "Lower & upper bounds> "; std::cin >> x >> y;
                if (x >= 1 && x <= tree.size() && y >= 1 && y <= tree.size())
                    std::cout << tree.rangeSum(x, y) << "\n";
                else 
                    std::cout << "Invalid\n";
                break;
        }
    } while (opt != 0);
    return 0;
    /* 
    NOTE : The print format is   [value] size, sum
                                    ├── Left child
                                    └── Right child
         Null nodes represented with a dot

         Red nodes are coloured red using terminal ANSI escape codes
         if your command prompt doesnt render this properly,
         you may see some letters like ^[[e91m etc..
     */
    /* NOTE :   Select function returns 0 if not found, 1..n otherwise
     */
}

