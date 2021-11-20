
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <iostream>



struct ListNode {

    protected :
        ListNode* next = nullptr;
    friend class LinkedList;

    public :
        int val;
        ListNode* nxt() const {return next;}

        ListNode() = default;
        ListNode(int x) : val(x) {}
        ~ListNode() = default;

        ListNode& operator=(int i) {
            val = i; return *this;
        }
        ListNode& operator=(ListNode&) = default;
        ListNode& operator=(ListNode&&) = default;
        
        operator int() const {return val;}
};
/* 
Assigning an int to the node with operator= sets its value
Copy & move using operator= use compiler-generated functions

The node can be implicitly converted to its `int` value

This way, using operator[] on the linked list returns the node,
which you can read/write as int (its value),
but also continue traversing the list using the same returned reference
if necessary, by calling .nxt() on it.

Afterthought : This seems complicated and unnecessary now
operator[] on the linked list should just directly return the node's value
and some kind of iterator for traversal separately.
 */




class LinkedList {

    protected :
        ListNode* start = nullptr;
        ListNode* end = nullptr;
        int l = 0;

    public :
        LinkedList() = default;
        LinkedList(const std::initializer_list<int>&);
        ~LinkedList() {clear(); std::cerr<<"Bye\n";}

        int size() {return l;}
        ListNode* startnode() {return start;}
        ListNode& operator[](int);
        ListNode& insert(int, int);
        ListNode& append(int);
        int remove(int);
        void clear();
        void print(const std::string&, const std::string&, std::ostream&);
};

LinkedList::LinkedList(const std::initializer_list<int>& il) {
    for (const int& x : il) {
        ListNode* nn = new ListNode(x);
        if (start == nullptr)
            start = nn;
        if (end != nullptr)
            end->next = nn;
        end = nn;
        ++l;
    }
}

ListNode& LinkedList::operator[](int i) {
    if (i<0 || i>=l) {
        throw std::out_of_range("Invalid index " + std::to_string(i) + \
        ". Extent is 0.." + std::to_string((l>0)?l-1:0));
    }
    int a=0;
    ListNode* n = start;
    while (a++ < i)
        n = n->next;
    return *n;
}

ListNode& LinkedList::insert(int i, int val) {
    if (i<0 || i>l) {
        throw std::out_of_range("Invalid insertion index " + std::to_string(i) + \
        ". Extent is 0.." + std::to_string(l));
    }
    ListNode* nn = new ListNode(val);
    if (i==0) {
        nn->next = start;
        start = nn;
        if (l==0)
            end = nn;
    } else if (i==l) {
        nn->next = nullptr;
        end->next = nn;
        end = nn;
    } else {
        int a=0;
        ListNode* n=start;
        while (a++ < i-1)
            n = n->next;
        nn->next = n->next;
        n->next = nn;
    }
    ++l;
    return *nn;
}

ListNode& LinkedList::append(int val) {
    ListNode* nn = new ListNode(val);
    nn->next = nullptr;
    if (l==0)
        start = nn;
    else
        end->next = nn;
    end = nn;
    ++l;
    return *nn;
}

int LinkedList::remove(int i) {
    if (i<0 || i>=l) {
        throw std::out_of_range("Invalid deletion index " + std::to_string(i) + \
        ". Extent is 0.." + std::to_string((l>0)?l-1:0));
    }
    int val;
    ListNode* temp;
    if (i==0) {
        val = start->val;
        temp = start->next;
        delete start;
        start = temp;
        if (l==1)
            end = nullptr;
    } else if (i==l-1) {
        temp = start;
        while (temp->next != end)
            temp = temp->next;
        temp->next = nullptr;
        val = end->val;
        delete end;
        end = temp;
    } else {
        int a=0;
        ListNode* n=start;
        while (a++ < i-1)
            n = n->next;
        temp = n->next;
        val = temp->val;
        n->next = temp->next;
        delete temp;
    }
    --l;
    return val;
}

void LinkedList::clear() {
    ListNode *c=start, *nx;
    while (c != nullptr) {
        nx = c->next;
        delete c;
        c = nx;
        --l;
    }
    start = nullptr;
    end = nullptr;
}

void LinkedList::print(const std::string& sep=" ", 
                       const std::string& fin="|\n", 
                       std::ostream& stream = std::cout) {
    ListNode* n = start;
    while (n != nullptr) {
        stream << n->val;
        if (n != end)
            stream << sep;
        n = n->next;
    }
    stream << fin;
}



/* Stack and Queue, based on the Linkedlist */

class Stack : private LinkedList {
    
    public :
        using LinkedList::LinkedList;
        using LinkedList::size;
        using LinkedList::print;
        using LinkedList::clear;

        ListNode& push(int val) {
            return insert(0, val);
        }
        int pop() {
            if (l>0)
                return remove(0);
            else
                throw std::out_of_range("Stack is empty");
        }
        ListNode& top() {
            if (start != nullptr)
                return *start;
            else
                throw std::out_of_range("Stack is empty");
        }
};



class Queue : private LinkedList {
    
    public :
        using LinkedList::LinkedList;
        using LinkedList::size;
        using LinkedList::print;
        using LinkedList::clear;

        ListNode& enqueue(int val) {
            return append(val);
        }
        int dequeue() {
            if (l>0)
                return remove(0);
            else
                throw std::out_of_range("Queue is empty");
        }
        ListNode& front() {
            if (start != nullptr)
                return *start;
            else
                throw std::out_of_range("Queue is empty");
        }
        ListNode& back() {
            if (end != nullptr) 
                return *end;
            else
                throw std::out_of_range("Queue is empty");
        }
};
