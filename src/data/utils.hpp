#ifndef UTILS_HPP
#define UTILS_HPP

#include "matrix.hpp"

namespace s21 {

template <typename T>
struct Node
{
    Node()
        : data_(T())
        , next_(nullptr)
    {
    }

    Node(T val, Node<T>* next = nullptr)
        : data_(val)
        , next_(next)
    {
    }

    ~Node() = default;

    T data_;
    Node<T>* next_;

}; // Node

class Utils {
public:
    static bool StrFormat(std::string str, std::string format) {
        if(str.length() < format.length()) return false;
        int8_t z = str.length() - 1;
        for(int8_t i = format.length() - 1; i >= 0; --i) {
            if(str[z--] != format[i]) return false;
        }
        return true;
    }

}; // Utils


}; // namespace s21

#endif // UTILS_HPP