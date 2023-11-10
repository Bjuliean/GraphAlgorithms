#ifndef TSMRESULT_HPP
#define TSMRESULT_HPP

#include <vector>
#include <cstddef>

namespace s21 {

struct TsmResult {
    std::vector<size_t> vertices;
    double distance;
};

};

#endif // TSMRESULT_HPP