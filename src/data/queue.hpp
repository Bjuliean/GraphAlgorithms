#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <memory>

#include "utils.hpp"

namespace s21 {

template <typename T, typename Alloc = std::allocator<Node<T>>>
class Queue {
public:
    Queue()
        : head_(nullptr)
        , size_(0)
    {
    }

    ~Queue()
    {
        DeallocateQueue();
    }

    void Push(T val) {
        Node<T>* nw = std::allocator_traits<Alloc>::allocate(alloc_, 1);
        try {
            std::allocator_traits<Alloc>::construct(alloc_, nw, val);
        } catch(...) {
            std::allocator_traits<Alloc>::deallocate(alloc_, nw, 1);
            throw;
        }
        nw->data_ = val;
        nw->next_ = nullptr;
        Size() == 0 ? head_ = nw : LastNode()->next_ = nw;
        ++size_;
    }

    T Pop() {
        if(Empty()) throw std::out_of_range("Stack is empty.\n");
        Node<T>* temp = head_->next_;
        T res = head_->data_;
        DeallocateNode(head_);
        head_ = temp;
        --size_;
        return res; 
    }

    inline T& Front() { return head_->data_; }

    T& Back() {
        Node<T>* temp = head_;
        while(temp->next_ != nullptr) temp = temp->next_;
        return temp->data_;
    }

    inline size_t Size() const noexcept { return size_; }

    inline bool Empty() const noexcept { return size_ == 0; }

    void Clear() {
        DeallocateQueue();
        head_ = nullptr;
    }

private:
    Node<T>* head_;
    size_t size_;
    Alloc alloc_;

    void DeallocateNode(Node<T>* node) {
        std::allocator_traits<Alloc>::destroy(alloc_, node);
        std::allocator_traits<Alloc>::deallocate(alloc_, node, 1);
    }

    void DeallocateQueue() {
        while(Size() > 0) Pop();
    }

    Node<T>* LastNode() {
        Node<T>* temp = head_;
        while(temp->next_ != nullptr) temp = temp->next_;
        return temp;
    }
    
}; // queue

}; // namespace s21

#endif // QUEUE_HPP