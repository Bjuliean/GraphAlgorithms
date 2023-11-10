#ifndef S21_GRAPH_ALGORITHMS_H
#define S21_GRAPH_ALGORITHMS_H

#include <algorithm>

#include "../graph/s21_graph.h"
#include "../data/stack.hpp"
#include "../data/queue.hpp"
#include "../salesman/ant_colony.hpp"

namespace s21 {

class GraphAlgorithms {
public:

    GraphAlgorithms()
        : INF(-1)
    {
    }

    ~GraphAlgorithms() = default;

    std::vector<int> DepthFirstSearch(Graph &graph, int startVertex);

    std::vector<int> BreadthFirstSearch(Graph &graph, int startVertex);

    int GetShortestPathBetweenVertices(Graph &graph, int vertex1, int vertex2);

    s21::Matrix<int> GetShortestPathsBetweenAllVertices(Graph &graph);

    s21::Matrix<int> GetLeastSpanningTree(Graph &graph);

    TsmResult SolveTravelingSalesmanProblem(Graph &graph);

private:
    const int INF;

    inline bool CorrectVertex(Graph &graph, int vertex) const noexcept { return static_cast<size_t>(vertex) > 0 && static_cast<size_t>(vertex) <= graph.MatrixSize(); }

    bool IsAllVertexVisited(std::vector<bool>& vec) const noexcept {
        for(auto it : vec) if(!it) return false;
        return true;
    }

}; // GraphAlgorithms

}; // namespace s21

#endif // S21_GRAPH_ALGORITHMS_H