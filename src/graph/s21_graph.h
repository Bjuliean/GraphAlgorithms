#ifndef S21_GRAPH_H
#define S21_GRAPH_H

#include "../data/utils.hpp"

namespace s21 {

class Graph {
public:
    Graph()
        : adjacency_matrix_(0, 0)
    {
    }

    Graph(const Graph& other);

    ~Graph() = default;

    bool LoadGraphFromFile(std::string filename);

    void ExportGraphToDot(std::string filename);

    inline size_t TotalElements() const noexcept { return adjacency_matrix_.Size(); }
    
    inline size_t MatrixSize() const noexcept { return adjacency_matrix_.Rows(); }

    inline s21::Matrix<int>& Matrix() { return adjacency_matrix_; }

    inline int& operator()(int i, int z) { return adjacency_matrix_(i, z); }

    void Debug() { adjacency_matrix_.Debug(); }

private:
    s21::Matrix<int> adjacency_matrix_;

    void MatrixSettings(std::string str);

    void ParseValues(std::string str);

}; // Graph

}; // namespace s21

#endif // S21_GRAPH_H