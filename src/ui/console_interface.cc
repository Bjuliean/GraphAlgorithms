#include "console_interface.h"

namespace s21 {

#define BACKGROUNDSYMB '0'

void ConsoleInterface::Start() {
  while (programm_running_) {
    ShowMainWindow();
    if (!HandleRequest()) {
      std::cout << "\nIncorrect menu item entered.\n--> OK";
      char c;
      std::cin >> c;
    }
  }
}

void ConsoleInterface::ShowMainWindow() {
  FillUIMatrix();
  LoadMenuItems();
  SetGraphLoadingResult(graph_status_);

  for (size_t i = 0; i < ui_.Rows(); ++i) {
    for (size_t z = 0; z < ui_.Cols(); ++z) {
      std::cout << ui_(i, z);
    }
    std::cout << "\n";
  }
}

void ConsoleInterface::FillUIMatrix() {
  ui_.Clear();
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  std::pair<size_t, size_t> wsize = MinimalWindowSize(menu_items_);
  size_t nwrow = static_cast<size_t>(w.ws_row) - 3 >= wsize.first ? w.ws_row - 3
                                                                  : wsize.first;
  size_t nwcol = static_cast<size_t>(w.ws_col) - 3 >= wsize.second
                     ? w.ws_col - 3
                     : wsize.second;
  ui_.Reserve(nwrow * nwcol);
  ui_.SetColsSize(nwcol);
  ui_.FillWithZeros();

  for (size_t i = 0; i < ui_.Rows(); ++i) {
    for (size_t z = 0; z < ui_.Cols(); ++z) {
      if (i == 0 || z == 0 || i == ui_.Rows() - 1 || z == ui_.Cols() - 1)
        ui_(i, z) = BACKGROUNDSYMB;
      else
        ui_(i, z) = ' ';
    }
  }
}

void ConsoleInterface::LoadMenuItems() {
  auto it = menu_items_.begin();
  for (size_t i = 1; i < ui_.Rows(); ++i) {
    for (size_t z = 2; z < ui_.Cols(); ++z) {
      if (z - 2 >= it->length()) break;
      ui_(i, z) = it->at(z - 2);
    }
    it++;
    if (it == menu_items_.end()) break;
  }
}

std::pair<size_t, size_t> ConsoleInterface::MinimalWindowSize(
    std::list<std::string>& items) {
  std::pair<size_t, size_t> res;
  res.first = items.size() + 2;
  res.second = 0;
  for (auto it : items) {
    if (it.length() > res.second) res.second = it.length();
  }
  res.second += 2;
  return res;
}

bool ConsoleInterface::HandleRequest() {
  std::cout << "\n--> ";
  char c = 0;
  std::cin >> c;
  if (!CheckRequestInput(c)) return false;
  switch (c) {
    case 'q':
      programm_running_ = false;
      break;
    case 'Q':
      programm_running_ = false;
      break;
    case '1':
      LoadGraph();
      break;
    case '2':
      BFS();
      break;
    case '3':
      DFS();
      break;
    case '4':
      PathBetweenTwoVertices();
      break;
    case '5':
      PathBetweenAllVertices();
      break;
    case '6':
      SpanningTree();
      break;
    case '7':
      Salesman();
      break;
    case '8':
      SaveGraph();
      break;
    default:
      break;
  }

  return true;
}

bool ConsoleInterface::CheckRequestInput(char c) {
  return (c >= '0' && c <= '9') || c == 'q' || c == 'Q';
}

void ConsoleInterface::LoadGraph() {
  std::string path;
  std::cout << "\nEnter the path to the file from the current directory: ";
  std::cin >> path;
  if (graph_.LoadGraphFromFile(path)) {
    graph_status_ = "Graph successfully loaded.";
    is_graph_loaded_ = true;
  } else {
    graph_status_ = "Failed to load graph.";
    is_graph_loaded_ = false;
  }
}

void ConsoleInterface::SetGraphLoadingResult(std::string str) {
  size_t free_row = 0;
  for (size_t i = 1; i < ui_.Rows(); ++i) {
    for (size_t z = 1; z < ui_.Cols() - 1; ++z) {
      if (ui_(i, z) == ' ' && ui_(i, z + 1) == ' ') {
        free_row = i;
        break;
      }
    }
  }
  for (size_t i = 0; i < str.length(); ++i) {
    ui_(free_row, i + 2) = str[i];
  }
}

void ConsoleInterface::BFS() {
  if (is_graph_loaded_) {
    std::cout << "\nBreadth First Search\nEnter start vertex number(from 1). ";
    size_t num = 0;
    std::cin >> num;
    std::vector<int> res = graph_handler_.BreadthFirstSearch(graph_, num);

    std::cout << "\n";
    for (size_t i = 0; i < res.size(); ++i) {
      i == res.size() - 1 ? std::cout << res[i] : std::cout << res[i] << "->";
    }
  } else {
    std::cout << "\nGraph was not loaded.";
  }
  std::cout << "\n--> OK";
  char c;
  std::cin >> c;
}

void ConsoleInterface::DFS() {
  if (is_graph_loaded_) {
    std::cout << "\nDepth First Search\nEnter start vertex number(from 1). ";
    size_t num = 0;
    std::cin >> num;
    std::vector<int> res = graph_handler_.DepthFirstSearch(graph_, num);

    std::cout << "\n";
    for (size_t i = 0; i < res.size(); ++i) {
      i == res.size() - 1 ? std::cout << res[i] : std::cout << res[i] << "->";
    }
  } else {
    std::cout << "\nGraph was not loaded.";
  }
  std::cout << "\n--> OK";
  char c;
  std::cin >> c;
}

void ConsoleInterface::PathBetweenTwoVertices() {
  if (is_graph_loaded_) {
    std::cout << "\nMinimal path between two vertices\nEnter start vertex "
                 "number(from 1). ";
    size_t num1 = 0;
    std::cin >> num1;
    std::cout << "\nEnter end vertex number(from 1). ";
    size_t num2 = 0;
    std::cin >> num2;

    int res = graph_handler_.GetShortestPathBetweenVertices(graph_, num1, num2);

    std::cout << "\nThe minimum path takes " << res << " conventional units.";
  } else {
    std::cout << "\nGraph was not loaded.";
  }
  std::cout << "\n--> OK";
  char c;
  std::cin >> c;
}

void ConsoleInterface::PathBetweenAllVertices() {
  if (is_graph_loaded_) {
    std::cout << "\nMinimal path between all vertices, matrix representation. ";

    s21::Matrix<int> res =
        graph_handler_.GetShortestPathsBetweenAllVertices(graph_);

    std::cout << "\n";
    for (size_t i = 0; i < res.Rows(); ++i) {
      for (size_t z = 0; z < res.Cols(); ++z) {
        z == res.Cols() - 1 ? std::cout << res(i, z)
                            : std::cout << res(i, z) << " ";
      }
      std::cout << "\n";
    }

  } else {
    std::cout << "\nGraph was not loaded.";
  }
  std::cout << "\n--> OK";
  char c;
  std::cin >> c;
}

void ConsoleInterface::SpanningTree() {
  if (is_graph_loaded_) {
    std::cout << "\nSpanning tree, matrix representation. ";

    s21::Matrix<int> res = graph_handler_.GetLeastSpanningTree(graph_);

    std::cout << "\n";
    for (size_t i = 0; i < res.Rows(); ++i) {
      for (size_t z = 0; z < res.Cols(); ++z) {
        z == res.Cols() - 1 ? std::cout << res(i, z)
                            : std::cout << res(i, z) << " ";
      }
      std::cout << "\n";
    }

  } else {
    std::cout << "\nGraph was not loaded.";
  }
  std::cout << "\n--> OK";
  char c;
  std::cin >> c;
}

void ConsoleInterface::Salesman() {
  if (is_graph_loaded_) {
    std::cout << "\nSolving the traveling salesman problem. ";

    s21::TsmResult res = graph_handler_.SolveTravelingSalesmanProblem(graph_);

    std::cout << "\nRoute length: " << res.distance << "\n";
    for (size_t i = 0; i < res.vertices.size(); ++i) {
      i == res.vertices.size() - 1 ? std::cout << res.vertices[i]
                                   : std::cout << res.vertices[i] << "->";
    }

  } else {
    std::cout << "\nGraph was not loaded.";
  }
  std::cout << "\n--> OK";
  char c;
  std::cin >> c;
}

void ConsoleInterface::SaveGraph() {
  if (is_graph_loaded_) {
    std::cout << "\nEnter the path of the saving file: ";
    std::string str;
    std::cin >> str;
    graph_.ExportGraphToDot(str);
    std::cout << "\nSuccessfully saved.";
  } else {
    std::cout << "\nGraph was not loaded.";
  }
  std::cout << "\n--> OK";
  char c;
  std::cin >> c;
}

};  // namespace s21