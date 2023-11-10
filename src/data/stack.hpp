#ifndef STACK_HPP
#define STACK_HPP

#include <memory>

#include "utils.hpp"

namespace s21 {

template <typename T, typename Alloc = std::allocator<Node<T>>> 
class Stack {
public:

    Stack()
        : head_(nullptr)
        , size_(0)
    {
    }

    ~Stack()
    {
        DeallocateStack();
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
        ++size_;
        nw->next_ = head_;
        head_ = nw;
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

    inline T& Top() { return head_->data_; }

    inline size_t Size() const noexcept { return size_; }

    inline bool Empty() const noexcept { return size_ == 0; }

    void Clear() {
        DeallocateStack();
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

    void DeallocateStack() {
        while(Size() > 0) Pop();
    }

}; // stack

}; // namespace s21

#endif // STACK_HPP