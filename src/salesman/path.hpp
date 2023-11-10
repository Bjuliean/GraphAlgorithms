#ifndef PATH_HPP
#define PATH_HPP

#include "../data/matrix.hpp"
#include "../graph/s21_graph.h"

namespace s21 {

class Path {
public:
    Path(s21::Graph graph)
        : graph_(graph.Matrix())
        , pheromones_(graph.MatrixSize(), graph.MatrixSize())
        , VAPORATION_COEF(0.75)
    {
        for(size_t i = 0; i < pheromones_.Rows(); ++i) {
            for(size_t z = 0; z < pheromones_.Cols(); ++z) {
                if(i != z) pheromones_(i, z) = 0.2f;
                if(i == z) pheromones_(i, z) = 0.0f;
            }
        }
    }

    ~Path() = default;

    s21::Matrix<int>& Distance() { return graph_; }

    s21::Matrix<double>& Pheromones() { return pheromones_; }

    void UpdatePheromones(std::vector<size_t>& path) {
        SpreadPheromones(path);
        PheromoneEvaporation();
    }

    size_t CalculatePathLength(std::vector<size_t>& path) {
        size_t res = 0;
        for(size_t i = 0; i < path.size() - 1; ++i) {
            res += graph_(path[i], path[i + 1]);
        }
        return res;
    }

private:
    s21::Matrix<int> graph_;
    s21::Matrix<double> pheromones_;
    const double VAPORATION_COEF;

    void SpreadPheromones(std::vector<size_t>& path) {
        size_t path_length = CalculatePathLength(path);
        double coef = static_cast<double>(graph_.Rows()) / static_cast<double>(path_length);
        for(size_t i = 0; i < path.size() - 1; ++i) {
            pheromones_(path[i], path[i + 1]) += coef;
        }
    }

    void PheromoneEvaporation() {
        for(size_t i = 0; i < pheromones_.Size(); ++i) pheromones_[i] *= VAPORATION_COEF;
    }


}; // Path

}; // namespace s21

#endif // PATH_HPP