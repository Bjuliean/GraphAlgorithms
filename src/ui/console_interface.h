#ifndef CONSOLE_INTERFACE_H
#define CONSOLE_INTERFACE_H

#include <sys/ioctl.h>
#include <unistd.h>
#include <list>

#include "../algorithms/s21_graph_algorithms.h"

namespace s21 {

class ConsoleInterface {
public:
    ConsoleInterface()
        : graph_()
        , graph_handler_()
        , ui_()
        , programm_running_(true)
        , menu_items_()
        , graph_status_("Graph was not loaded.")
        , is_graph_loaded_(false)
    {
        menu_items_.push_back(std::string("1. Load graph from file."));
        menu_items_.push_back(std::string("2. Breadth traversal of a graph."));
        menu_items_.push_back(std::string("3. Depth traversal of a graph."));
        menu_items_.push_back(std::string("4. Finding the shortest path between two vertices."));
        menu_items_.push_back(std::string("5. Finding the shortest path between all vertices."));
        menu_items_.push_back(std::string("6. Finding the minimum spanning tree in a graph."));
        menu_items_.push_back(std::string("7. Solving the traveling salesman problem."));
        menu_items_.push_back(std::string("8. Save graph."));
        menu_items_.push_back(std::string("Enter 'q' to exit."));
    }

    ~ConsoleInterface() = default;

    void Start();

private:
    s21::Graph graph_;
    s21::GraphAlgorithms graph_handler_;
    s21::Matrix<char> ui_;
    bool programm_running_;
    std::list<std::string> menu_items_;
    std::string graph_status_;
    bool is_graph_loaded_;

    void ShowMainWindow();

    void FillUIMatrix();

    void LoadMenuItems();

    bool HandleRequest();

    bool CheckRequestInput(char c);

    void LoadGraph();

    std::pair<size_t, size_t> MinimalWindowSize(std::list<std::string>& items);

    void SetGraphLoadingResult(std::string str);

    void BFS();

    void DFS();

    void PathBetweenTwoVertices();
    
    void PathBetweenAllVertices();

    void SpanningTree();

    void Salesman();

    void SaveGraph();

}; // ConsoleInterface

}; // namespace s21

#endif // CONSOLE_INTERFACE_H