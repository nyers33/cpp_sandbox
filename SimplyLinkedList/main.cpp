#include <iostream>
#include <vector>
#include <set>
#include <functional>

// singly-linked list
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class LinkedList {
public:
    ListNode* head;
    LinkedList() : head(nullptr) {}

    // copy constructor
    // to initialize a previously uninitialized object from some other object's data
    LinkedList(const LinkedList& other) : head(nullptr) {
        if (other.head) {
            head = new ListNode(other.head->val);
            ListNode* temp = head;
            ListNode* otherTemp = other.head->next;
            while (otherTemp) {
                temp->next = new ListNode(otherTemp->val);
                temp = temp->next;
                otherTemp = otherTemp->next;
            }
        }
    }

    int size()
    {
        int sizeOfLL = 0;
        ListNode* temp = head;
        while (temp)
        {
            ++sizeOfLL;
            temp = temp->next;
        }

        return sizeOfLL;
    }

    // assignment operator
    // to replace the data of a previously initialized object with some other object's data
    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) return *this; // handle self-assignment

        // clear existing list
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }

        // copy from other
        if (other.head) {
            head = new ListNode(other.head->val);
            ListNode* temp = head;
            ListNode* otherTemp = other.head->next;
            while (otherTemp) {
                temp->next = new ListNode(otherTemp->val);
                temp = temp->next;
                otherTemp = otherTemp->next;
            }
        }
        return *this;
    }

    // insert at the end
    void insert(int val) {
        if (!head) {
            head = new ListNode(val);
            return;
        }
        ListNode* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = new ListNode(val);
    }

    // delete all nodes with a specific value
    void deleteValue(int val)
    {
        while (head && head->val == val) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
        ListNode* current = head;
        while (current && current->next) {
            if (current->next->val == val) {
                ListNode* temp = current->next;
                current->next = current->next->next;
                delete temp;
            }
            else {
                current = current->next;
            }
        }
    }

    // delete all nodes with a specific values
    void deleteValues(std::vector<int>& values)
    {
        if (!head)
            return;

        std::set<int> deleteSet(values.begin(), values.end());
        if (deleteSet.empty())
            return;

        // dummy node to handle head deletions
        ListNode* dummy = new ListNode(0, head);
        ListNode* current = dummy;

        while (current->next) {
            // whether value should be deleted
            if (deleteSet.find(current->next->val) != deleteSet.end())
            {
                ListNode* temp = current->next;
                current->next = current->next->next; // skip the node
                delete temp; // free memory
            }
            else {
                current = current->next; // progress with traversal
            }
        }

        head = dummy->next; // update head in case the first node was deleted
        delete dummy; // clean up dummy node
    }

    // delete the middle node
    void deleteMiddleNode()
    {
        if (!head)
            return;

        // find size of list and calc mid position
        int middleOffset = 0;
        ListNode* temp = head;
        while (temp)
        {
            ++middleOffset;
            temp = temp->next;
        }
        middleOffset = middleOffset  / 2;

        // find mid list element
        ListNode* midElement = head;
        while (--middleOffset > 0)
        {
            midElement = midElement->next;
        }

        // eliminate mid list element
        if (midElement->next)
        {
            temp = midElement->next;
            midElement->next = midElement->next->next;
            delete temp;
            temp = nullptr;
        }
        else
        {
            // only one element
            delete head;
            head = nullptr;
        }
    }

    void reverseIterative()
    {
        ListNode* prev = nullptr;
        ListNode* current = head;
        ListNode* next = nullptr;

        while (current) {
            next = current->next; // store next node
            current->next = prev; // reverse the link
            prev = current;       // move prev forward
            current = next;       // move current forward
        }

        head = prev; // update head to the new first node
    }

    void reverseRecursive() {
        head = reverseRecursiveInternal(head);
    }

    ListNode* reverseRecursiveInternal(ListNode* node)
    {
        // base case - last node becomes new head
        if (!node || !node->next) return node; 

        // recursively reverse rest
        ListNode* newHead = reverseRecursiveInternal(node->next);
        // reverse the link
        node->next->next = node;
        // set current node’s next to null
        node->next = nullptr;    

        // return new head node
        return newHead; 
    }

    void removeNodesSmallerThanMaxSoFar()
    {
        // empty or single-node list
        if (!head || !head->next)
            return;

        // step 1 - reverse the linked list
        reverseIterative();

        // step 2 - remove nodes that are smaller than the max seen so far
        ListNode* maxNode = head; // first node is the max initially

        ListNode* current = head;
        while (current && current->next) {
            if (current->next->val < maxNode->val) {
                // delete the smaller node
                ListNode* temp = current->next;
                current->next = current->next->next;
                delete temp;
            }
            else {
                // update maxNode if we find a larger value
                current = current->next;
                maxNode = current;
            }
        }

        // step 3 - reverse back to original order
        reverseIterative();
    }

    // in-place merge another sorted linked-list
    void merge(LinkedList& other, std::function<bool(int, int)> comp = std::less<int>())
    {
        // target linked-list is empty
        if (!head) {
            head = other.head;
            other.head = nullptr;
            return;
        }

        // source linked-list is empty, nothing to do
        if (!other.head) return;

        // placeholder to simplify list merging / fixed starting point
        ListNode* dummy = new ListNode(0);

        // always points to the last merged node
        ListNode* tail = dummy;

        ListNode* l1 = head;
        ListNode* l2 = other.head;

        while (l1 && l2) {
            // attach node to tail->next
            // move forward l1 / l2
            if (comp(l1->val, l2->val)) {
                tail->next = l1;
                l1 = l1->next;
            }
            else {
                tail->next = l2;
                l2 = l2->next;
            }
            // move tail forward
            tail = tail->next;
        }

        // attaching remaining elements
        tail->next = (l1) ? l1 : l2;

        // assigning the new head
        head = dummy->next;
        
        // merging done
        delete dummy;
        other.head = nullptr;
        
    }

    // merge two sorted linked lists in an iterative fashion
    static ListNode* mergeIterative(ListNode* l1, ListNode* l2, std::function<bool(int, int)> comp)
    {
        // if either list is empty, return the other list.
        if (!l1) return l2;
        if (!l2) return l1;

        // ensure l1 is the smaller head
        if (l1->val > l2->val) std::swap(l1, l2);
        ListNode* head = l1;

        // traverse l1, inserting nodes from l2 in their correct positions
        while (l1->next && l2) {
            // move the l2 node into l1
            if (l2->val < l1->next->val)
            {
                ListNode* temp = l2;
                l2 = l2->next;
                temp->next = l1->next;
                l1->next = temp;
            }
            l1 = l1->next;
        }

        // if l1 reaches the end, append remaining l2
        if (!l1->next) l1->next = l2;
        
        return head;
    }

    // merge two sorted linked lists in a recursive fashion - GfG
    static ListNode* mergeRecursive(ListNode* l1, ListNode* l2, std::function<bool(int, int)> comp)
    {
        // if either list is empty, return the other list
        if (l1 == nullptr) return l2;
        if (l2 == nullptr) return l1;

        // pick the smaller value between l1 and l2 nodes
        if (comp(l1->val, l2->val)) {

            // recursively merge the rest of the lists
            // and link the result to the current node
            l1->next = mergeRecursive(l1->next, l2, comp);
            return l1;
        }
        else {
            // recursively merge the rest of the lists
            // and link the result to the current node
            l2->next = mergeRecursive(l1, l2->next, comp);
            return l2;
        }
    }
    
    // insertion sort
    void insertionSort(std::function<bool(int, int)> comp = std::less<int>())
    {
        ListNode* sorted = nullptr;
        while (head) {
            ListNode* node = head;
            head = head->next;
            if (!sorted || comp(node->val, sorted->val)) {
                node->next = sorted;
                sorted = node;
            }
            else {
                ListNode* temp = sorted;
                while (temp->next && !comp(node->val, temp->next->val))
                    temp = temp->next;
                node->next = temp->next;
                temp->next = node;
            }
        }
        head = sorted;
    }

    // bubble sort
    void bubbleSort(std::function<bool(int, int)> comp = std::less<int>())
    {
        if (!head || !head->next) return;

        bool swapped;
        do {
            swapped = false;
            ListNode* current = head;
            while (current->next) {
                if (comp(current->next->val, current->val))
                {
                    std::swap(current->val, current->next->val);
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
    }

    // merge sort
    void mergeSort(std::function<bool(int, int)> comp = std::less<int>())
    {
        head = mergeSortInternal(head, comp);
    }

    // merge sort helper
    ListNode* mergeSortInternal(ListNode* node, std::function<bool(int, int)> comp)
    {
        // if the list is empty or has only one node, it's already sorted
        if (!node || !node->next) return node;

        // split the list into two halves
        ListNode* slow = node;
        ListNode* fast = node->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        ListNode* mid = slow->next;
        slow->next = nullptr;

        // recursively sort each half
        ListNode* left = mergeSortInternal(node, comp);
        ListNode* right = mergeSortInternal(mid, comp);

        // merge the two sorted halves
        return mergeRecursive(left, right, comp);
    }

    // display the linked list
    void display() {
        ListNode* temp = head;
        while (temp) {
            std::cout << temp->val << " -> ";
            temp = temp->next;
        }
        std::cout << "NULL" << std::endl;
    }

    static ListNode* deepCopy(ListNode* head) {
        if (!head) return nullptr;
        ListNode* newHead = new ListNode(head->val);
        ListNode* current = newHead;
        head = head->next;

        while (head) {
            current->next = new ListNode(head->val);
            current = current->next;
            head = head->next;
        }
        return newHead;
    }

    // operator overloading to merge two lists safely
    LinkedList operator+(const LinkedList& other) {
        LinkedList result;
        ListNode* copy1 = deepCopy(this->head);
        ListNode* copy2 = deepCopy(other.head);
        result.head = mergeIterative(copy1, copy2, std::less<int>());
        return result;
    }

    // destructor to delete entire list
    ~LinkedList() {
        ListNode* current = head;
        while (current) {
            ListNode* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
    }
};

int main() {
    LinkedList list1;
    list1.insert(1);
    list1.insert(3);
    list1.insert(5);
    list1.insert(6);
    list1.insert(7);

    LinkedList list2;
    list2.insert(2);
    list2.insert(4);
    list2.insert(6);
    list2.insert(8);

    int list1Size = list1.size();
    int list2Size = list2.size();

    std::cout << "list 1: ";
    list1.display();
    std::cout << "list 2: ";
    list2.display();
    std::cout << std::endl;

    std::cout << "merge linked-lists into new list" << std::endl;
    {
        LinkedList mergedList = list1 + list2;
        std::cout << "merged List: ";
        mergedList.display();

        mergedList.deleteValue(2);
        mergedList.deleteValue(6);
        std::cout << "merged list after deleting values 2 & 6: ";
        mergedList.display();
        std::cout << "original lists are intact" << std::endl;
        std::cout << "list 1: ";
        list1.display();
        std::cout << "list 2: ";
        list2.display();
    }
    std::cout << std::endl;

    std::cout << "in-palce merge linked-list" << std::endl;
    {
        list1.merge(list2);
        std::cout << "list1 <--- list2: ";
        list1.display();

        std::cout << "list2: ";
        list2.display();
    }
    std::cout << std::endl;

    LinkedList listRandom;
    for (const auto i : { 4, 3, 5, 6, 1, 8, 2, 4, 7, 6 }) {
        listRandom.insert(i);
    }
    std::cout << "listRandom: ";
    listRandom.display();
    listRandom.reverseIterative();
    std::cout << "reverse: ";
    listRandom.display();
    listRandom.reverseRecursive();
    std::cout << "reverse: ";
    listRandom.display();
    std::cout << std::endl;

    {
        std::cout << "insertion sort - less" << std::endl;
        {
            LinkedList listSorted(listRandom);
            listSorted.insertionSort(std::less<int>());
            listSorted.display();
        }

        std::cout << "bubble sort - greater" << std::endl;
        {
            LinkedList listSorted(listRandom);
            listSorted.bubbleSort(std::greater<int>());
            listSorted.display();
        }

        std::cout << "merge sort - less" << std::endl;
        {
            LinkedList listSorted(listRandom);
            listSorted.mergeSort(std::less<int>());
            listSorted.display();
        }
    }
    std::cout << std::endl;

    std::cout << "delete middle element" << std::endl;
    {
        LinkedList deleteMidList;
        for (const auto i : { 1 }) {
            deleteMidList.insert(i);
        }
        deleteMidList.display();
        deleteMidList.deleteMiddleNode();
        deleteMidList.display();
    }
    std::cout << std::endl;

    std::cout << "delete values" << std::endl;
    {
        LinkedList deleteValuesList;
        for (const auto i : { 1,2,3,4,1,2,5,7,2,3,1 }) {
            deleteValuesList.insert(i);
        }
        
        std::vector<int> valuesToDelete{ 1,2 };
        deleteValuesList.display();
        deleteValuesList.deleteValues(valuesToDelete);
        deleteValuesList.display();
    }

    std::cout << "remove every node which has a node with a greater value anywhere to the right side of it" << std::endl;
    {
        LinkedList myList;
        for (const auto i : { 5,2,13,3,8 }) {
            myList.insert(i);
        }

        myList.display();
        myList.removeNodesSmallerThanMaxSoFar();
        myList.display();
    }

    return 0;
}
