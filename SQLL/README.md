## SQLL

A Stack, Queue and (singly) Linked List implementation in C++, to store integer values.
The `class LinkedList` has the following methods :
- Constructors - default and from `std::initializer_list<int>` which inserts the N elements in `O(N)` time
- Retrieving the `i`th Node using `operator[]` in `O(N)` time, by traversing the linked list till that point. 
- Inserting a new `int` into the linkedlist at the `i`th position in `O(N)` time for arbitrary `i`, or `O(1)` time for insertion at the start/end.
- Deleting an `int` at the `i`th position in `O(N)` time for arbitrary `i`, or `O(1)` time for deletion at the start only.
- Deleting all elements in `O(N)` time, also called by the Destructor.
- Printing the contents of the linkedlist to an `std::ostream`
- Ability to traverse the list using the `nxt` method of a Node
- Ability to read/write to the `int` value of a node from any reference to it.

The classes `Stack` & `Queue` inherit from `LinkedList`, with additional methods `push/pop/top` in the stack, or `enqueue/dequeue/front/back` in the queue instead of arbitrary insert/delete.

Methods throw `std::out_of_range` whenever necessary (if invalid index/position parameters are passed).

