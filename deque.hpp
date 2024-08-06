#ifndef deque_hpp
#define deque_hpp
/* 
    this is not the same as std::deque. 
    it is an "output restricted double ended queue" as 
    described in chapter 20 of Sedgewick's "Algorithms" (2nd edition)
    using push/pop gives a lifo stack, 
    using put/pop yields a fifo queue.
*/
template <class T>
class deque {
    private:
        struct node {
            T info;
            node* next;
            node(T i) : info(i), next(nullptr) { }
        };
        using link = node*;
        link head, tail;
        int count;
    public:
        deque() {
            head = nullptr;
            tail = nullptr;
            count = 0;
        }
        ~deque() {
            while (head != nullptr) {
                link x = head;
                head = head->next;
                delete x;
            }
        }
        int size() const {
            return count;
        }
        bool empty() const {
            return head == nullptr;
        }
        void put(T info) {
            link x = new node(info);
            if (empty()) head = x;
            else tail->next = x;
            tail = x;
            count++;
        }
        void push(T info) {
            link x = new node(info);
            x->next = head;
            head = x;
            if (tail == nullptr) tail = x;
            count++;
        }
        T pop() {
            T ret = head->info;
            link x = head;
            head = head->next;
            delete x;
            count--;
            return ret;
        }
};

#endif