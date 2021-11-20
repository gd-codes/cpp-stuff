#include <iostream>
#include <exception>


#include "sqll.hpp"


using namespace std;

void test_list() {
    cout  << "ok\n";
    LinkedList l {1,3,5,7};
    cout << &l[0] << " " << &l[1] << " " << &l[2] << " " << &l[3];
    try {
        cout << &l[4];
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    l.print(", ");
    l.append(-2);
    l.print();
    l.append(10);
    l.print();
    l.clear();
    l.print();
    l.append(11);
    l.print();
    try {
        l.insert(10,10);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    l.insert(0,3);
    l.print();
    l.insert(l.size(),12);
    l.print();
    l.insert(1,101);
    l.print();
    l.append(4);
    l.print();
    l[3] = 22;
    l.print();
    cout << "l2 = " << l[2] << "\n";
    cout << l.remove(0) << "\n";
    l.print();
    cout << l.remove(l.size()-1) << "\n";
    l.print();
    cout << l.remove(1) << "\n";
    l.print();
    try {
        l.remove(100);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

int main() {
    cout << "Linkedlist tests\n";
    test_list();

    cout << endl << "Stack tests\n";
    Stack s;
    s.push(22);
    s.push(3);
    s.print();
    try {
        cout << s.top() << "\n";
        cout << s.top().nxt()  << "\n";
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}