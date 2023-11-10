#include "s21_graph_algorithms.h"

namespace s21 {

std::vector<int> GraphAlgorithms::DepthFirstSearch(Graph &graph,
                                                   int startVertex) {
  std::vector<int> result;
  if (CorrectVertex(graph, startVertex)) {
    std::vector<bool> visited(graph.MatrixSize());
    s21::Stack<int> st;
    st.Push(startVertex - 1);
    visited[startVertex - 1] = true;
    while (!st.Empty()) {
      result.push_back(st.Top() + 1);
      int current = st.Pop();

      for (int i = graph.MatrixSize() - 1; i >= 0; i--) {
        if (graph(i, current) > 0 && !visited[i]) {
          visited[i] = true;
          st.Push(i);
        }
      }
    }
  }
  return result;
}

std::vector<int> GraphAlgorithms::BreadthFirstSearch(Graph &graph,
                                                     int startVertex) {
  std::vector<int> result;
  if (CorrectVertex(graph, startVertex)) {
    std::vector<bool> visited(graph.MatrixSize());
    s21::Queue<int> q;
    q.Push(startVertex - 1);
    visited[startVertex - 1] = true;
    while (!q.Empty()) {
      result.push_back(q.Front() + 1);
      int current = q.Pop();

      for (size_t i = 0; i < graph.MatrixSize(); ++i) {
        if (graph(i, current) > 0 && !visited[i]) {
          visited[i] = true;
          q.Push(i);
        }
      }
    }
  }
  return result;
}

int GraphAlgorithms::GetShortestPathBetweenVertices(Graph &graph, int vertex1,
                                                    int vertex2) {
  int result = 0;
  if (CorrectVertex(graph, vertex1) && CorrectVertex(graph, vertex2)) {
    --vertex1;
    --vertex2;
    size_t matrix_size_ = graph.MatrixSize();
    std::vector<bool> visited(matrix_size_, false);
    std::vector<int> distance(matrix_size_, INF);
    distance[vertex1] = 0;
    int current = vertex1;

    while (!IsAllVertexVisited(visited)) {
      for (size_t i = 0; i < matrix_size_; ++i) {
        if (!visited[i] && graph(current, i) > 0) {
          int sum = distance[current] + graph(current, i);
          if (distance[i] == INF || (distance[i] > sum)) {
            distance[i] = sum;
          }
        }
      }
      visited[current] = true;

      for (size_t i = 0; i < visited.size(); ++i) {
        if (!visited[i]) {
          current = i;
          break;
        }
      }

      for (size_t i = 0; i < distance.size(); ++i) {
        if (!visited[i] &&
            ((distance[i] < distance[current] && distance[i] != INF) ||
             distance[current] == INF)) {
          current = i;
        }
      }
      if (distance[current] == INF) distance[current] = 0;
    }

    result = distance[vertex2];
  }
  return result;
}

s21::Matrix<int> GraphAlgorithms::GetShortestPathsBetweenAllVertices(
    Graph &graph) {
  s21::Matrix<int> distance(graph.Matrix());

  for (size_t i = 0; i < distance.Rows(); ++i) {
    for (size_t z = 0; z < distance.Cols(); ++z) {
      if (distance(i, z) == 0 && i != z) distance(i, z) = INF;
    }
  }

  for (size_t i = 0; i < distance.Rows(); ++i) {
    for (size_t z = 0; z < distance.Rows(); ++z) {
      for (size_t x = 0; x < distance.Rows(); ++x) {
        if (distance(z, i) != INF && distance(i, x) != INF) {
          distance(z, x) == INF
              ? distance(z, x) = distance(z, i) + distance(i, x)
              : distance(z, x) =
                    std::min(distance(z, x), distance(z, i) + distance(i, x));
        }
      }
    }
  }

  return distance;
}

s21::Matrix<int> GraphAlgorithms::GetLeastSpanningTree(Graph &graph) {
  size_t matrix_size = graph.MatrixSize();
  s21::Matrix<int> tree(matrix_size, matrix_size);
  std::vector<bool> visited(matrix_size, false);
  visited[0] = true;
  std::vector<std::pair<int, std::pair<size_t, size_t>>> temp;
  while (!IsAllVertexVisited(visited)) {
    for (size_t i = 0; i < visited.size(); ++i) {
      if (visited[i]) {
        for (size_t z = 0; z < graph.MatrixSize(); ++z) {
          if (!visited[z] && i != z && graph(i, z) > 0) {
            temp.push_back(std::pair<int, std::pair<size_t, size_t>>(
                graph(i, z), std::pair<size_t, size_t>(i, z)));
          }
        }
      }
    }

    std::pair<int, std::pair<size_t, size_t>> *edge = &temp[0];
    for (size_t i = 0; i < temp.size(); ++i) {
      if (temp[i].first < edge->first) {
        edge = &temp[i];
      }
    }

    size_t y = edge->second.first;
    size_t x = edge->second.second;
    int val = edge->first;
    tree(y, x) = val;
    tree(x, y) = val;
    visited[x] = true;
    temp.clear();
  }

  return tree;
}

TsmResult GraphAlgorithms::SolveTravelingSalesmanProblem(Graph &graph) {
  s21::AntColony colony(graph);
  size_t total_ants = graph.Matrix().Size();
  int steps_without_improves = 30;
  TsmResult result, temp;
  result.distance = INFINITY;

  while (steps_without_improves) {
    colony.CreateAnts(total_ants);
    colony.ReleaseAnts();
    temp = colony.GetBestPath();
    if (result.distance == temp.distance) {
      --steps_without_improves;
    } else {
      result.distance = temp.distance;
      result.vertices = temp.vertices;
    }
  }
  if (result.vertices.size() < graph.MatrixSize())
    throw std::logic_error(
        "It is impossible to solve the traveling salesman problem given a "
        "given graph.");

  return result;
}

};  // namespace s21