#include "s21_graph.h"

namespace s21 {

Graph::Graph(const Graph& other) : adjacency_matrix_(other.adjacency_matrix_) {}

bool Graph::LoadGraphFromFile(std::string filename) {
  adjacency_matrix_.Clear();
  bool openres = false;
  std::ifstream fin;
  fin.open(filename);
  if (fin.is_open()) {
    openres = true;
    std::string str;
    while (std::getline(fin, str)) {
      adjacency_matrix_.Capacity() == 0 ? MatrixSettings(str)
                                        : ParseValues(str);
    }
  }
  fin.close();
  return openres;
}

void Graph::ExportGraphToDot(std::string filename) {
  if (!Utils::StrFormat(filename, ".dot")) filename += ".dot";
  std::ofstream fout;
  fout.open(filename);
  if (fout.is_open()) {
    fout << "graph dotfile {\n";
    for (size_t i = 0; i < adjacency_matrix_.Cols(); ++i) {
      for (size_t z = 0; z < adjacency_matrix_.Rows(); ++z) {
        if (adjacency_matrix_(i, z) != 0) {
          fout << "\t" << i + 1 << " -- " << z + 1;
          fout << " [label=" << adjacency_matrix_(i, z) << "];\n";
        }
      }
    }
    fout << "}\n";
  }
  fout.close();
}

void Graph::MatrixSettings(std::string str) {
  size_t size = std::atoi(str.c_str());
  adjacency_matrix_.SetColsSize(size);
  adjacency_matrix_.Reserve(size * size);
}

void Graph::ParseValues(std::string str) {
  std::vector<int> vec;
  char* lex = std::strtok((char*)str.data(), " ");
  while (lex != NULL) {
    vec.push_back(std::atoi(lex));
    lex = std::strtok(NULL, " ");
  }
  adjacency_matrix_.PushVector(vec);
}

};  // namespace s21